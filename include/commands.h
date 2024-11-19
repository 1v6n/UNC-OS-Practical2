/**
 * @file commands.h
 * @brief Header file for handling shell commands.
 *
 * This header file declares functions used to handle various shell commands such as changing directories,
 * echoing input, clearing the screen, managing intervals and metrics, and interacting with the monitoring
 * processes. These functions ensure that the shell can execute and manage both internal and external
 * commands effectively.
 *
 * @date 03/11/2024
 * @author 1v6n
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "global.h"
#include "jobs.h"
#include "utils.h"

/**
 * @brief Handles the 'cd' command, changing the current directory.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_cd(ParsedCommand* parsed_cmd);

/**
 * @brief Handles the 'echo' command, printing text or reading from a file.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_echo(ParsedCommand* parsed_cmd);

/**
 * @brief Clears the terminal screen.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_clr(ParsedCommand* parsed_cmd);

/**
 * @brief Handles the 'quit' command, cleaning up and exiting the program.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_quit(ParsedCommand* parsed_cmd);

/**
 * @brief Sets the interval for the monitor’s update frequency.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_set_interval(ParsedCommand* parsed_cmd);

/**
 * @brief Selects metrics to be monitored.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_set_metrics(ParsedCommand* parsed_cmd);

/**
 * @brief Starts the monitoring process using the specified configuration.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_start_monitor(ParsedCommand* parsed_cmd);

/**
 * @brief Displays manual information about available commands.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_man(ParsedCommand* parsed_cmd);

/**
 * @brief Stops the currently running monitoring process.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_stop_monitor(ParsedCommand* parsed_cmd);

/**
 * @brief Retrieves metrics from a FIFO and interacts with the monitoring process.
 *
 * @param fifo_path Path to the FIFO file.
 * @param monitor_path Path to the monitor executable.
 */
void retrive_metrics(const char* fifo_path, const char* monitor_path);

/**
 * @brief Creates a configuration file with specified interval and metrics.
 *
 * @param config_file Path to the configuration file.
 * @param interval The update interval in seconds.
 * @param metrics Array of metric names.
 * @param num_metrics The number of metrics.
 */
void create_config_file(const char* config_file, int interval, char** metrics, size_t num_metrics);

/**
 * @brief Displays the status of the current monitoring process.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_status_monitor(ParsedCommand* parsed_cmd);

/**
 * @brief Initializes metrics from the status file.
 *
 * @param status_file Path to the status file.
 */
void initialize_metrics_from_status_file(const char* status_file);

/**
 * @brief Displays a mapping of available metrics and their IDs.
 */
void display_metrics_mapping(void);

/**
 * @brief Frees memory allocated for metrics.
 */
void free_metrics(void);

#endif // COMMANDS_H
