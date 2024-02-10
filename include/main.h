#ifndef MAIN_H
#define MAIN_H

void parse_bool(const char *value, bool *item);

void parse_subpair(const char *value, keyval *item);

int handle_config(void* user, const char* section, const char* name, const char* value);

int main(int argc, char const *argv[]);

int main_main();

char const *argv[]);

int main2(int argc, char const *argv[]);


#endif