#include "stdio.h"
#include "string.h"

#include "afe_co_int.h"

// #include "afe_common.h"
#include "commands.h"

#include "afe_hal.h"

#include "afe.h"
#include "pll2g.h"
#include "pllsa.h"

command_registration *global_thandler = NULL;

extern void display_sizeof();

#define DATA_WHOLE 1
#if (defined(DATA_WHOLE) && (DATA_WHOLE))

struct bit_field_st {
    //  low
    unsigned byte0  : 8;
    unsigned byte1  : 8;
    unsigned byte2  : 8;
    unsigned byte3  : 8;
    unsigned byte9  : 9;
    unsigned byte7  : 7;
    unsigned        : 7;
    unsigned byte92 : 9;
};

union Data {
    uint64_t whole;
    struct bit_field_st bf;
};
#else
#endif // DATA_WHOLE

int gline = 0;

void bit_field_test()
{
    LINE_START_0(int, a);
    int a1 = LINE_N(a);
    printf("%d\n", a1);
    int abc = GS_NEXT(gline);
    int next = GS_NEXT(gline);
    struct bit_field_st bit_field_st[2];
    int next1 = GS_NEXT(gline);
    int next2 = GS_NEXT(gline);

    printf("%d\n", GS_NEXT(gline));
    printf("%d\n", GS_NEXT(gline));
    printf("%d\n", GS_NEXT(gline));

    for (int II = 0; II < 100; II++) {
        printf("%d\n", GS_NEXT(gline));
    }

    bit_field_st[0].byte0 = 0x12;
    bit_field_st[0].byte1 = 0x34;
    bit_field_st[0].byte2 = 0x56;
    bit_field_st[0].byte3 = 0x78;
    bit_field_st[0].byte3 = 0x78;
    bit_field_st[0].byte92 = 0xaa;
    // uint64_t *p = 0x12345678;
    // bit_field_st[1];
    for (size_t i = 0; i < 2; i++) {
        printf("bit_field_st\t%016x\n", bit_field_st[i]);
        printf("bit_field_st.0\t%08x\n", bit_field_st[i].byte0);
        printf("bit_field_st.1\t%08x\n", bit_field_st[i].byte1);
        printf("bit_field_st.2\t%08x\n", bit_field_st[i].byte2);
        printf("bit_field_st.3\t%08x\n", bit_field_st[i].byte3);
        printf("bit_field_st.92\t%08x\n", bit_field_st[i].byte92);
    }
    {
        union Data data;
        data.whole = 0x123456789ab0c1d2; // 1311768467462996434
        printf("data.bf\t%016x\n", data.bf);
        printf("data.bf.0\t%08x\n", data.bf.byte0);
        printf("data.bf.1\t%08x\n", data.bf.byte1);
        printf("data.bf.2\t%08x\n", data.bf.byte2);
        printf("data.bf.3\t%08x\n", data.bf.byte3);
        printf("data.bf.92\t%08x\n", data.bf.byte92);
    }
}

#if OLDER
void display_commands(const command_registration *commands, const int argc, const char *argv[], int level)
{
    command_registration *thandler = commands;
    while (!IS_COMMANDP_NULL(thandler)) {
        if (thandler->chain) {
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("%s.%s\n", thandler->module, thandler->name);
            display_commands(thandler->chain, argc, argv, level + 1);
        } else {
            for (int i = 0; i < level; i++) {
                printf("  ");
            }

            printf("%s", thandler->module);
            printf(".%s", thandler->name);
            printf("\t%p", thandler->handler);
            printf("\t%s", thandler->usage);
            printf("\t%s", thandler->help);
            printf("\t%p", thandler->chain);
            printf("\n");
            // thandler->handler(argc, argv);
        }
        thandler++;
    }
}

command_registration *goto_commands(const command_registration *commands, const int argc, const char *argv[], int level)
{
    int cnt = ARGC;
    char **vv = ARGV;

    // printf("file: %s, line: %d\n", __FILE__, __LINE__);
    // DPRINT_FUNCNAME(AAA);

    command_registration *thandler = commands;
    command_registration *iterhandler = thandler;
    command_registration *temp = NULL;
    // thandler = commands;

    if (*vv) {
        printf("$ %s\n", *vv);

        // check all block
        while (!IS_COMMANDP_NULL(iterhandler)) {
            //! printf("# %s\n", iterhandler->module);
            if (iterhandler->chain && (0 == strcmp(iterhandler->module, *vv))) {
                iterhandler = iterhandler->chain;
                return goto_commands(iterhandler, argc - 1, argv + 1, level + 1);
            } else {
            }
            iterhandler++;
        }

        // check all function

        iterhandler = thandler;

        while (!IS_COMMANDP_NULL(iterhandler)) {
            //! printf("- %s\n", iterhandler->name);

            if (0 == strcmp(iterhandler->name, *vv)) { // function
                printf("execute: %s.%s\n", iterhandler->module, iterhandler->name);
                iterhandler->rev = NO_EXE;
                iterhandler->rev = iterhandler->handler(cnt, vv);
                return iterhandler;
            }
            iterhandler++;
        }

        // check all subchain
        iterhandler = thandler;

        while (!IS_COMMANDP_NULL(iterhandler)) {

            if (iterhandler->chain) {
                //! printf("## %s\n", iterhandler->name);
                temp = goto_commands(iterhandler->chain, argc, argv, level + 1);
                if (IS_COMMANDP_NULL(temp)) {
                    iterhandler++;
                    continue;
                } else {
                    return temp;
                }
            }
            iterhandler++;
        }

        // check all subchain
        iterhandler = thandler;

        while (!IS_COMMANDP_NULL(iterhandler)) {

            if (iterhandler->chain) {
                //! printf("## %s\n", iterhandler->name);
                temp = goto_commands(iterhandler->chain, argc, argv, level + 1);
                if (IS_COMMANDP_NULL(temp)) {
                    iterhandler++;
                    continue;
                } else {
                    return temp;
                }
            }
            iterhandler++;
        }
    }

    if (IS_COMMANDP_NULL(iterhandler)) {
        printf("cannot find the command: %s\n", *vv);
        if (level == 0) {
            display_commands(thandler, argc, argv, 0);
        }
        return thandler;
    } else {
        return iterhandler;
    }
}

#endif // OLDER

char cmdline[128];
/*!
 * @brief AFE main like proc
 * @param argc arg count
 * @param argv string value args
 * @return
 */
int afe_cmd_proc(int argc, char const *argv[])
{
    afe_register_all_commands((int)afe_commands, argv);
    pllsa_register_all_commands(afe_commands);
    pll2g_register_all_commands(afe_commands);

#if DEBUG
    display_sizeof();
    bit_field_test();
#endif // DEBUG

    for (int32_t i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    printf("============================\n\n\n");
    display_commands(wifi_commands, argc, argv, 0);

    if (!global_thandler)
        global_thandler = wifi_commands;
    printf("============================\n\n\n");
    goto_commands(wifi_commands, &global_thandler, argc - 1, &argv[1], 0);
    // global_thandler = goto_commands(global_thandler, argc - 1, &argv[1], 0);
    if (!IS_COMMANDP_NULL(global_thandler)) {
        printf("global_thandler: name = %s, retv=%d\n", global_thandler->name, global_thandler->rev);
    } else {
        // NO_EXE
    }

    while (1) {
        printf("Enter a command: ");
        scanf(" %s", cmdline);
        char c = *cmdline;
        goto_commands(global_thandler, &global_thandler, 1, cmdline, 0);
        // Process the command
        switch (c && (strlen(cmdline) == 1)) {
        case 'A':
            // Process command A
            break;
        case 'B':
            // Process command B
            break;
        case 'Q':
            // Process command Q
            return 0; // Exit the program
        }
    }
    return 0;
}

#if 0
command_registration afe_hal_commands[] = {
  // {.module = "afe", .name = nameof(afe_commands), .chain = NULL        },
    {.module = "afe", .name = nameof(afe_commands), .chain = afe_commands},

    COMMAND_REGISTRATION_DONE,
};
#endif // 0

command_registration wifi_commands[] = {
    {
     .module = "wifi",
     .help = "",
     .usage = "",
     .handler = NULL,
     .name = nameof(afe),
     .chain = afe_commands,
     },
 // {.module = "wifi", .name = nameof(afe_hal2_commands), .chain = afe_hal_commands},
    COMMAND_REGISTRATION_DONE,
};
