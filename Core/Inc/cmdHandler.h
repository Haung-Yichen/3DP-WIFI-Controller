#ifndef _CMD_HANDLER_H_
#define _CMD_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_CMD_COUNT 20
#define MAX_CMD_LEN   20

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CommandCallback)(const char* args);

bool register_command(const char* cmdName, CommandCallback callback);
bool execute_command(const char* input);

#ifdef __cplusplus
}
#endif

#endif /* _CMD_HANDLER_H_ */
