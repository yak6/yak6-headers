#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 512

#include "parse.h"

char* read_value(const char *file_name, const char *key) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE];
    char *found_key, *value;

    while (fgets(line, sizeof(line), file)) {
        found_key = strtok(line, ":");
        value = strtok(NULL, "\n");

        if (found_key && value && strcmp(found_key, key) == 0) {
            fclose(file);
            char *result = malloc(strlen(value) + 1);
            strcpy(result, value);
            return result;
        }
    }

    fclose(file);
    return NULL;
}

int overwrite_value(const char *file_name, const char *key, const char *new_value) {
    FILE *file = fopen(file_name, "r+");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    char line[MAX_LINE];
    char *found_key;
    int found = 0;

    FILE *tmp_file = tmpfile();
    if (!tmp_file) {
        perror("Failed to open temporary file");
        fclose(file);
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        found_key = strtok(line, ":");

        if (found_key && strcmp(found_key, key) == 0) {
            fprintf(tmp_file, "%s:%s\n", key, new_value);
            found = 1;
        } else {
            fprintf(tmp_file, "%s:%s", found_key, strtok(NULL, "\n"));
        }
    }

    if (!found) {
        fprintf(tmp_file, "%s:%s\n", key, new_value);
    }

    rewind(file);
    rewind(tmp_file);

    while (fgets(line, sizeof(line), tmp_file)) {
        fputs(line, file);
    }

    fflush(file);
    ftruncate(fileno(file), ftell(file));

    fclose(file);
    fclose(tmp_file);

    return 0;
}
