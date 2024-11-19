#include "execution.h"

void execute_command(ParsedCommand* parsed_cmd)
{
    if (parsed_cmd->is_internal)
    {
        if (parsed_cmd->is_background)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("Fork failed");
            }
            else if (pid == 0)
            {
                handle_internal_command(parsed_cmd);
                exit(EXIT_SUCCESS);
            }
            else
            {
                add_job(pid, parsed_cmd->command);
                printf("[Background] PID: %d\n", pid);
            }
        }
        else
        {
            handle_internal_command(parsed_cmd);
        }
    }
    else if (parsed_cmd->is_piped)
    {
        execute_piped_commands(parsed_cmd);
    }
    else
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
        }
        else if (pid == 0)
        {
            execvp(parsed_cmd->args[0], parsed_cmd->args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (parsed_cmd->is_background)
            {
                add_job(pid, parsed_cmd->command);
                printf("[Background] PID: %d\n", pid);
            }
            else
            {
                foreground_pid = pid;
                waitpid(pid, NULL, 0);
                foreground_pid = -1;
            }
        }
    }
}

void execute_piped_commands(ParsedCommand* parsed_cmd)
{
    int pipe_fd[MAX_PIPES - 1][2];
    for (int i = 0; i < parsed_cmd->num_pipes; i++)
    {
        if (pipe(pipe_fd[i]) == -1)
        {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i <= parsed_cmd->num_pipes; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (i > 0)
            {
                dup2(pipe_fd[i - 1][0], STDIN_FILENO);
            }
            if (i < parsed_cmd->num_pipes)
            {
                dup2(pipe_fd[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < parsed_cmd->num_pipes; j++)
            {
                close(pipe_fd[j][0]);
                close(pipe_fd[j][1]);
            }
            char* args[MAX_ARGS];
            char* arg = strtok(parsed_cmd->pipes[i], " ");
            int arg_count = 0;
            while (arg && arg_count < MAX_ARGS - 1)
            {
                args[arg_count++] = arg;
                arg = strtok(NULL, " ");
            }
            args[arg_count] = NULL;
            execvp(args[0], args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            if (i < parsed_cmd->num_pipes)
            {
                close(pipe_fd[i][1]);
            }
            if (i > 0)
            {
                close(pipe_fd[i - 1][0]);
            }
        }
    }
    for (int i = 0; i < parsed_cmd->num_pipes; i++)
    {
        close(pipe_fd[i][0]);
        close(pipe_fd[i][1]);
    }
}

void handle_internal_command(ParsedCommand* parsed_cmd)
{
    int original_stdout = -1;
    handle_file_redirection(parsed_cmd->output_file, &original_stdout);
    CommandHandler command_handlers[] = {{"cd", handle_cd},
                                         {"echo", handle_echo},
                                         {"clr", handle_clr},
                                         {"quit", handle_quit},
                                         {"set_interval", handle_set_interval},
                                         {"set_metrics", handle_set_metrics},
                                         {"start_monitor", handle_start_monitor},
                                         {"stop_monitor", handle_stop_monitor},
                                         {"status_monitor", handle_status_monitor},
                                         {"man", handle_man},
                                         {NULL, NULL}};
    for (int i = 0; command_handlers[i].command != NULL; i++)
    {
        if (strcmp(parsed_cmd->args[0], command_handlers[i].command) == 0)
        {
            command_handlers[i].handler(parsed_cmd);
            reset_file_redirection(original_stdout);
            return;
        }
    }
    fprintf(stderr, "Unknown internal command: %s\n", parsed_cmd->args[0]);
    reset_file_redirection(original_stdout);
}

void handle_file_redirection(const char* output_file, int* original_stdout)
{
    if (output_file)
    {
        FILE* file = fopen(output_file, "w");
        if (!file)
        {
            perror("Output file open failed");
            return;
        }
        *original_stdout = dup(STDOUT_FILENO);
        dup2(fileno(file), STDOUT_FILENO);
        fclose(file);
    }
}

void reset_file_redirection(int original_stdout)
{
    if (original_stdout != -1)
    {
        fflush(stdout);
        dup2(original_stdout, STDOUT_FILENO);
        close(original_stdout);
    }
}
