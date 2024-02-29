#include <stdio.h>
#include <string.h>

#include "afe_common.h"

#define nameof(obj) #obj

struct type_and_name {
    char *name;
    char size;
};

/*!
 * @brief 如果 类型名字 和 变量名字一样？
 */
extern struct type_and_name *type_and_name;

/* clang-format on */
// AlignArrayOfStructures: Right

struct type_and_name const type_and_names[] = {
    {          nameof(char),           sizeof(char)},
    {        nameof(int8_t),         sizeof(int8_t)},
    { nameof(unsigned char),  sizeof(unsigned char)},
    {       nameof(uint8_t),        sizeof(uint8_t)},
    {         nameof(short),          sizeof(short)},
    {       nameof(int16_t),        sizeof(int16_t)},
    {nameof(unsigned short), sizeof(unsigned short)},
    {      nameof(uint16_t),       sizeof(uint16_t)},
    {           nameof(int),            sizeof(int)},
    {       nameof(int32_t),        sizeof(int32_t)},
    {  nameof(unsigned int),   sizeof(unsigned int)},
    {      nameof(uint32_t),       sizeof(uint32_t)},
    { nameof(unsigned long),  sizeof(unsigned long)},
    {      nameof(uint64_t),       sizeof(uint64_t)},
    {     nameof(long long),      sizeof(long long)},
    {      nameof(int128_t),       sizeof(int128_t)},
    {     nameof(long long),      sizeof(long long)},
    {     nameof(uint128_t),      sizeof(uint128_t)}
};
/* clang-format on */

/*!
 * @brief display size of number type
 */
void display_sizeof()
{
#if DEBUG
    for (int ii = 0; ii < sizeof(type_and_names) / sizeof(struct type_and_name); ii++) {
        printf("%s\t\t= %d\n", type_and_names[ii].name, type_and_names[ii].size);
    }

#else
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

#define aaaa         0
#define bbbbbbbb     1

#define bbbbbbbbssss 1

/**/
#define aaaaaabbbbbbbbssss 1

int main(int argc, char const *argv[])
// int main(int argc, const char *argv[])
{
    strcpy(argv[0], "afe_cmd");
    afe_cmd_proc(argc, argv);
}
