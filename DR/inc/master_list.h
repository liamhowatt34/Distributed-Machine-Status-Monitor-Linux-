#pragma once
#include <sys/shm.h>
#include "../../Common/inc/util.h"
#include "../../Common/inc/log.h"

#define SEEN_MACHINE 1
#define UNKNOWN_MACHINE 0
#define MAX_DC_TIME 35
#define MAX_DC_ROLES 10

typedef struct {
    pid_t dc_pid;
    time_t last_time_heard_from;
    int status;
} DCInfo;

typedef struct {
    int q_id;
    int num_of_dcs;
    DCInfo dc[MAX_DC_ROLES];   
} MasterList;

int check_master_list_for_pid(MasterList* master_list, Message* message);
MasterList* handle_message(MasterList* master_list, Message* message, int seen, char log_status_str[], FILE* log_file);
MasterList* check_dcs_time(MasterList* master_list, FILE* log_file);
MasterList* check_if_offline_msg_received(MasterList* master_list, Message* message);
bool dcs_exist(MasterList* master_list);
int get_dc_number(MasterList* master_list, Message* message);
void log_dc_removal(FILE* log_file, int dc_number, int machine_pid);
void log_new_dc(FILE* log_file, int dc_number, int machine_pid);
void log_updated_dc(FILE* log_file, char status_str[], Message* message, int dc_number);
void log_all_dcs_offline(FILE* log_file);