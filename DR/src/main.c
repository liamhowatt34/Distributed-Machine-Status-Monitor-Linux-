//
// Project: Distributed Machine Status Monitor
// File: main.c
// Programmer: Liam Howatt
// Description: This file is the main starting point for the
//              DR application.
//
#include "../inc/master_list.h"

int main(void) {
    // Open log file
    FILE* log_file = NULL;
    log_file = fopen("tmp/dataMonitor.log", "a");
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

    // Generate key for shared memory
    key_t shared_memory_key = ftok(".", 16535);
    if (shared_memory_key == -1) {
        log_error(log_file, strerror(errno));
        fclose(log_file);
        return -1;
    }

    // Allocate message queue if it does not exists yet
    int q_id;
    q_id = msgget(message_key, 0);
    if (q_id == -1) {
        // Create message queue
        q_id = msgget(message_key, IPC_CREAT | 0660);
        if (q_id == -1) {
            log_error(log_file, strerror(errno));
            fclose(log_file);
            return -1;
        }
    }

    // Allocate shared memory if it does not exist yet
    int shared_mem_id;
    shared_mem_id = shmget(shared_memory_key, sizeof(MasterList), 0);
    if (shared_mem_id == -1) {
        // Create shared memory
        shared_mem_id = shmget(shared_memory_key, sizeof(MasterList), (IPC_CREAT | 0660));
        if (shared_mem_id == -1) {
            log_error(log_file, strerror(errno));

            // Clean up queue
            int free_q_return_code = msgctl(q_id, IPC_RMID, NULL);
            if (free_q_return_code == -1) {
                log_error(log_file, strerror(errno));
            }

            fclose(log_file);
            return -1;
        }
    }

    // Access shared memory
    MasterList* master_list = NULL;
    master_list = (MasterList*)shmat(shared_mem_id, NULL, 0);
    if (master_list == (void*)-1) {
        log_error(log_file, strerror(errno));

        // Free queue and shared memory
        int free_q_return_code = msgctl(q_id, IPC_RMID, NULL);
        if (free_q_return_code == -1) {
            log_error(log_file, strerror(errno));
        }
        int free_shmem_return_code = shmctl(shared_mem_id, IPC_RMID, 0);
        if (free_shmem_return_code == -1) {
            log_error(log_file, strerror(errno));
        }

        fclose(log_file);
        return -1;
    }

    // Initialize master list
    master_list->q_id = q_id;
    master_list->num_of_dcs = 0;

    sleep(15);

    // Main processing loop
    while (1) {
        // Reset log status str at the beginning of each iteration
        memset(log_status_str, 0, sizeof(log_status_str));

        // Make sure queue still exists
        q_id = msgget(message_key, 0);
        if (q_id == -1) {
            log_error(log_file, "Queue no longer exists, DR EXITING\n");

            // Free shared memory
            int free_shmem_return_code = shmctl(shared_mem_id, IPC_RMID, 0);
            if (free_shmem_return_code == -1) {
                log_error(log_file, strerror(errno));
            }

            fclose(log_file);
            break;
        }

        Message message;
        size_t message_size = sizeof(Message) - sizeof(long);

        // Receive incoming messages
        int return_code = msgrcv(q_id, &message, message_size, 0, 0);
        if (return_code == -1) {
            log_error(log_file, strerror(errno));

            // Free queue and shared memory
            int free_q_return_code = msgctl(q_id, IPC_RMID, NULL);
            if (free_q_return_code == -1) {
                log_error(log_file, strerror(errno));
            }
            int free_shmem_return_code = shmctl(shared_mem_id, IPC_RMID, 0);
            if (free_shmem_return_code == -1) {
                log_error(log_file, strerror(errno));
            }

            fclose(log_file);
            return -1;  
        }

        // Check if machine has been seen before and handle received message
        int seen_machine = check_master_list_for_pid(master_list, &message);
        master_list = handle_message(master_list, &message, seen_machine, log_status_str, log_file);

        // Check and account if any dcs time has expired
        master_list = check_dcs_time(master_list, log_file);

        // Check if offline message has been received
        master_list = check_if_offline_msg_received(master_list, &message);

        // Check if num of dcs == 0
        bool still_dcs_running = dcs_exist(master_list);
        if (!still_dcs_running) {
            log_all_dcs_offline(log_file);
            
            // Free queue and shared memory
            int free_q_return_code = msgctl(q_id, IPC_RMID, NULL);
            if (free_q_return_code == -1) {
                log_error(log_file, strerror(errno));
                fclose(log_file);
                return -1;
            }
            int free_shmem_return_code = shmctl(shared_mem_id, IPC_RMID, 0);
            if (free_shmem_return_code == -1) {
                log_error(log_file, strerror(errno));
                fclose(log_file);
                return -1;
            }

            fclose(log_file);
            return 0;
        }

        usleep(1500000); // 1.5 seconds
    }
}
