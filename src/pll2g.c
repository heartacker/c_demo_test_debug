#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include "afe_common.h"

#include "pll2g.h"

#include "commands.h"

COMMAND_HANDLER_V5(pll2g, reset, int pll2g_reset(int32_t mask, int32_t b))
{
    int32_t b = ARGV2_int32_t(1);
    return pll2g_reset(0, b);
}

COMMAND_REGISTER_V5(__COUNTER__, NULL, "AaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaA",
                    "AaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaA", pll2g, reset,
                    int pll2g_reset(int32_t mask, int32_t b))
{
    return mask + b;
}

#if (__COUNTER__) < (1)
// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
// #warning "please register at least one function"
void pllsa_register_all_commands()
{
    printf("warning \"please register at least one function\"");
}

#else

command_registration pll2g_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_DONE,
    COMMAND_REGISTRATION_DONE,
};

void pll2g_register_all_commands(void *ownner)
{
    memset(pll2g_commands, 0, ARRAY_LENS(pll2g_commands));
    module_register_commandhandler_0();
    // pllsa_register_commandhandler_1();
    for (char i = 0; i < ARRAY_LENS(pll2g_commands); i++) {
        if (!IS_COMMAND_NULL(pll2g_commands[i])) {
            pll2g_commands[i].upperchain = ownner;
        }
    }
}

#endif