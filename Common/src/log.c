//
// Project: Distributed Machine Status Monitor
// File: log.c
// Programmer: Liam Howatt
// Description: This file has the function definitions for the log related files.
//
#include "../inc/log.h"


// Function: void get_status_string(char status_buffer[], int status)
// Purpose: Copies the correct status string to the status_buffer depending
//          on the status.
void get_status_string(char status_buffer[], int status) {
    switch (status) {
    case EVERYTHING_OKAY:
        strncpy(status_buffer, "Everything is OKAY", MAX_STATUS_LEN);
        break;
    case HYDRAULIC_PRESSURE_FAILURE:
        strncpy(status_buffer, "Hydraulic Pressure Failure", MAX_STATUS_LEN);
        break;
    case SAFETY_BUTTON_FAILURE:
        strncpy(status_buffer, "Safety Button Failure", MAX_STATUS_LEN);
        break;
    case NO_RAW_MATERIAL:
        strncpy(status_buffer, "No Raw Material in the Process", MAX_STATUS_LEN);
        break;
    case TEMP_OUT_OF_RANGE:
        strncpy(status_buffer, "Operating Temperature Out of Range", MAX_STATUS_LEN);
        break;
    case OPERATOR_ERROR:
        strncpy(status_buffer, "Operator Error", MAX_STATUS_LEN);
        break;
    case MACHINE_OFFLINE:
        strncpy(status_buffer, "Machine is Off-line", MAX_STATUS_LEN);
        break;
    default:
        status_buffer[0] = '\0';
        break;
    }
    status_buffer[MAX_STATUS_LEN - 1] = '\0';
}


// Function: random_time()
// Purpose: Returns a number between 10 and 30 to determine
//          the amount of time to wait b/w messages.
int random_time() {
    return rand() % (MAX_SECONDS - MIN_SECONDS + 1) + MIN_SECONDS;
}


// Function: void log_error(FILE* log_file, const char* error_message) 
// Purpose: Logs strerror(errno) messages to a log file.
void log_error(FILE* log_file, const char* error_message) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    
    fprintf(log_file, "[%d-%d-%d %d:%d:%d] : %s\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        error_message);
    
    fflush(log_file);
}