#include <stdio.h>
#include <string.h>

#include "afe_common.h"

void a();

int main(int argc, char const *argv[])
// int main(int argc, const char *argv[])
{
#if DEBUG
    printf("%ld\n", sizeof(char));
    printf("%ld\n", sizeof(int8_t));

    printf("%ld\n", sizeof(unsigned char));
    printf("%ld\n", sizeof(uint8_t));

    printf("%ld\n", sizeof(short));
    printf("%ld\n", sizeof(int16_t));

    printf("%ld\n", sizeof(unsigned short));
    printf("%ld\n", sizeof(uint16_t));

    printf("%ld\n", sizeof(int));
    printf("%ld\n", sizeof(int32_t));

    printf("%ld\n", sizeof(unsigned int));
    printf("%ld\n", sizeof(uint32_t));

    printf("%ld\n", sizeof(unsigned long));
    printf("%ld\n", sizeof(uint64_t));
#endif // DEBUG

    for (uint32_t i = 0; i < argc; i++) {
        printf("%d:\t %s\n", i, argv[i]);
    }

    strcpy(argv[0], "afe_cmd");

    for (int32_t i = 0; i < argc; i++) {
        printf("%u: %s\n", i, argv[i]);
    }
    printf("");

    a();
    return 0;
}

void a()
{
    printf("");
}

#if 0

a()
{
}

#endif // 0