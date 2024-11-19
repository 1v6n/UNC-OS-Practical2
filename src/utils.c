#include "utils.h"

void setup_signal_handlers(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void handle_signal(int sig)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, sig);
    }
}

void parse_input(char* input, ParsedCommand* parsed_cmd)
{
    memset(parsed_cmd, 0, sizeof(ParsedCommand));
    parsed_cmd->is_background = (input[strlen(input) - 1] == '&');
    if (parsed_cmd->is_background)
    {
        input[strlen(input) - 1] = '\0';
    }
    char* input_redirect = strchr(input, '<');
    char* output_redirect = strchr(input, '>');
    if (input_redirect)
    {
        *input_redirect = '\0';
        parsed_cmd->input_file = strtok(input_redirect + 1, " \t");
    }
    if (output_redirect)
    {
        *output_redirect = '\0';
        parsed_cmd->output_file = strtok(output_redirect + 1, " \t");
    }
    char* token = strtok(input, "|");
    int num_commands = 0;
    while (token && num_commands < MAX_PIPES)
    {
        parsed_cmd->pipes[num_commands++] = strdup(token);
        token = strtok(NULL, "|");
    }
    parsed_cmd->is_piped = (num_commands > 1);
    parsed_cmd->num_pipes = num_commands - 1;
    if (parsed_cmd->is_piped)
    {
        return;
    }
    parsed_cmd->command = parsed_cmd->pipes[0];
    int i = 0;
    token = strtok(parsed_cmd->pipes[0], " ");
    while (token && i < MAX_ARGS - 1)
    {
        parsed_cmd->args[i++] = token;
        token = strtok(NULL, " ");
    }
    parsed_cmd->args[i] = NULL;
    parsed_cmd->is_internal = is_internal_command(parsed_cmd->args[0]);
}

int is_internal_command(const char* command)
{
    const char* internal_commands[] = {"cd",          "echo",          "clr",          "quit",           "set_interval",
                                       "set_metrics", "start_monitor", "stop_monitor", "status_monitor", "man",
                                       NULL};

    for (int i = 0; internal_commands[i] != NULL; i++)
    {
        if (strcmp(command, internal_commands[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int create_fifo(const char* path, mode_t mode)
{
    if (mkfifo(path, mode) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo failed");
            return -1;
        }
    }
    return 0;
}

void display_prompt(void)
{
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf(COLOR_CWD "%s" COLOR_PROMPT "$ " COLOR_RESET, cwd);
    }
    else
    {
        perror("getcwd() error");
    }
    fflush(stdout);
}

void cleanup_parsed_command(ParsedCommand* parsed_cmd)
{
    for (int i = 0; i < parsed_cmd->num_pipes + 1; i++)
    {
        free(parsed_cmd->pipes[i]);
    }
    memset(parsed_cmd, 0, sizeof(ParsedCommand));
}

void display_start_screen(void)
{
    printf("\033[1;32m[READY]\033[0m Shell environment initialized successfully.\n");
    printf("\033[1;36m----------------------------------------------\033[0m\n");
    printf(" \033[1;33m//== Survivor Operations Interface v1.0 ==\\\\\033[0m\n");
    printf("\033[1;36m----------------------------------------------\033[0m\n\n");

    printf("\033[1;37mWelcome, Operator.\033[0m\n");
    printf("The system is now \033[1;32moperational\033[0m and standing by for your commands.\n\n");

    printf("For assistance, type '\033[1;34mman\033[0m' to access the command guide.\n");
    printf("\033[1;31mCaution:\033[0m Ensure you are prepared for your mission before proceeding.\n\n");
}

bool clean_and_check_input(char* str)
{
    str[strcspn(str, "\n")] = '\0'; // Remove the newline character
    return strlen(str) != 0;        // Return true if the string is not empty
}
