#include "stdio.h"
#include "string.h"

#include "afe_common.h"

#include "afe_hal.h"

#include "afe.h"
#include "pllsa.h"

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

void goto_commands(const command_registration *commands, const int argc, const char *argv[])
{
    int cnt = ARGC;
    char **vv = ARGV;

    // printf("file: %s, line: %d\n", __FILE__, __LINE__);
    // DPRINT_FUNCNAME(AAA);

    command_registration *thandler = commands;
    // thandler = commands;

    while (*vv) {
        printf("%s\n", *vv);

        // check all block

        while (!COMMAMDS_IS_NULL(thandler) || thandler->chain) {
            printf("%s\n", thandler->name);

            if (0 == strcmp(thandler->name, *vv)) { // function
                thandler->handler(cnt, vv);
                return;
            } else if (0 == strcmp(thandler->module, *vv)) { // function
                thandler = thandler->chain;
                continue;
            } else {
                if (thandler->chain) {
                    // printf("========== %s ==========\n", thandler->name);
                    goto_commands(thandler->chain, argc, argv);
                } else {
                    // printf("%s", thandler->submod);
                    // printf("\t%s", thandler->name);
                    // printf("\t%p", thandler->handler);
                    // printf("\t%s", thandler->usage);
                    // printf("\t%s", thandler->help);
                    // printf("\t%p", thandler->chain);
                    // printf("\n");
                    // thandler->handler(argc, argv);
                }
            }
            thandler++;
        }
        cnt--;
        vv++;
    }
}

void display_commands(const command_registration *commands, const int argc, const char *argv[])
{
    command_registration *thandler = commands;
    while (!COMMAMDS_IS_NULL(thandler) || thandler->chain) {
        if (thandler->chain) {
            printf("========== %s ==========\n", thandler->name);
            display_commands(thandler->chain, argc, argv);
        } else {
            printf("%s", thandler->module);
            printf("\t%s", thandler->name);
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
/*!
 * @brief AFE main like proc
 * @param argc arg count
 * @param argv string value args
 * @return
 */
int afe_cmd_proc(int argc, char const *argv[])
{
    afe_register_all_commands();
    pllsa_register_all_commands();

#if DEBUG
    display_sizeof();
    bit_field_test();
#endif // DEBUG

    for (int32_t i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    display_commands(afe_hal_commands, argc, argv);

    printf("============================\n\n\n");

    goto_commands(afe_hal_commands, argc - 1, &argv[1]);

    return 0;
}

command_registration afe_hal_commands[] = {
    {.module = "afe",   .name = nameof(afe_commands),   .chain = afe_commands  },
    {.module = "pllsa", .name = nameof(pllsa_commands), .chain = pllsa_commands},
    COMMAND_REGISTRATION_DONE,
};