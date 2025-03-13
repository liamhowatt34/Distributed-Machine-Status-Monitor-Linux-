//
// Project: Distributed Machine Status Monitor
// File: main.c
// Programmer: Liam Howatt 
// Description: This file is the main starting point for the
//              DX application.
//
#include "../inc/dx.h"

int main(void) {
    // Seed for random num generator
    srand(time(NULL));

    // Open log file
    FILE* log_file = NULL;
    log_file = fopen("tmp/dataCorruptor.log", "a");
    if (log_file == NULL) {
        log_error(log_file, strerror(errno));
        return -1;
    }

    // Get the shared memory key
    key_t shared_memory_key = ftok(".", 16535);
    if (shared_memory_key == -1) {
        log_error(log_file, strerror(errno));
        fclose(log_file);
        return -1;
    }

    // Get access to the shared memory
    int shared_mem_id;
    int count = 0;
    while (count <= MAX_TRIES) {
        shared_mem_id = shmget(shared_memory_key, sizeof(MasterList), 0);
        if (shared_mem_id == -1) {
            sleep(10);
        } else {
            break;
        }
        count++;
    }

    if (count > MAX_TRIES) {
        log_error(log_file, "Max tries (100) reached while trying to access shared memory\n");
        fclose(log_file);
        return -1;
    }

    // Get the key for the message queue
    key_t message_key = ftok("Common/inc/util.h", DC_ID);
    if (message_key == -1) {
        log_error(log_file, strerror(errno));
        fclose(log_file);
        return -1;
    }

    // Access shared memory
    MasterList* master_list = NULL;
    master_list = (MasterList*)shmat(shared_mem_id, NULL, 0);
    if (master_list == (void*)-1) {
        log_error(log_file, strerror(errno));
        fclose(log_file);
        return -1;
    }

    // Main processing loop
    int interval = 0;
    while (1) {
        // Sleep for a random time b/w 10 and 30 seconds
        interval = random_time();
        sleep(interval);

        // Check if the queue exists, if not, exit
        int q_id;
        if ((q_id = msgget(message_key,0)) == -1) {
            log_dx_queue_doesnt_exist(log_file);
            fclose(log_file);
            return 0;
        }

        // Select a random task from the wheel of desctruction
        int wod_selection = select_random_wod_item();

        // Execute selected task
        int result_code = execute_wod_task(wod_selection, master_list, q_id, log_file);
        if (result_code == -1) {
            log_error(log_file, strerror(errno));
            fclose(log_file);
            return -1;
        }
    }

    fclose(log_file);
    return 0;
}