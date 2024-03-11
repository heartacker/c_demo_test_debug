#ifndef __AFE__H__
#define __AFE__H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "afe_common.h"
#include "commands.h"

extern command_registration afe_commands[];
// extern void afe_register_all_commands(void *owner);
extern void afe_register_all_commands(int argc, char *argv[]);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif //!__AFE__H__