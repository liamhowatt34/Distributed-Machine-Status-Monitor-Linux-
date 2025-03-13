#pragma once
#include "util.h"

#define MIN_SECONDS 10
#define MAX_SECONDS 30

void get_status_string(char status_buffer[], int status);
int random_time();
void log_error(FILE* log_file, const char* error_message);