#include <stdio.h>
#include <string.h>

#include "afe_co_int.h"
#include "afe_common.h"

#include "commands.h"

#include "afe.h"

#if 1

COMMAND_HANDLER_V2(afe, afe_lock_dect, __COUNTER__, "[abc:iaaaaaaaaaaaaant]",
                   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", NULL)
{
    DPRINT_FUNCNAME();
    return 0;
}

COMMAND_HANDLER_V2(afe, afe_reset, __COUNTER__, "[abc:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa:int]",
                   "abc", NULL)
{
    int cnt = ARGC;
    char **vv = ARGV;

    printf("file: %s, line: %d\n", __FILE__, __LINE__);
    DPRINT_FUNCNAME(AAA);

    while (*vv) {
        printf("%s\n", *vv);
        vv++;
    }

    return 0;
}

#endif // 0

command_registration afe_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_DONE,
    COMMAND_REGISTRATION_DONE,
};

#if (__COUNTER__) <= (1)
// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
#warning "please register at least one function"
void afe_register_all_commands()
{
    printf("%d\n", 1);
}
#else

void afe_register_all_commands()
{
    afe_register_commandhandler_0();
    afe_register_commandhandler_1();
}
#endif
