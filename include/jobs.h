/**
 * @file jobs.h
 * @brief Header file for job management in the shell.
 *
 * This header file declares functions used to manage background jobs within the shell environment.
 * It includes functions for adding new jobs, reaping completed jobs, and cleaning up active jobs
 * when exiting the shell. These functions help maintain process control and job tracking, essential
 * for managing concurrent tasks in the shell.
 *
 * @date 03/11/2024
 * @author 1v6n
 */

#ifndef JOBS_H
#define JOBS_H

#include "global.h"

/**
 * @brief Adds a new job to the job list.
 *
 * @param pid The process ID of the new job.
 * @param command The command string associated with the job.
 */
void add_job(pid_t pid, char* command);

/**
 * @brief Reaps completed background jobs and removes them from the job list.
 */
void reap_completed_jobs(void);

/**
 * @brief Cleans up all active jobs by terminating them and exits the program.
 */
void cleanup_and_exit(void);

#endif // JOBS_H
