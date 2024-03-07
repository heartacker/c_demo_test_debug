#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afe_co_int.h"
#include "afe_common.h"

#include "commands.h"

#include "afe.h"
#include "pll2g.h"
#include "pllsa.h"

#if 1

#if 1
#define DEFINE_FUNC(NAME, typename0, typename1, types...) NAME(typename0, typename1, types)
DEFINE_FUNC(int abc, int a, int b, char c, char e)
{
    return 0;
}

#endif // 1

COMMAND_HANDLER_V2(__COUNTER__, NULL, "aaaaaaaaaaaaaaaaaaaaaa COMMAND_HANDLER_V2 aaaaaaaaaaaaaaaaaaaa",
                   "[abc:iaaaaaaaaaaaaant]", afe, afe_lock_dect)
{
    DPRINT_FUNCNAME();
    return 0;
}

COMMAND_HANDLER_V3(__COUNTER__, NULL, "afe_reset0", "(int a, int b)", afe, reset0, int afe_reset0(int a, int b))
{
    return 0;
}

COMMAND_HANDLER_V5(afe, reset, int afe_reset(uint32_t mask, char dif))
{
    uint32_t mask = ARGV2_uint32_t(1);
    char dif = ARGV2_char(2);
    return afe_reset(mask, dif);
}

COMMAND_REGISTER_V5(__COUNTER__, NULL, "help", "(uint32_t mask, char dif)", afe, reset,
                    int afe_reset(uint32_t mask, char dif))
{

    printf("%d, %c", mask, dif);
    return 0;
}

#endif // 0

#if (__COUNTER__) < (1)
// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
#warning "please register at least one function"
void afe_register_all_commands(void *ownner)
{
    printf("%d\n", 1);
}
#else

command_registration afe_commands[__COUNTER__ + 2] = {
    COMMAND_REGISTRATION_DONE,
    COMMAND_REGISTRATION_DONE,
    COMMAND_REGISTRATION_DONE,
    {.module = "pllsa", .name = nameof(pllsa_commands), .chain = pllsa_commands},
    {.module = "pll2g", .name = nameof(pll2g_commands), .chain = pll2g_commands},
    COMMAND_REGISTRATION_DONE
};

void afe_register_all_commands(void *ownner)
{
    // memset(afe_commands, 0, ARRAY_LENS(afe_commands));
    module_register_commandhandler_0();
    module_register_commandhandler_1();
    module_register_commandhandler_2();

    for (char i = 0; i < ARRAY_LENS(afe_commands); i++) {
        if (!IS_COMMAND_NULL(afe_commands[i])) {
            afe_commands[i].upperchain = ownner;
        }
    }
}
#endif
