#include "cmdHandler.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    char cmdName[MAX_CMD_LEN];
    CommandCallback callback;
} Command_Typedef;

static Command_Typedef commands[MAX_CMD_COUNT];
static uint8_t cmdQty = 0;

bool register_command(const char* cmdName, CommandCallback callback)
{
    if (cmdQty >= MAX_CMD_COUNT || cmdName == NULL || callback == NULL)
        return false;

    // 檢查是否重複
    for (uint8_t i = 0; i < cmdQty; ++i) {
        if (strcmp(commands[i].cmdName, cmdName) == 0) {
            return false;  // 已存在相同命令
        }
    }

    strncpy(commands[cmdQty].cmdName, cmdName, MAX_CMD_LEN - 1);
    commands[cmdQty].cmdName[MAX_CMD_LEN - 1] = '\0';
    commands[cmdQty].callback = callback;
    ++cmdQty;
    return true;
}

bool execute_command(const char* input)
{
    if (input == NULL) return false;

    // 找出空白分隔：命令 + 參數
    const char* spacePos = strchr(input, ' ');
    char cmdName[MAX_CMD_LEN] = {0};
    const char* args = "";

    if (spacePos) {
        size_t len = (size_t)(spacePos - input);
        if (len >= MAX_CMD_LEN) len = MAX_CMD_LEN - 1;
        strncpy(cmdName, input, len);
        cmdName[len] = '\0';
        args = spacePos + 1;
    } else {
        strncpy(cmdName, input, MAX_CMD_LEN - 1);
        cmdName[MAX_CMD_LEN - 1] = '\0';
    }

    for (uint8_t i = 0; i < cmdQty; ++i) {
        if (strcmp(commands[i].cmdName, cmdName) == 0) {
            commands[i].callback(args);
            return true;
        }
    }

    return false;
}
