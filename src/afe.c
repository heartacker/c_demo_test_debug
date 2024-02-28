#include <stdio.h>
#include <string.h>

#include "afe_common.h"

extern void display_sizeof();

int afe_cmd_proc(int argc, char const *argv[])
{
    display_sizeof();
    return 0;
}
