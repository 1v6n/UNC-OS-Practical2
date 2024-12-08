/**
 * @file global.h
 * @brief Header file declaring global variables, constants, and data structures for the shell.
 *
 * This header file includes the declaration of global variables, constants, and structures
 * used throughout the shell program. It provides essential definitions for job management,
 * command parsing, and configuration handling, enabling various shell functionalities such
 * as process tracking, input parsing, and interaction with system metrics.
 *
 * @date 03/11/2024
 * @author 1v6n
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <cjson/cJSON.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 64                               /**< Maximum number of arguments for a command. */
#define MAX_PIPES 10                              /**< Maximum number of pipes supported. */
#define INPUT_BUFFER_SIZE 1024                    /**< Size of the input buffer. */
#define MAX_PATH 1024                             /**< Maximum path length. */
#define BUFFER_SIZE 256                           /**< Size of a general buffer. */
#define MAX_JOBS 100                              /**< Maximum number of concurrent jobs. */
#define METRICS_FILE "/tmp/monitor_metrics"       /**< Path to the file where metrics are stored. */
#define CONFIG_FILE "config.json"                 /**< Path to the configuration file. */
#define DEFAULT_INTERVAL 5                        /**< Default interval for monitoring. */
#define FIFO_PATH "/tmp/monitor_fifo"             /**< Path to the FIFO used for inter-process communication. */
#define MONITOR_PATH "./submodule/so_i_24_1v6n_2" /**< Path to the monitor executable. */
#define STATUS_FILE "/tmp/monitor_status"         /**< Path to the status file. */
#define _POSIX_C_SOURCE 200809L                   /**< Macro for enabling POSIX.1-2008 compatibility. */
#define COLOR_CWD "\x1b[34m"                      /**< Blue color for the cwd*/
#define COLOR_PROMPT "\x1b[32m"                   /**< Green color for the $ symbol*/
#define COLOR_USER "\033[1;32m"                   /**< Green color for the user name*/
#define COLOR_RESET "\x1b[0m"                     /**< Reset to default color*/
#define MODE_DIRECTORIES_ONLY 1                   /**< Mode for searching directories only. */
#define MODE_FILES_AND_CONTENT 2                  /**< Mode for searching files and displaying content. */
#define PATH_MAX 4096                             /**< Maximum path length. */

/**
 * @struct Job
 * @brief Structure representing a job in the shell.
 */
typedef struct
{
    int job_id;                      /**< Job identifier. */
    pid_t pid;                       /**< Process ID of the job. */
    char command[INPUT_BUFFER_SIZE]; /**< Command string associated with the job. */
} Job;

/**
 * @brief Array of jobs representing active jobs.
 */
extern Job jobs[MAX_JOBS]; /**< Array representing active jobs. */

/**
 * @brief Number of active jobs.
 */
extern int job_count; /**< Count of active jobs. */

/**
 * @struct ParsedCommand
 * @brief Structure representing a parsed command.
 */
typedef struct
{
    char* command;          /**< Base command. */
    char* args[MAX_ARGS];   /**< Arguments list. */
    char* input_file;       /**< Input redirection file, if any. */
    char* output_file;      /**< Output redirection file, if any. */
    int is_background;      /**< Background execution flag. */
    int is_piped;           /**< Piped command flag. */
    int is_internal;        /**< Internal command flag. */
    int num_pipes;          /**< Number of pipes. */
    char* pipes[MAX_PIPES]; /**< Array of piped commands. */
} ParsedCommand;

/**
 * @struct CommandHandler
 * @brief Structure mapping a command to its handler function.
 */
typedef struct
{
    const char* command;             /**< Command name. */
    void (*handler)(ParsedCommand*); /**< Handler function pointer. */
} CommandHandler;

extern const char* fifo_path;    /**< Path to the FIFO for inter-process communication. */
extern const char* monitor_path; /**< Path to the monitor executable. */
extern cJSON* root;              /**< Root JSON object for configuration. */
extern int interval;             /**< Interval for monitoring updates. */
extern char* metrics[MAX_ARGS];  /**< Array of metric names. */
extern size_t num_metrics;       /**< Number of selected metrics. */
extern pid_t foreground_pid;     /**< Process ID of the foreground process. */
extern Job jobs[MAX_JOBS];       /**< Array representing the active jobs. */
extern int job_count;            /**< Count of active jobs. */

#endif // GLOBALS_H
