#ifndef nativeQutil__h
#define nativeQutil__h

#ifdef __cplusplus
extern "C"
{
#endif

#include "cJSON.h"
#include "sqlite3.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *get_formatted_time();

FILE *create_log_file(const char*);

int write_log(FILE *, const char*);

#ifdef __cplusplus
}
#endif

#endif
