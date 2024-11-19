#include "global.h"

const char* fifo_path = FIFO_PATH;
const char* monitor_path = MONITOR_PATH;
cJSON* root = NULL;
int interval = DEFAULT_INTERVAL;
char* metrics[MAX_ARGS];
size_t num_metrics = 0;
pid_t foreground_pid = -1;
Job jobs[MAX_JOBS];
int job_count = 0;
