#include <stdio.h>
#include <string.h>

#include "afe.h"

extern void display_sizeof();

/*!
 * @brief AFE main like proc
 * @param argc arg count
 * @param argv string value args
 * @return
 */
int afe_cmd_proc(int argc, char const *argv[])
{
    display_sizeof();
    for (int32_t i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    return 0;
}
