#ifndef AT_COMMANDS_H
#define AT_COMMANDS_H

#include <string.h>

#define AT_CMD_SIZE (64U + 1U)
#define AT_CMD "AT"
#define AT_CMD_BASE AT_CMD"+"
#define AT_CMD_SET "="
#define AT_CMD_INQUIRY "?"
#define AT_CMD_TEST "=?"

static inline char* at_command_execute(const char *command, char at_cmd_out[AT_CMD_SIZE])
{
    if ((command != NULL) && (at_cmd_out != NULL))
    {
        (void)strcat
        (
            strcpy
            (
                (char*)memset((void*)at_cmd_out, 0, AT_CMD_SIZE),
                AT_CMD_BASE
            ),
            command
        );
    }

    return at_cmd_out;
}

static inline char* at_command_inquiry(const char *command, char at_cmd_out[AT_CMD_SIZE])
{
    if ((command != NULL) && (at_cmd_out != NULL))
    {
        (void)strcat
        (
            at_command_execute(command, at_cmd_out),
            AT_CMD_INQUIRY
        );
    }

    return at_cmd_out;
}

static inline char* at_command_test(const char *command, char at_cmd_out[AT_CMD_SIZE])
{
    if ((command != NULL) && (at_cmd_out != NULL))
    {
        (void)strcat
        (
            at_command_execute(command, at_cmd_out),
            AT_CMD_TEST
        );
    }

    return at_cmd_out;
}

static inline char* at_command_set(const char *command, char *data, char at_cmd_out[AT_CMD_SIZE])
{
    if ((command != NULL) && (at_cmd_out != NULL) && (data != NULL))
    {
        size_t at_cmd_len = sizeof(AT_CMD_BASE) + strlen(command) + strlen(data);

        if (at_cmd_len <= AT_CMD_SIZE)
        {
            (void)strcat
            (
                strcat
                (
                    at_command_execute(command, at_cmd_out),
                    AT_CMD_SET
                ),
                data
            );
        }
    }

    return at_cmd_out;
}

#endif