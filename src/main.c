#include <stdio.h>

#include "afe_common.h"

void a();

int main(int argc, char const *argv[])
{
    printf("Hello World!\n");
    printf("%ld", sizeof(int));
    printf("\n");

    for (uint32_t i = 0; i < argc; i++) {
        printf("%d:\t %s\n", i, argv[i]);
    }

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