/**
 * @file utils.h
 * @brief Header file for utility functions used in shell operations.
 *
 * This header file declares utility functions that assist in the operations of the shell program.
 * These include functions for parsing user input, managing signal handling, cleaning up parsed
 * command structures, creating FIFOs for inter-process communication, and displaying the shell prompt.
 *
 * @date 09/10/2024
 * @author 1v6n
 */

#ifndef UTILS_H
#define UTILS_H

#include "global.h"

/**
 * @brief Parses user input into a structured command.
 *
 * @param input The raw input string from the user.
 * @param parsed_cmd Pointer to the ParsedCommand structure to store parsed details.
 */
void parse_input(char* input, ParsedCommand* parsed_cmd);

/**
 * @brief Checks if a given command is an internal shell command.
 *
 * @param command The command to check.
 * @return 1 if the command is internal, 0 otherwise.
 */
int is_internal_command(const char* command);

/**
 * @brief Cleans up memory allocated for a parsed command.
 *
 * @param parsed_cmd Pointer to the ParsedCommand structure to clean up.
 */
void cleanup_parsed_command(ParsedCommand* parsed_cmd);

/**
 * @brief Sets up signal handlers for the shell.
 */
void setup_signal_handlers(void);
/**
 * @brief Handles a signal received by the shell.
 *
 * @param sig The signal number received.
 */
void handle_signal(int sig);

/**
 * @brief Creates a FIFO for inter-process communication.
 *
 * @param path The path of the FIFO to create.
 * @param mode The file mode for the FIFO.
 * @return 0 on success, -1 on failure.
 */
int create_fifo(const char* path, mode_t mode);

/**
 * @brief Displays the shell prompt with the current working directory.
 */
void display_prompt(void);

/**
 * @brief Displays the start screen for the shell.
 */
void display_start_screen(void);

/**
 * @brief Cleans and checks the user input for special characters.
 *
 * @param str The input string to clean and check.
 * @return True if the input is clean, false otherwise.
 */
bool clean_and_check_input(char* str);

/**
 * @brief Searches for configuration files with a specific extension in a given directory and optionally reads their
 * content.
 *
 * This function recursively searches through the specified directory and its subdirectories for files with the given
 * extension. If the mode is set to MODE_FILES_AND_CONTENT, it will also read and display the content of the found
 * files.
 *
 * @param dir_path The path to the directory to search in.
 * @param file_extension The file extension to search for.
 * @param mode The mode of operation. If set to MODE_FILES_AND_CONTENT, the function will read and display the content
 * of the found files.
 *
 */
void search_config_files(const char* dir_path, const char* file_extension, int mode);

/**
 * @brief Reads a configuration file in JSON format and displays its content.
 *
 * This function opens a file specified by the given file path, reads its entire
 * content into a buffer, parses the JSON data, and prints the parsed JSON data.
 * If any error occurs during file operations, memory allocation, or JSON parsing,
 * appropriate error messages are displayed.
 *
 * @param file_path The path to the configuration file to be read.
 */
void read_and_display_config(const char* file_path);

/**
 * @brief Prints a cJSON item in a human-readable format with indentation.
 *
 * This function recursively prints a cJSON item and its children (if any) in a
 * human-readable format with indentation based on the depth level.
 *
 * @param item A pointer to the cJSON item to be printed. If the item is NULL,
 *             the function prints "NULL" and returns.
 * @param depth The current depth level for indentation. This determines the
 *              number of spaces used for indentation.
 */
void print_json_item(cJSON* item, int depth);

#endif // UTILS_H
