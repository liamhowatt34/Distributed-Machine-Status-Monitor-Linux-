//
// Project: Distributed Machine Status Monitor
// File: main.c
// Programmer: Liam Howatt
// Description: This file is the main starting point for the
//              DC application.
//
#include "../inc/message.h"

int main(void) {
    // Seed for random num generator
    srand(time(NULL));

    // Open log file
    FILE* log_file = NULL;
    log_file = fopen("tmp/dataCreator.log", "a");
    if (log_file == NULL) {
        log_error(log_file, strerror(errno));
        return -1;
    }

    // String for logging status
    char log_status_str[MAX_STATUS_LEN] = {0};

    // Generate key for the message queue
    key_t message_key = ftok("Common/inc/util.h", DC_ID);
    if (message_key == -1) {
        log_error(log_file, strerror(errno));
        fclose(log_file);
        return -1;
    }

    // Check if the message queue exists
    int q_id;
    while (1) {
        if ((q_id = msgget(message_key,0)) == -1) {
            sleep(10);
        } else {
            break;
        }
    }

    // Get process id
    pid_t machine_pid = getpid();

    // Send first message statinh everything is okay
    Message first_message = create_message(machine_pid, EVERYTHING_OKAY);
    size_t message_size = sizeof(Message) - sizeof(long);

    int send_result = msgsnd(q_id, &first_message, message_size, 0);
    if (send_result == -1) {
        log_error(log_file, strerror(errno));
        fclose(log_file);
        return -1;
    }

    log_dc(log_file, log_status_str, EVERYTHING_OKAY, machine_pid);

    // Send messages until machine is offline is selected and sent
    int status_to_send = 0;
    int interval = 0;

    while (1) {
        // Reset log status str at the beginning of each iteration 
        memset(log_status_str, 0, sizeof(log_status_str));
        
        // Sleep b/w 10 and 30 seconds
        interval = random_time();
        sleep(interval);

        // Ensure the queue still exists
        if ((q_id = msgget(message_key,0)) == -1) {
            log_error(log_file, "Queue no longer exists, DC EXITING\n");
            fclose(log_file);
            break;
        } 

        // Create a message
        status_to_send = random_status();
        Message message = create_message(machine_pid, status_to_send);

        // Send message
        int send_result = msgsnd(q_id, &message, sizeof(Message) - sizeof(long), 0);
        if (send_result == -1) {
            log_error(log_file, strerror(errno));
            fclose(log_file);
            return -1;
        }
        
        // Log sent message
        log_dc(log_file, log_status_str, status_to_send, machine_pid);

        if (status_to_send == MACHINE_OFFLINE) {
            break;
        }
    }

    fclose(log_file);
    return 0;
}