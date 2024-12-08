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
    const char* internal_commands[] = {
        "cd",           "echo",           "clr", "quit",         "set_interval",   "set_metrics", "start_monitor",
        "stop_monitor", "status_monitor", "man", "list_configs", "search_configs", NULL};

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
    char* user = getenv("USER");
    if (user == NULL)
    {
        user = "unknown";
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf(COLOR_USER "%s@%s:" COLOR_CWD "%s" COLOR_PROMPT "$ " COLOR_RESET, user, user, cwd);
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
    str[strcspn(str, "\n")] = '\0';
    return strlen(str) != 0;
}

void search_config_files(const char* dir_path, const char* file_extension, int mode)
{
    DIR* dir = opendir(dir_path);
    if (!dir)
    {
        fprintf(stderr, "Failed to open directory: %s\n", dir_path);
        perror("opendir");
        return;
    }
    if (mode == MODE_FILES_AND_CONTENT)
    {
        printf("Searching for %s files in %s\n", file_extension, dir_path);
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char entry_path[PATH_MAX];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dir_path, entry->d_name);
        struct stat sb;
        if (stat(entry_path, &sb) == 0)
        {
            if (S_ISDIR(sb.st_mode))
            {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                {

                    search_config_files(entry_path, file_extension, mode);
                }
            }
            else if (S_ISREG(sb.st_mode))
            {
                if (mode == MODE_FILES_AND_CONTENT)
                {
                    printf("Checking file: %s\n", entry_path);
                }
                size_t len = strlen(entry->d_name);
                size_t ext_len = strlen(file_extension);
                if (len > ext_len && strcmp(entry->d_name + len - ext_len, file_extension) == 0)
                {
                    printf("Directory: %s\n", entry_path);
                    if (mode == MODE_FILES_AND_CONTENT)
                    {
                        read_and_display_config(entry_path);
                    }
                }
            }
        }
        else
        {
            perror("stat failed");
        }
    }
    closedir(dir);
}

void print_json_item(cJSON* item, int depth)
{
    if (item == NULL)
    {
        printf("NULL\n");
        return;
    }
    char indent[depth * 2 + 1];
    memset(indent, ' ', depth * 2);
    indent[depth * 2] = '\0';
    const char* key = item->string ? item->string : "";
    switch (item->type)
    {
    case cJSON_False:
        printf("%s%s=false\n", indent, key);
        break;
    case cJSON_True:
        printf("%s%s=true\n", indent, key);
        break;
    case cJSON_NULL:
        printf("%s%s=NULL\n", indent, key);
        break;
    case cJSON_Number:
        if ((int)item->valuedouble == item->valuedouble)
        {
            printf("%s%s=%d\n", indent, key, (int)item->valuedouble);
        }
        else
        {
            printf("%s%s=%f\n", indent, key, item->valuedouble);
        }
        break;
    case cJSON_String:
        printf("%s%s=%s\n", indent, key, item->valuestring ? item->valuestring : "(null)");
        break;
    case cJSON_Array:
        printf("%s%s=[Array]\n", indent, key);
        for (cJSON* array_item = item->child; array_item != NULL; array_item = array_item->next)
        {
            print_json_item(array_item, depth + 1);
        }
        break;
    case cJSON_Object:
        if (key[0] != '\0')
        {
            printf("%s%s={Object}\n", indent, key);
        }
        else
        {
            printf("%s{Object}\n", indent);
        }
        for (cJSON* object_item = item->child; object_item != NULL; object_item = object_item->next)
        {
            print_json_item(object_item, depth + 1);
        }
        break;
    default:
        printf("%s%s=Unknown type\n", indent, key);
        break;
    }
}

void read_and_display_config(const char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (!file)
    {
        perror("fopen failed");
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(file_size + 1);
    if (buffer == NULL)
    {
        perror("malloc failed");
        fclose(file);
        return;
    }
    (void)fread(buffer, 1, file_size, file);

    buffer[file_size] = '\0';
    fclose(file);
    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    if (!root)
    {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    printf("Reading configuration file: %s\n", file_path);
    print_json_item(root, 0);
    cJSON_Delete(root);
}
