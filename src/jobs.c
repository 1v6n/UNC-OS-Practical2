/**
 * @file jobs.h
 * @brief Implementation of job management functions.
 */
#include "jobs.h"

void add_job(pid_t pid, char* command)
{
    if (job_count < MAX_JOBS)
    {
        jobs[job_count].job_id = job_count + 1;
        jobs[job_count].pid = pid;
        strncpy(jobs[job_count].command, command, INPUT_BUFFER_SIZE - 1);
        jobs[job_count].command[INPUT_BUFFER_SIZE - 1] = '\0';
        job_count++;
    }
    else
    {
        printf("Job limit reached, unable to track new background job.\n");
    }
}

void reap_completed_jobs(void)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i < job_count; i++)
        {
            if (jobs[i].pid == pid)
            {
                printf("[%d]+ Done %s\n", jobs[i].job_id, jobs[i].command);
                for (int j = i; j < job_count - 1; j++)
                {
                    jobs[j] = jobs[j + 1];
                }
                job_count--;
                break;
            }
        }
    }
}

void cleanup_and_exit(void)
{
    for (int i = 0; i < job_count; i++)
    {
        kill(jobs[i].pid, SIGTERM);
    }
    printf("\n\033[1;31m============================================\033[0m\n");
    printf("\033[1;31m|          Shutting down processes          |\033[0m\n");
    printf("\033[1;31m|        All active tasks terminated        |\033[0m\n");
    printf("\033[1;31m============================================\033[0m\n");
    printf("\033[1;32m\nThank you for using the shell. Goodbye!\033[0m\n\n");
    exit(0);
}
