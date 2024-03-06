#ifndef __AFE_COMMON__H__
#define __AFE_COMMON__H__
#define DEBUG 1

typedef int (*Exe)(int, char **);

#define ARRAY_LENS(array)    (sizeof(array) / sizeof(array[0]))

#define nameof(obj)          #obj

#define DPRINT_FUNCNAME(...) printf("this function : %s\n", __func__)

#endif //!__AFE_COMMON__H__