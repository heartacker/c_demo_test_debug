#include "stdio.h"
#include "stdlib.h"

#include "afe_common.h"

#include "pllsa.h"

#include "commands.h"

START_REGISTER_COMMANDS(pllsa);

#if USE_COMMAND_FOR_DEBUG
COMMAND_HANDLER_V6(pllsa, reset, int pllsa_reset(uint32_t mask, char dif))
{
    uint32_t mask = ARGV2_uint32_t(1);
    char dif = ARGV2_char(2);
    return pllsa_reset(mask, dif);
}

COMMAND_REGISTER_V6(__COUNTER__, NULL, "help", "(uint32_t mask, char dif)", pllsa, reset,
                    int pllsa_reset(uint32_t mask, char dif))
#else
int pllsa_reset(uint32_t mask, char dif, ...)
#endif
{

    printf("%d, %c", mask, dif);
    return 0;
}

#if (__COUNTER__) < (1)
// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
// #warning "please register at least one function"
void pllsa_register_all_commands()
{
    printf("warning \"please register at least one function\"");
}
command_registration pllsa_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_DONE,
};
#else

command_registration pllsa_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_DONE,
    COMMAND_REGISTRATION_DONE,
};

void pllsa_register_all_commands()
{
    memset(pllsa_commands, 0, ARRAY_LENS(pllsa_commands));
    module_register_commandhandler_0();
    // pllsa_register_commandhandler_1();
}

#endif
