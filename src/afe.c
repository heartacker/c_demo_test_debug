#include <stdio.h>
#include <string.h>

#include "afe.h"

extern void display_sizeof();

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

void bit_field_test()
{

    struct bit_field_st bit_field_st[2];
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

    bit_field_test();

    return 0;
}
