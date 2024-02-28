#include <stdio.h>
#include <string.h>

#include "afe_common.h"

void display_sizeof()
{
#if DEBUG
    printf("sizeof(char)\t\t\t=\t%ld\n", sizeof(char));
    printf("sizeof(int8_t)\t\t\t=\t%ld\n", sizeof(int8_t));

    printf("sizeof(unsigned char)\t\t=\t%ld\n", sizeof(unsigned char));
    printf("sizeof(uint8_t)\t\t\t=\t%ld\n", sizeof(uint8_t));

    printf("sizeof(short)\t\t\t=\t%ld\n", sizeof(short));
    printf("sizeof(int16_t)\t\t\t=\t%ld\n", sizeof(int16_t));

    printf("sizeof(unsigned short)\t\t=\t%ld\n", sizeof(unsigned short));
    printf("sizeof(uint16_t)\t\t=\t%ld\n", sizeof(uint16_t));

    printf("sizeof(int)\t\t\t=\t%ld\n", sizeof(int));
    printf("sizeof(int32_t)\t\t\t=\t%ld\n", sizeof(int32_t));

    printf("sizeof(unsigned int)\t\t=\t%ld\n", sizeof(unsigned int));
    printf("sizeof(uint32_t)\t\t=\t%ld\n", sizeof(uint32_t));

    printf("sizeof(unsigned long)\t\t=\t%ld\n", sizeof(unsigned long));
    printf("sizeof(uint64_t)\t\t=\t%ld\n", sizeof(uint64_t));

    printf("sizeof(long long)\t\t=\t%ld\n", sizeof(long long));
    printf("sizeof(int128_t)\t\t=\t%ld\n", sizeof(int128_t));

    printf("sizeof(unsigned long long)\t=\t%ld\n", sizeof(unsigned long long));
    printf("sizeof(uint128_t)\t\t=\t%ld\n", sizeof(uint128_t));
#endif // DEBUG
}

extern int afe_cmd_proc(int argc, char const *argv[]);

int main(int argc, char const *argv[])
// int main(int argc, const char *argv[])
{
    for (uint32_t i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    strcpy(argv[0], "afe_cmd");
    for (int32_t i = 0; i < argc; i++) {
        printf("%u: %s\n", i, argv[i]);
    }
    return afe_cmd_proc(argc, argv);
}
