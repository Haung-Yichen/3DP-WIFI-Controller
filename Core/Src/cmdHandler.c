#include "cmdHandler.h"
#include <string.h>
#include <stdio.h>

/* 命令格式定義  */
typedef struct {
	char cmdName[MAX_CMD_LEN];
	CommandCallback callback;
} Command_Typedef;

static Command_Typedef commands[MAX_CMD_COUNT]; //存放所有命令
static uint8_t cmdQty = 0;						//目前命令數量

CmdHandlerStat register_command(const char *cmdName, CommandCallback callback) {

	if (cmdQty >= MAX_CMD_COUNT) {
		return QTY_OVER;
	}else if (cmdName == NULL) {
		return NAME_ERR;
	}else if (callback == NULL) {
		return CALBCK_ERR;
	}

	// 檢查是否重複
	for (uint8_t i = 0; i < cmdQty; ++i) {
		if (strcmp(commands[i].cmdName, cmdName) == 0) {  //重複
			return REPEAT_ERR;
		}
	}

	strncpy(commands[cmdQty].cmdName, cmdName, MAX_CMD_LEN - 1);
	commands[cmdQty].cmdName[MAX_CMD_LEN - 1] = '\0';
	commands[cmdQty].callback = callback;
	++cmdQty;
	return CMD_OK;
}

CmdHandlerStat execute_command(const char *cmd, void *res) {
	if (cmd == NULL) return CMD_ERR;

	const char *spacePos = strchr(cmd, ' ');
	char cmdName[MAX_CMD_LEN] = {0};
	const char *args = "";

	if (spacePos) {
		size_t len = (size_t) (spacePos - cmd);
		if (len >= MAX_CMD_LEN) len = MAX_CMD_LEN - 1;
		strncpy(cmdName, cmd, len);
		cmdName[len] = '\0';
		args = spacePos + 1;
	} else {
		strncpy(cmdName, cmd, MAX_CMD_LEN - 1);
		cmdName[MAX_CMD_LEN - 1] = '\0';
	}

	for (uint8_t i = 0; i < cmdQty; ++i) {
		if (strcmp(commands[i].cmdName, cmdName) == 0) {
			commands[i].callback(args, res);
			return CMD_OK;
		}
	}
	return EXC_ERR;
}

#ifdef DEBUG
void print_all_cmd(void) {
	for (uint8_t i = 0; i < cmdQty; ++i) {
		printf("%s\n", commands[i].cmdName);
	}
}
#endif