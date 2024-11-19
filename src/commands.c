#include "commands.h"

void handle_cd(ParsedCommand* parsed_cmd)
{
    if (chdir(parsed_cmd->args[1] ? parsed_cmd->args[1] : getenv("HOME")) != 0)
    {
        perror("cd failed");
    }
}

void handle_echo(ParsedCommand* parsed_cmd)
{
    if (parsed_cmd->input_file)
    {
        FILE* file = fopen(parsed_cmd->input_file, "r");
        if (!file)
        {
            perror("Input file open failed");
            return;
        }
        char buffer[INPUT_BUFFER_SIZE];
        while (fgets(buffer, sizeof(buffer), file))
        {
            printf("%s", buffer);
        }
        fclose(file);
    }
    else
    {
        for (int i = 1; parsed_cmd->args[i]; i++)
        {
            printf("%s ", parsed_cmd->args[i]);
        }
        printf("\n");
    }
}

void handle_clr(ParsedCommand* parsed_cmd)
{
    printf("\033[H\033[J");
}

void handle_quit(ParsedCommand* parsed_cmd)
{
    free_metrics();
    cleanup_and_exit();
}

void handle_set_interval(ParsedCommand* parsed_cmd)
{
    if (parsed_cmd->args[1])
    {
        interval = atoi(parsed_cmd->args[1]);
        printf("Interval set to %d seconds\n", interval);
        create_config_file(CONFIG_FILE, interval, metrics, num_metrics);
    }
    else
    {
        fprintf(stderr, "\nUsage:\n");
        fprintf(stderr, "  set_interval <seconds>\n\n");
        fprintf(stderr, "Description:\n");
        fprintf(stderr, "  Set the interval (in seconds) for the monitor's update frequency.\n\n");
    }
}

void handle_set_metrics(ParsedCommand* parsed_cmd)
{
    if (parsed_cmd->args[1] == NULL)
    {
        display_metrics_mapping();
        fprintf(stderr, "\nUsage:\n");
        fprintf(stderr, "  set_metrics <metric_number1> <metric_number2> ...\n\n");
        fprintf(stderr, "Description:\n");
        fprintf(stderr, "  Select one or more metrics by their respective numbers.\n");
        return;
    }
    num_metrics = 0;
    for (int i = 1; parsed_cmd->args[i] && num_metrics < MAX_ARGS; i++)
    {
        int metric_number = atoi(parsed_cmd->args[i]);
        if (metric_number > 0 && metric_number <= MAX_ARGS)
        {
            const char* metric_name = metrics[metric_number - 1]; // Adjust for 0-based index
            printf("Selected metric number %d corresponds to metric name: %s\n", metric_number, metric_name);
            if (metric_name)
            {
                metrics[num_metrics] = (char*)metric_name;
                num_metrics++;
            }
            else
            {
                fprintf(stderr, "Invalid metric name for metric number: %d\n", metric_number);
            }
        }
        else
        {
            fprintf(stderr, "Invalid metric number: %d\n", metric_number);
        }
    }
    create_config_file(CONFIG_FILE, interval, metrics, num_metrics);
}

void handle_start_monitor(ParsedCommand* parsed_cmd)
{
    if (root == NULL)
    {
        fprintf(stderr, "Configuration not loaded.\n");
        return;
    }
    cJSON* metrics_array = cJSON_GetObjectItem(root, "metrics");
    if (!cJSON_IsArray(metrics_array))
    {
        fprintf(stderr, "Metrics is not an array\n");
        return;
    }
    char metrics_string[INPUT_BUFFER_SIZE] = "";
    cJSON* metric;
    cJSON_ArrayForEach(metric, metrics_array)
    {
        if (cJSON_IsString(metric))
        {
            strcat(metrics_string, metric->valuestring);
            strcat(metrics_string, ", ");
        }
    }
    size_t len = strlen(metrics_string);
    if (len > 2 && metrics_string[len - 2] == ',' && metrics_string[len - 1] == ' ')
    {
        metrics_string[len - 2] = '\0';
    }
    if (create_fifo(FIFO_PATH, 0666) == -1)
    {
        return;
    }

    printf("\n\033[1;36m=========================================\033[0m\n");
    printf("\033[1;35m|         Starting the Monitor...        |\033[0m\n");
    printf("\033[1;36m=========================================\033[0m\n");

    pid_t fifo_pid = fork();
    if (fifo_pid < 0)
    {
        perror("fork failed");
        return;
    }
    else if (fifo_pid == 0)
    {
        FILE* fifo_file = fopen(FIFO_PATH, "w");
        if (fifo_file == NULL)
        {
            perror("fopen fifo for writing failed");
            exit(EXIT_FAILURE);
        }
        if (fwrite(metrics_string, sizeof(char), strlen(metrics_string), fifo_file) != strlen(metrics_string))
        {
            perror("fwrite to fifo failed");
        }
        fclose(fifo_file);
        exit(EXIT_SUCCESS);
    }
    execlp(MONITOR_PATH, MONITOR_PATH, NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
    waitpid(fifo_pid, NULL, 0);
}

void handle_man(ParsedCommand* parsed_cmd)
{
    printf("\033[1;36m\n================== MANUAL ==================\033[0m\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mcd\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Change the current working directory.\n");
    printf("\033[1;33mUSAGE:\033[0m       cd <directory_path>\n");
    printf("\033[1;33mEXAMPLE:\033[0m     cd /home/user\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mecho\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Display a line of text or a string.\n");
    printf("\033[1;33mUSAGE:\033[0m       echo <text>\n");
    printf("\033[1;33mEXAMPLE:\033[0m     echo Hello, World!\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mclr\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Clear the screen.\n");
    printf("\033[1;33mUSAGE:\033[0m       clr\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mquit\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Exit the shell.\n");
    printf("\033[1;33mUSAGE:\033[0m       quit\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mset_interval\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Set the interval (in seconds) for the monitor's update frequency.\n");
    printf("\033[1;33mUSAGE:\033[0m       set_interval <seconds>\n");
    printf("\033[1;33mEXAMPLE:\033[0m     set_interval 10\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mset_metrics\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Set the metrics to be monitored.\n");
    printf("\033[1;33mUSAGE:\033[0m       set_metrics <metric_number1> <metric_number2> ...\n");
    printf("\033[1;33mEXAMPLE:\033[0m     set_metrics 1 3 5\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mstart_monitor\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Start the monitoring process in the background.\n");
    printf("\033[1;33mUSAGE:\033[0m       start_monitor\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mstop_monitor\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Stop the currently running monitor process.\n");
    printf("\033[1;33mUSAGE:\033[0m       stop_monitor\n\n");

    printf("\033[1;33mCOMMAND:\033[0m   \033[1;37mstatus_monitor\033[0m\n");
    printf("\033[1;33mDESCRIPTION:\033[0m Display the status of the monitoring process.\n");
    printf("\033[1;33mUSAGE:\033[0m       status_monitor\n\n");

    printf("\033[1;36m============================================\033[0m\n\n");
}

void handle_stop_monitor(ParsedCommand* parsed_cmd)
{
    for (int i = 0; i < job_count; i++)
    {
        if (strcmp(jobs[i].command, "start_monitor") == 0)
        {
            pid_t monitor_pid = jobs[i].pid;
            if (kill(monitor_pid, 2) == 0)
            {
                printf("\n\033[1;31m=========================================\033[0m\n");
                printf("\033[1;31m|      Monitor Process Terminated       |\033[0m\n");
                printf("\033[1;31m=========================================\033[0m\n");
                printf("\033[1;33mMonitor with PID %d has been successfully stopped.\033[0m\n\n", monitor_pid);
                for (int j = i; j < job_count - 1; j++)
                {
                    jobs[j] = jobs[j + 1];
                }
                job_count--;
                return;
            }
            else
            {
                perror("Failed to stop monitor");
                return;
            }
        }
    }
    printf("No active monitor found to stop.\n");
}

void retrive_metrics(const char* fifo_path, const char* monitor_path)
{
    if (unlink(fifo_path) == -1 && errno != ENOENT)
    {
        perror("unlink failed");
        return;
    }
    if (create_fifo(FIFO_PATH, 0666) == -1)
    {
        return;
    }
    pid_t writer_pid = fork();
    if (writer_pid < 0)
    {
        perror("fork failed");
        return;
    }
    else if (writer_pid == 0)
    {
        FILE* fifo_file = fopen(fifo_path, "w");
        if (fifo_file == NULL)
        {
            perror("fopen fifo for writing failed");
            exit(EXIT_FAILURE);
        }
        if (fwrite("1", sizeof(char), 1, fifo_file) != 1)
        {
            perror("fwrite to fifo failed");
            fclose(fifo_file);
            exit(EXIT_FAILURE);
        }
        fclose(fifo_file);
        exit(EXIT_SUCCESS);
    }
    pid_t monitor_pid = fork();
    if (monitor_pid < 0)
    {
        perror("fork failed");
        return;
    }
    else if (monitor_pid == 0)
    {
        execl(monitor_path, monitor_path, (char*)NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(writer_pid, NULL, 0);
        waitpid(monitor_pid, NULL, 0);
    }
}

void create_config_file(const char* config_file, int interval, char** metrics, size_t num_metrics)
{
    if (root)
    {
        cJSON_Delete(root);
    }
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "interval", interval);
    cJSON* metrics_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "metrics", metrics_array);
    for (size_t i = 0; i < num_metrics; i++)
    {
        cJSON_AddItemToArray(metrics_array, cJSON_CreateString(metrics[i]));
    }
    char* json_string = cJSON_Print(root);
    if (json_string == NULL)
    {
        fprintf(stderr, "Failed to print JSON\n");
        return;
    }
    FILE* config_file_fp = fopen(config_file, "w");
    if (config_file_fp == NULL)
    {
        perror("fopen");
        free(json_string);
        return;
    }
    fprintf(config_file_fp, "%s\n", json_string);
    fclose(config_file_fp);
    free(json_string);
}

void handle_status_monitor(ParsedCommand* parsed_cmd)
{
    FILE* file = fopen(STATUS_FILE, "r");
    if (!file)
    {
        perror("\033[1;31mFailed to open status file\033[0m");
        return;
    }

    printf("\n\033[1;34m=========================================\033[0m\n");
    printf("\033[1;34m|          Monitor Status Report        |\033[0m\n");
    printf("\033[1;34m=========================================\033[0m\n\n");

    char buffer[INPUT_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("\033[1;37m%s\033[0m", buffer);
    }
    printf("\n\033[1;34m=========================================\033[0m\n");
    fclose(file);
}

void initialize_metrics_from_status_file(const char* status_file)
{
    FILE* file = fopen(status_file, "r");
    if (file == NULL)
    {
        perror("fopen");
        return;
    }
    char line[BUFFER_SIZE];
    num_metrics = 0;
    while (fgets(line, sizeof(line), file) && num_metrics < MAX_ARGS)
    {
        char* metric_name = strchr(line, ':');
        if (metric_name)
        {
            metric_name += 2;
            metric_name[strcspn(metric_name, "\n")] = '\0';
            metrics[num_metrics] = strdup(metric_name);
            if (metrics[num_metrics] == NULL)
            {
                perror("strdup failed");
                fclose(file);
                return;
            }
            num_metrics++;
        }
    }

    fclose(file);
}

void display_metrics_mapping(void)
{
    printf("\n==================== Available Metrics ====================\n");
    printf("  %-5s | %s\n", "ID", "Metric Name");
    printf("----------------------------------------------------------\n");
    for (size_t i = 0; i < num_metrics; i++)
    {
        printf("  %-5zu | %s\n", i + 1, metrics[i]);
    }
    printf("==========================================================\n\n");
}

void free_metrics(void)
{
    for (size_t i = 0; i < num_metrics; i++)
    {
        free(metrics[i]);
    }
    num_metrics = 0;
}
