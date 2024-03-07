#include "stdio.h"

#include "pllsa.h"

#include "commands.h"

COMMAND_HANDLER_V2(pllsa, pllsa_reset, __COUNTER__, "AaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaA", "useful",
                   NULL)
{
    return 0;
}

command_registration pllsa_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_DONE,
};

#if (__COUNTER__) <= (1)
// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
#warning "please register at least one function"
void pllsa_register_all_commands()
{
    printf("%d\n", 1);
}

#else

void pllsa_register_all_commands()
{
    module_register_commandhandler_0();
    // pllsa_register_commandhandler_1();
}
#endif