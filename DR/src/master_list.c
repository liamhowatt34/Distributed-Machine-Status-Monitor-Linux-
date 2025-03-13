//
// Project: Distributed Machine Status Monitor
// File: master_list.c
// Programmer: Liam Howatt
// Date: March 9, 2025
// Description: This file contains the functions
//              related to the master list functionality.
//
#include "../inc/master_list.h"


// Function: int check_master_list_for_pid(MasterList* master_list, Message* message)
// Purpose: Check the dcs in the master list for the dc pid that sent the message.
int check_master_list_for_pid(MasterList* master_list, Message* message) {
    for (int i = 0; i < master_list->num_of_dcs; i++) {
            if (master_list->dc[i].dc_pid == message->machine_pid) {
                return SEEN_MACHINE;
            }
        }
    return UNKNOWN_MACHINE;
}


// Function: MasterList* handle_message(MasterList* master_list, Message* message, int seen, char log_status_str[], FILE* log_file)
// Purpose: Determine what to do with the message we received, either update or add to the master list.
MasterList* handle_message(MasterList* master_list, Message* message, int seen, char log_status_str[], FILE* log_file) {
    if (seen) {
        // Update DCInfo in master list
        for (int i = 0; i < master_list->num_of_dcs; i++) {
            if (master_list->dc[i].dc_pid != message->machine_pid) {
                continue;
            }

            master_list->dc[i].status = message->status;
            master_list->dc[i].last_time_heard_from = time(NULL);

            log_updated_dc(log_file, log_status_str, message, i + 1);
            break;
        }

        return master_list;
    }

    if (master_list->num_of_dcs == MAX_DC_ROLES) {
        return master_list;
    }

    DCInfo dc_info = {
        .dc_pid = message->machine_pid,
        .last_time_heard_from = time(NULL),
        .status = message->status
    };

    // Add new DC to master list
    int first_free_index = master_list->num_of_dcs;
    master_list->dc[first_free_index] = dc_info;
    master_list->num_of_dcs++;

    log_new_dc(log_file, master_list->num_of_dcs, dc_info.dc_pid);

    return master_list;
}


// Function: MasterList* check_dcs_time(MasterList* master_list, FILE* log_file)
// Purpose: Compare the time of the dcs to now to see if they have
//          exceeded 35 seconds without messaging, if so, delete dc.
MasterList* check_dcs_time(MasterList* master_list, FILE* log_file) {
    time_t now = time(NULL);

    for (int i = 0; i < master_list->num_of_dcs; i++) {
        if (difftime(now, master_list->dc[i].last_time_heard_from) > MAX_DC_TIME) {
            log_dc_removal(log_file, i + 1, master_list->dc[i].dc_pid);

            // Shift the array
            for (int j = i; j < master_list->num_of_dcs - 1; j++) {
                master_list->dc[j] = master_list->dc[j + 1];
            }

            // Adjust num of dcs and index
            master_list->num_of_dcs--;
            i--;
        }
    }

    return master_list;
}


// Function: MasterList* check_if_offline_msg_received(MasterList* master_list, Message* message) 
// Purpose: If machine offline message is received, we remove that dc from the master list.
MasterList* check_if_offline_msg_received(MasterList* master_list, Message* message) {
    if (message->status != MACHINE_OFFLINE) {
        return master_list;
    }

    for (int i = 0; i < master_list->num_of_dcs; i++) {
        if (master_list->dc[i].dc_pid != message->machine_pid) {
            continue;
        }

        // Shift the array
        for (int j = i; j < master_list->num_of_dcs - 1; j++) {
            master_list->dc[j] = master_list->dc[j + 1];
        }
        // Adjust num of dcs
        master_list->num_of_dcs--;
        break;
    }

    return master_list;
}


// Function: bool dcs_exist(MasterList* master_list)
// Purpose: Check if the num of dcs > 0
bool dcs_exist(MasterList* master_list) {
    return master_list->num_of_dcs > 0;
}


// Function: int get_dc_number(MasterList* master_list, Message* message)
// Purpose: Get the dc number in the master list that matches the dc that sent the message.
int get_dc_number(MasterList* master_list, Message* message) {
    for (int i = 0; i < master_list->num_of_dcs; i++) {
        if (message->machine_pid == master_list->dc[i].dc_pid) {
            return i + 1;
        }
    }

    return -1;
}


// Function: void log_dc_removal(FILE* log_file, int dc_number, int machine_pid)
// Purpose: Log the removal of a dc from the master list.
void log_dc_removal(FILE* log_file, int dc_number, int machine_pid) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);

    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DC-%d [%d] removed from master list - NON-RESPONSIVE\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        dc_number, machine_pid);

    fflush(log_file);
}


// Function: void log_new_dc(FILE* log_file, int dc_number, int machine_pid)
// Purpose: Log a new dc added to the master list.
void log_new_dc(FILE* log_file, int dc_number, int machine_pid) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);

    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DC-%d [%d] added to the master list - NEW DC - Status 0 (Everything is OKAY)\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        dc_number, machine_pid);

    fflush(log_file);
}


// Function: void log_updated_dc(FILE* log_file, char status_str[], Message* message, int dc_number)
// Purpose: Log an update the a dc in the master list.
void log_updated_dc(FILE* log_file, char status_str[], Message* message, int dc_number) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);

    if (message->status == MACHINE_OFFLINE) {
        fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DC-%d [%d] has gone OFFLINE - removing from the master list\n",
            local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
            dc_number, message->machine_pid);
        fflush(log_file);
        return;
    }

    get_status_string(status_str, message->status);
    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DC-%d [%d] updated in the master list - MSG RECEIVED - Status %d (%s)\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        dc_number, message->machine_pid, message->status, status_str);

    fflush(log_file);
}


// Function: void log_all_dcs_offline(FILE* log_file)
// Purpose: Log an message that all dcs are offline.
void log_all_dcs_offline(FILE* log_file) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    
    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : All DCs have gone offline or terminated - DR TERMINATING\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    
    fflush(log_file);
}