
#include "list.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum {
  CSV_OK = 0,
  CSV_ERR_BADPTR,
  CSV_ERR_NOMEM,
  CSV_ERR_FILE_NOT_FOUND,
  CSV_ERR_INVALID_FILE,
  CSV_ERR_UNKNOWN
} CsvError;

typedef enum { CSV_QUOTED = 0, CSV_UNQUOTED, CSV_QUOTE_SEEN } CsvParserState;

struct CsvTable {
  struct List *rows;
};

struct CsvRow {
  struct List *fields;
};

struct CsvField {
  struct List *data;
};

void print_ints(void *value);
void print_chars(void *value);
void print_table(void *value);
void print_row(void *value);
void print_field(void *value);
void print_error(int err);

// CsvParser
CsvError csv_parse_string(struct CsvTable *table, const char *csv_string);
CsvError csv_parse_file(struct CsvTable *table, const char *file_path);

// CsvTable functions
void csv_table_free(void *);
struct CsvTable *csv_table_new();
CsvError csv_table_add(struct CsvTable *table, const struct CsvRow *row);

// CsvRow functions
void csv_row_free(void *);
CsvError csv_row_add(struct CsvRow *row, const struct CsvField *field);
struct CsvRow *csv_row_new();

// CsvField functions
void csv_field_free(void *);
struct CsvField *csv_field_new();

static CsvError map_list_error(ListError err);

int main(int argc, char **argv) {
  CsvError err = CSV_OK;

  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  struct CsvTable *table = csv_table_new();
  if (table == NULL) {
    return CSV_ERR_NOMEM;
  }

  if (argc == 1) {
    err = csv_parse_string(table, "a\n\"\"");
  } else {
    err = csv_parse_file(table, argv[1]);
  }
  if (err != CSV_OK) {
    printf("oh my god we fuckin died with err=%d\n", err);
  }

  // print_table(table);
  printf("rows=%zu\n", table->rows->length);
  printf("\n");
  csv_table_free(table);

  clock_gettime(CLOCK_MONOTONIC, &end);

  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("⏱️ Total Parsing Time: %f seconds (%f ms)\n", time_taken,
         time_taken * 1000.0);

  return err;
}

void print_ints(void *value) { printf("%d", *(int *)value); }

void print_chars(void *value) { printf("%c", *(char *)value); }

void print_table(void *value) {
  list_print(((struct CsvTable *)value)->rows, print_row);
}

void print_row(void *value) {
  list_print(((struct CsvRow *)value)->fields, print_field);
}

void print_field(void *value) {
  list_raw_print(((struct CsvField *)value)->data, print_chars);
}

CsvError csv_parse_string(struct CsvTable *table, const char *csv_string) {
  CsvError err = CSV_OK;
  if (strlen(csv_string) == 0) {
    return err;
  }
  int current_state = CSV_UNQUOTED;
  int pending_field = 1;

  struct CsvRow *row = csv_row_new();
  struct CsvField *field = csv_field_new();
  if (row == NULL || field == NULL) {
    printf("shit's fucked yo, abort abort oh my god we're gonna die\n");
    err = CSV_ERR_NOMEM;
    goto cleanup;
  }

  for (size_t i = 0; csv_string[i] != '\0'; i++) {
    switch (csv_string[i]) {
    case '\n':
      if (current_state != CSV_QUOTED) {
        err = csv_row_add(row, field);
        if (err != CSV_OK) {
          goto cleanup;
        }
        free(field);
        field = NULL;
        err = csv_table_add(table, row);
        if (err != CSV_OK) {
          goto cleanup;
        }
        free(row);
        field = csv_field_new();
        row = csv_row_new();
        pending_field = 0;

        if (field == NULL || row == NULL) {
          err = CSV_ERR_BADPTR;
          goto cleanup;
        }

        current_state = CSV_UNQUOTED;
      } else {
        err = map_list_error(list_add(field->data, &(csv_string[i])));
        if (err != CSV_OK) {
          goto cleanup;
        }
        current_state = CSV_QUOTED;
      }
      break;
    case '"':
      if (current_state == CSV_UNQUOTED) {
        pending_field = 1;
        current_state = CSV_QUOTED;
      } else if (current_state == CSV_QUOTED) {
        current_state = CSV_QUOTE_SEEN;
      } else { // CSV_QUOTE_SEEN
        err = map_list_error(list_add(field->data, &(csv_string[i])));
        if (err != CSV_OK) {
          goto cleanup;
        }
        current_state = CSV_QUOTED;
      }
      break;
    case ',':
      if (current_state != CSV_QUOTED) {
        err = csv_row_add(row, field);
        pending_field = 0;
        if (err != CSV_OK) {
          goto cleanup;
        }
        free(field);
        field = csv_field_new();
        if (field == NULL) {
          err = CSV_ERR_BADPTR;
          goto cleanup;
        }

        pending_field = 1;
        current_state = CSV_UNQUOTED;
      } else {
        err = map_list_error(list_add(field->data, &(csv_string[i])));
        if (err != CSV_OK) {
          goto cleanup;
        }
        current_state = CSV_QUOTED;
      }
      break;
    case '\r':
      if (current_state == CSV_QUOTED) {
        err = map_list_error(list_add(field->data, &(csv_string[i])));
        if (err != CSV_OK) {
          goto cleanup;
        }
        current_state = CSV_QUOTED;
      }
      break;
    default:
      pending_field = 1;
      err = map_list_error(list_add(field->data, &(csv_string[i])));
      if (err != CSV_OK) {
        goto cleanup;
      }
      if (current_state != CSV_QUOTED) {
        current_state = CSV_UNQUOTED;
      } else {
        current_state = CSV_QUOTED;
      }
    }
  }

  if (pending_field == 1) {
    err = csv_row_add(row, field);
    if (err != CSV_OK)
      goto cleanup;
    free(field); // row owns field->data now
    field = NULL;

    err = csv_table_add(table, row);
    if (err != CSV_OK)
      goto cleanup;
    free(row); // table owns row->fields now
  } else {
    list_free(field->data);
    free(field);
    list_free(row->fields);
    free(row);
  }

  return err;

cleanup:
  if (field != NULL) {
    list_free(field->data);
    free(field);
  }

  if (row != NULL) {
    list_free(row->fields);
    free(row);
  }

  return err;
}

CsvError csv_parse_file(struct CsvTable *table, const char *file_path) {
  printf("attempting to open file %s\n", file_path);
  FILE *csv_fd = fopen(file_path, "rb");
  if (csv_fd == NULL) {
    printf("unable to find file %s\n", file_path);
    return CSV_ERR_FILE_NOT_FOUND;
  }

  fseek(csv_fd, 0, SEEK_END);
  long buffer_size = ftell(csv_fd);
  if (buffer_size < 0) {
    fclose(csv_fd);
    return CSV_ERR_INVALID_FILE;
  }
  size_t member_size = sizeof(char);

  printf("file %s found, size %ld bytes\n", file_path, buffer_size);

  char *buffer = malloc((member_size * buffer_size) + 1);
  if (buffer == NULL) {
    fclose(csv_fd);
    return CSV_ERR_NOMEM;
  }

  fseek(csv_fd, 0, SEEK_SET);
  size_t chars_read =
      fread(buffer, member_size, buffer_size / member_size, csv_fd);
  // malloc 1 extra byte and set it to nul terminator because fread strictly
  // reads until EOF
  buffer[chars_read] = '\0';

  printf("read %zu members, expected %ld bytes, got %zu bytes\n", chars_read,
         buffer_size, chars_read * (size_t)member_size);

  fclose(csv_fd);

  if (chars_read != (size_t)buffer_size) {
    free(buffer);
    return CSV_ERR_INVALID_FILE;
  }

  CsvError result = csv_parse_string(table, buffer);
  free(buffer);
  return result;
}

struct CsvTable *csv_table_new() {
  struct CsvTable *table = malloc(sizeof(struct CsvTable));
  if (table == NULL) {
    return NULL;
  }

  table->rows = list_new(sizeof(struct CsvRow), csv_row_free);
  if (table->rows == NULL) {
    free(table);
    return NULL;
  }

  return table;
}

void csv_table_free(void *value) {
  list_free(((struct CsvTable *)value)->rows);
  free(value);
}

CsvError csv_table_add(struct CsvTable *table, const struct CsvRow *row) {
  return map_list_error(list_add(table->rows, row));
}

void csv_row_free(void *value) { list_free(((struct CsvRow *)value)->fields); }

CsvError csv_row_add(struct CsvRow *row, const struct CsvField *field) {
  return map_list_error(list_add(row->fields, field));
}

void csv_field_free(void *value) {
  list_free(((struct CsvField *)value)->data);
}

struct CsvRow *csv_row_new() {
  struct CsvRow *row = malloc(sizeof(struct CsvRow));
  if (row == NULL) {
    return NULL;
  }
  row->fields = list_new(sizeof(struct CsvField), csv_field_free);
  if (row->fields == NULL) {
    free(row);
    return NULL;
  }

  return row;
}

struct CsvField *csv_field_new() {
  struct CsvField *field = malloc(sizeof(struct CsvField));
  if (field == NULL) {
    return NULL;
  }

  field->data = list_new(sizeof(char), NULL);
  if (field->data == NULL) {
    free(field);
    return NULL;
  }

  return field;
}

static CsvError map_list_error(ListError err) {
  switch (err) {
  case LIST_OK:
    return CSV_OK;
  case LIST_ERR_BADPTR:
    return CSV_ERR_BADPTR;
  case LIST_ERR_NOMEM:
    return CSV_ERR_NOMEM;
  default:
    return CSV_ERR_UNKNOWN;
  }
}
