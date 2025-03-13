//
// Project: Distributed Machine Status Monitor
// File: message.c
// Programmer: Liam Howatt
// Description: This file contains the necessary functions 
//              relating to the messaging of the DC machine.
//
#include "../inc/message.h"


// Function: Message create_message(pid_t machine_pid, int dc_status)
// Purpose: This function creates a message with the process id
//          and the status we want to send to the DR.
Message create_message(pid_t machine_pid, int dc_status) {
    Message message = {
        .message_type = 1,
        .machine_pid = machine_pid,
        .status = dc_status
    };
    return message;
}


// Function: random_status()
// Purpose: Returns a number between 0 and 7 to determine
//          the status we are sending to the Data Reader.
int random_status() {
    return rand() % 7;
}


// Function: void log_dc(FILE* log_file, char status_str[], int status_to_send, int machine_pid)
// Purpose: Logs the correct status to the DC's log file.
void log_dc(FILE* log_file, char status_str[], int status_to_send, int machine_pid) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    get_status_string(status_str, status_to_send);
    
    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : DC [%d] - MSG SENT - Status %d (%s)\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        machine_pid, status_to_send, status_str);

    fflush(log_file);
}