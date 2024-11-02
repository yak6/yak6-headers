#ifndef PARSE_H
#define PARSE_H

char* read_value(const char *file_name, const char *key);
int overwrite_value(const char *file_name, const char *key, const char *new_value);

#endif // PARSE_H
