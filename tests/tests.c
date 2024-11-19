#include "execution.h"
#include "jobs.h"
#include "utils.h"
#include <unity/unity.h>
#define TEST_BUFFER 256

void setUp(void)
{
}

void tearDown(void)
{
}

void test_add_job(void)
{
    pid_t test_pid = 1234;
    char* command = "test_command";
    add_job(test_pid, command);

    TEST_ASSERT_EQUAL_INT(1, job_count);
    TEST_ASSERT_EQUAL_INT(test_pid, jobs[0].pid);
    TEST_ASSERT_EQUAL_STRING(command, jobs[0].command);
}

void test_parse_input(void)
{
    ParsedCommand cmd;
    char input[] = "echo Hello, World!";
    parse_input(input, &cmd);

    TEST_ASSERT_EQUAL_STRING("echo", cmd.args[0]);
    TEST_ASSERT_EQUAL_STRING("Hello,", cmd.args[1]);
    TEST_ASSERT_EQUAL_STRING("World!", cmd.args[2]);
    TEST_ASSERT_NULL(cmd.args[3]);
}

void test_handle_cd_valid_path(void)
{
    ParsedCommand cmd;
    cmd.args[1] = "/tmp";

    handle_cd(&cmd);

    char cwd[TEST_BUFFER];
    getcwd(cwd, sizeof(cwd));

    TEST_ASSERT_EQUAL_STRING("/tmp", cwd);
}

void test_execute_command(void)
{
    ParsedCommand cmd;
    char input[] = "invalid_command";
    parse_input(input, &cmd);
    execute_command(&cmd);

    TEST_ASSERT_MESSAGE(cmd.is_internal == 0, "Should identify invalid_command as not internal");
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_job);
    RUN_TEST(test_parse_input);
    RUN_TEST(test_handle_cd_valid_path);
    RUN_TEST(test_execute_command);
    return UNITY_END();
}
