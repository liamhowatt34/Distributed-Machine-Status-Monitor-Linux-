#pragma once
#include <signal.h>
#include "../../Common/inc/util.h"
#include "../../Common/inc/log.h"
#include "../../DR/inc/master_list.h"

#define MAX_TRIES 100
#define ZERO 0
#define TWENTY 20

#define ZERO_DO_NOTHING 0
#define ONE_KILL_DC1 1
#define TWO_KILL_DC3 2
#define THREE_KILL_DC2 3
#define FOUR_KILL_DC1 4
#define FIVE_KILL_DC3 5
#define SIX_KILL_DC2 6
#define SEVEN_KILL_DC4 7
#define EIGHT_DO_NOTHING 8
#define NINE_KILL_DC5 9
#define TEN_DELETE_MSG_Q 10
#define ELEVEN_KILL_DC1 11
#define TWELVE_KILL_DC6 12
#define THIRTEEN_KILL_DC2 13
#define FOURTEEN_KILL_DC7 14
#define FIFTEEN_KILL_DC3 15
#define SIXTEEN_KILL_DC8 16
#define SEVENTEEN_DELETE_MSG_Q 17
#define EIGHTEEN_KILL_DC9 18
#define NINETEEN_DO_NOTHING 19
#define TWENTY_KILL_DC10 20

#define DC1 1
#define DC2 2
#define DC3 3
#define DC4 4
#define DC5 5
#define DC6 6
#define DC7 7
#define DC8 8
#define DC9 9
#define DC10 10


void log_dx_queue_doesnt_exist(FILE* log_file);
int select_random_wod_item();
int get_pid_of_dc(MasterList* master_list, int dc_number);
bool does_dc_exist(MasterList* master_list, int dc_number);
void log_wod_task(FILE* log_file, int action_number, int dc_number, int target_pid);
int execute_wod_task(int wod_selection, MasterList* master_list, int q_id, FILE* log_file);