/**
 * @file execution.h
 * @brief Header file for command execution management in the shell.
 *
 * This header file declares functions responsible for executing and managing commands in the shell environment.
 * These functions include handling both internal and external commands, managing command pipelines, and ensuring proper
 * output redirection and restoration of the original environment state.
 *
 * @date 03/11/2024
 * @author 1v6n
 */

#ifndef EXECUTION_H
#define EXECUTION_H

#include "commands.h"

/**
 * @brief Executes a parsed command, handling internal, background, and external commands.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void execute_command(ParsedCommand* parsed_cmd);

/**
 * @brief Executes a series of commands connected by pipes.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void execute_piped_commands(ParsedCommand* parsed_cmd);

/**
 * @brief Handles the execution of internal shell commands.
 *
 * @param parsed_cmd Pointer to the parsed command structure.
 */
void handle_internal_command(ParsedCommand* parsed_cmd);

/**
 * @brief Redirects output to a specified file and stores the original stdout.
 *
 * @param output_file Path to the output file.
 * @param original_stdout Pointer to store the original stdout file descriptor.
 */
void handle_file_redirection(const char* output_file, int* original_stdout);

/**
 * @brief Resets the file redirection to the original stdout.
 *
 * @param original_stdout The original stdout file descriptor.
 */
void reset_file_redirection(int original_stdout);

#endif // EXECUTION_H
