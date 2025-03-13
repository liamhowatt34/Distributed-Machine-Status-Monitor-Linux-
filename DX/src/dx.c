//
// Project: Distributed Machine Status Monitor
// File: dx.c
// Programmer: Liam Howatt
// Description: This file containes the functions relating to
//              the DX application.
//
#include "../inc/dx.h"


// Function: void log_dx_queue_doesnt_exist(FILE* log_file)
// Purpose: Log when the message queue does not exist yet.
void log_dx_queue_doesnt_exist(FILE* log_file) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    
    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DX detected that msgQ is gone - assuming DR/DCs done\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    
    fflush(log_file);
}


// Function: int select_random_wod_item()
// Purpose: Select a random number between 0 and 20.
int select_random_wod_item() {
    return rand() % (TWENTY - ZERO + 1) + ZERO;
}


// Function: int get_pid_of_dc(MasterList* master_list, int dc_number)
// Purpose: Iterate over the master list until the dc that matches the dc_number is found.
int get_pid_of_dc(MasterList* master_list, int dc_number) {
    for (int i = 0; i < master_list->num_of_dcs; i++) {
        if (dc_number == i + 1) {
            return master_list->dc[i].dc_pid;
        }
    }

    return -1;
}


// Function: bool does_dc_exist(MasterList* master_list, int dc_number)
// Purpose: See if the dc_number exists in the master list.
bool does_dc_exist(MasterList* master_list, int dc_number) {
    return dc_number <= master_list->num_of_dcs;
}


// Function: void log_wod_task(FILE* log_file, int action_number, int dc_number, int target_pid)
// Purpose: Log when executing a wod task. 
void log_wod_task(FILE* log_file, int action_number, int dc_number, int target_pid) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);

    if (action_number == TEN_DELETE_MSG_Q || action_number == SEVENTEEN_DELETE_MSG_Q) {
        fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DX deleted the msgQ - the DR/DCs can't talk anymore - exiting\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

        fflush(log_file);
        return;
    }

    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : WOD Action %d - DC-%d [%d] TERMINATED\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        action_number, dc_number, target_pid);
    fflush(log_file);
}


// Function: int execute_wod_task(int wod_selection, MasterList* master_list, int q_id, FILE* log_file) 
// Purpose: Execute a selected wod task. Usually killing a dc in the master list if it exists.
//          Will also deleted the message queue possibly. Return 0 on success or -1 on kill fail.
int execute_wod_task(int wod_selection, MasterList* master_list, int q_id, FILE* log_file) {
    bool dc_exists = false;
    int target_pid = get_pid_of_dc(master_list, DC1);
    int free_q_return_code;
    int result_code = 0;

    switch (wod_selection) {
    case ZERO_DO_NOTHING:
        break;
    case ONE_KILL_DC1:
        dc_exists = does_dc_exist(master_list, DC1);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC1);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, ONE_KILL_DC1, DC1, target_pid);
        break;
    case TWO_KILL_DC3:
        dc_exists = does_dc_exist(master_list, DC3);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC3);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, TWO_KILL_DC3, DC3, target_pid);
        break;
    case THREE_KILL_DC2:
        dc_exists = does_dc_exist(master_list, DC2);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC2);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, THREE_KILL_DC2, DC2, target_pid);
        break;
    case FOUR_KILL_DC1:
        dc_exists = does_dc_exist(master_list, DC1);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC1);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, FOUR_KILL_DC1, DC1, target_pid);
        break;
    case FIVE_KILL_DC3:
        dc_exists = does_dc_exist(master_list, DC3);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC3);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, FIVE_KILL_DC3, DC3, target_pid);
        break;
    case SIX_KILL_DC2:
        dc_exists = does_dc_exist(master_list, DC2);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC2);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, SIX_KILL_DC2, DC2, target_pid);
        break;
    case SEVEN_KILL_DC4:
        dc_exists = does_dc_exist(master_list, DC4);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC4);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, SEVEN_KILL_DC4, DC4, target_pid);
        break;
    case EIGHT_DO_NOTHING:
        break;
    case NINE_KILL_DC5:
        dc_exists = does_dc_exist(master_list, DC5);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC5);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, NINE_KILL_DC5, DC5, target_pid);
        break;
    case TEN_DELETE_MSG_Q:
        free_q_return_code = msgctl(q_id, IPC_RMID, NULL);
            if (free_q_return_code == -1) {
                result_code = -1;
            }
        log_wod_task(log_file, TEN_DELETE_MSG_Q, 0, target_pid);
        break;
    case ELEVEN_KILL_DC1:
        dc_exists = does_dc_exist(master_list, DC1);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC1);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, ELEVEN_KILL_DC1, DC1, target_pid);
        break;
    case TWELVE_KILL_DC6:
        dc_exists = does_dc_exist(master_list, DC6);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC6);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, TWELVE_KILL_DC6, DC6, target_pid);
        break;
    case THIRTEEN_KILL_DC2:
        dc_exists = does_dc_exist(master_list, DC2);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC2);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, THIRTEEN_KILL_DC2, DC2, target_pid);
        break;
    case FOURTEEN_KILL_DC7:
        dc_exists = does_dc_exist(master_list, DC7);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC7);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, FOURTEEN_KILL_DC7, DC7, target_pid);
        break;
    case FIFTEEN_KILL_DC3:
        dc_exists = does_dc_exist(master_list, DC3);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC3);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, FIFTEEN_KILL_DC3, DC3, target_pid);
        break;
    case SIXTEEN_KILL_DC8:
        dc_exists = does_dc_exist(master_list, DC8);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC8);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, SIXTEEN_KILL_DC8, DC8, target_pid);
        break;
    case SEVENTEEN_DELETE_MSG_Q:
        free_q_return_code = msgctl(q_id, IPC_RMID, NULL);
            if (free_q_return_code == -1) {
                result_code = -1;
            }
        log_wod_task(log_file, SEVENTEEN_DELETE_MSG_Q, 0, target_pid);
        break;
    case EIGHTEEN_KILL_DC9:
        dc_exists = does_dc_exist(master_list, DC9);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC9);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, EIGHTEEN_KILL_DC9, DC9, target_pid);
        break;
    case NINETEEN_DO_NOTHING:
        break;
    case TWENTY_KILL_DC10:
        dc_exists = does_dc_exist(master_list, DC10);
        if (!dc_exists) {
            break;
        }

        target_pid = get_pid_of_dc(master_list, DC10);
        if (kill(target_pid, SIGHUP) == -1) {
            result_code = -1;
        }
        log_wod_task(log_file, TWENTY_KILL_DC10, DC10, target_pid);
        break;
    default:
        break;
    }

    return result_code;
}