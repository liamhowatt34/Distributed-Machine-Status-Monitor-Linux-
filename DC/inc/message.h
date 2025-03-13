#pragma once
#include "../../Common/inc/util.h"
#include "../../Common/inc/log.h"


Message create_message(pid_t machine_pid, int dc_status);
int random_status();
void log_dc(FILE* log_file, char status_str[], int status_to_send, int machine_pid);