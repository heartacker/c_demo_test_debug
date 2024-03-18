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

COMMAND_HANDLER_NOARGS(__COUNTER__, NULL, " COMMAND_HANDLER_NOARGS afe_lock_dect", "()", afe, lock_dect,
                       afe_lock_dect(), );
int afe_lock_dect()
{
    return 0;
}

COMMAND_HANDLER(__COUNTER__, NULL, "afe_reset", "(uint32_t mask, char dif)", afe, reset,
                afe_reset(uint32_t mask, char dif), )
{
    uint32_t mask = ARGV2_uint32_t(1);
    char dif = ARGV2_char(2);
    return afe_reset(mask, dif);
}

int afe_reset(uint32_t mask, char dif)
{

    printf("%d, %c", mask, dif);
    return 0;
}

COMMAND_HANDLER_LAMDA(
    __COUNTER__, NULL, "afe_reset", "(uint32_t mask, char dif)", afe, reset2, afe_reset2(uint32_t mask, char dif), {
        uint32_t mask = ARGV2_uint32_t(1);
        char dif = ARGV2_char(2);
        return afe_reset(mask, dif);
    }, );

int afe_reset2(uint32_t mask, char dif)
{

    printf("%d, %c", mask, dif);
    return 0;
}

COMMAND_ARGC_ARGV(__COUNTER__, NULL, "afe_reset_argcv", "(int argc, char *argv[])", afe, reset_argcv)
{
    return 0;
}

#endif // 0

#if (__COUNTER__) < (1)

// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
#warning "please register at least one function"
void afe_register_all_commands(int p_int_ownner, char *argv[])
{
    printf("%d\n", 1);
}

#else

command_registration afe_commands[__COUNTER__ + 2] = {
  // COMMAND_REGISTRATION_NONE,
    COMMAND_REGISTRATION_NONE,
    COMMAND_REGISTRATION_NONE,
    COMMAND_REGISTRATION_NONE,
    {.module = "afe", .name = nameof(pllsa), .chain = pllsa_commands},
    {.module = "afe", .name = nameof(pll2g), .chain = pll2g_commands},
    COMMAND_REGISTRATION_DONE
};

void afe_register_all_commands(int p_int_ownner, char *argv[])
{
    // memset(afe_commands, 0, ARRAY_LENS(afe_commands));
    module_register_commandhandler_0(((int *)p_int_ownner));
    module_register_commandhandler_1(((int *)p_int_ownner));
    module_register_commandhandler_2(((int *)p_int_ownner));

    for (char i = 0; i < ARRAY_LENS(afe_commands); i++) {
        if (!IS_COMMAND_NULL(afe_commands[i])) {
            afe_commands[i].upperchain = (void *)((int *)p_int_ownner);
        }
    }
}
#endif
