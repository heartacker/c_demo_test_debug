#include "stdio.h"
#include "stdlib.h"

#include "afe_co_int.h"

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
                    int pllsa_reset(uint32_t mask, char dif, ...))
#else
int pllsa_reset(uint32_t mask, char dif, ...)
#endif
{

    printf("%d, %c", mask, dif);
    return 0;
}

COMMAND_HANDLER(__COUNTER__, NULL, "kvco calibration", "(bool recal)", pllsa, kvco_calib,
                pllsa_kvco_calib(int recal, ...), (int, recal))
{
    int recal = ARGV2_bool(1);
    return pllsa_kvco_calib(recal);
}

int pllsa_kvco_calib(int recal, ...)
{
    return recal * 1024;
}

COMMAND_HANDLER(__COUNTER__, NULL, "kvco calibration", "(bool recal)", pllsa, enable_testpath,
                pllsa_enable_testpath(uint8_t path_select), (uint8_t path_select))
{
    uint8_t path_select = (uint8_t)ARGV2_int32_t(1);
    return pllsa_enable_testpath(path_select);
}

int pllsa_enable_testpath(uint8_t path_select)
{
    return path_select * +3000;
}

COMMAND_HANDLER_NOARGS(__COUNTER__, NULL, "config the pll sa bw", "()", pllsa, bw_config, pllsa_bw_config());

int pllsa_bw_config()
{
    printf("%s\n", __FUNCTION__);
    return 0;
}

#if (__COUNTER__) < (1)
// START_REGISTE_CMD(check, afe_commands, __COUNTER__);
// #warning "please register at least one function"
void pllsa_register_all_commands(void *owner)
{
    printf("warning \"please register at least one function\"");
}
command_registration pllsa_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_DONE,
};
#else

command_registration pllsa_commands[__COUNTER__] = {
    COMMAND_REGISTRATION_NONE,
    COMMAND_REGISTRATION_NONE,
    COMMAND_REGISTRATION_NONE,
    COMMAND_REGISTRATION_DONE,
};

void pllsa_register_all_commands(void *ownner)
{
    // memset(pllsa_commands, 0, ARRAY_LENS(pllsa_commands));
    module_register_commandhandler_0(ownner);
    module_register_commandhandler_1(ownner);
    module_register_commandhandler_2(ownner);
    for (char i = 0; i < ARRAY_LENS(pllsa_commands); i++) {
        if (!IS_COMMAND_NULL(pllsa_commands[i])) {
            pllsa_commands[i].upperchain = ownner;
        }
    }
}

#endif
