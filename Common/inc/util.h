#pragma once
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#define DC_ID 1

#define EVERYTHING_OKAY 0
#define HYDRAULIC_PRESSURE_FAILURE 1
#define SAFETY_BUTTON_FAILURE 2
#define NO_RAW_MATERIAL 3
#define TEMP_OUT_OF_RANGE 4
#define OPERATOR_ERROR 5
#define MACHINE_OFFLINE 6

#define MAX_STATUS_LEN 100

typedef struct {
    long message_type;
    pid_t machine_pid;
    int status;
} Message;