/**
 * @file main.c
 * @brief Entry point for the shell program.
 */
#include "execution.h"
#include "utils.h"

/**
 * @brief Main function for the shell program.
 *
 * This function sets up signal handlers, retrieves initial metrics, and initializes the shell environment.
 * It then enters an infinite loop to read user input, parse commands, execute them, and clean up as needed.
 *
 * @return 0 on successful execution.
 */
/**
 * @brief Main function for the shell program.
 *
 * This function sets up signal handlers, retrieves initial metrics, and initializes the shell environment.
 * It then either reads commands from a batch file if provided or enters an infinite loop to read user input,
 * parse commands, execute them, and clean up as needed.
 *
 * @return 0 on successful execution.
 */
int main(int argc, char* argv[])
{
    setup_signal_handlers();
    retrive_metrics(FIFO_PATH, MONITOR_PATH);
    initialize_metrics_from_status_file(METRICS_FILE);
    create_config_file(CONFIG_FILE, interval, metrics, num_metrics);
    display_start_screen();

    if (argc == 2)
    {
        FILE* file = fopen(argv[1], "r");
        if (!file)
        {
            perror("Failed to open batch file");
            return EXIT_FAILURE;
        }

        char line[INPUT_BUFFER_SIZE];
        while (fgets(line, sizeof(line), file))
        {
            if (!clean_and_check_input(line))
            {
                continue;
            }
            ParsedCommand parsed_cmd;
            parse_input(line, &parsed_cmd);
            execute_command(&parsed_cmd);
            cleanup_parsed_command(&parsed_cmd);
        }
        fclose(file);
        return EXIT_SUCCESS;
    }
    else
    {
        char input[INPUT_BUFFER_SIZE];
        while (1)
        {
            reap_completed_jobs();
            display_prompt();
            if (!fgets(input, sizeof(input), stdin))
                break;
            if (!clean_and_check_input(input))
            {
                continue;
            }
            ParsedCommand parsed_cmd;
            parse_input(input, &parsed_cmd);
            execute_command(&parsed_cmd);
            cleanup_parsed_command(&parsed_cmd);
        }
    }
    return 0;
}
