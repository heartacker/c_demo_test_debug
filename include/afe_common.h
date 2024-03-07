#ifndef __AFE_COMMON__H__
#define __AFE_COMMON__H__
#define DEBUG 1

typedef int (*Exe)(int, char **);

#define nameof(obj)          #obj

#define ARRAY_LENS(array)    (sizeof(array) / sizeof(array[0]))

#define DPRINT_FUNCNAME(...) printf("this function : %s\n", __func__)

/*treat line for count */

#define LINE_START_0(type, name) type name = __LINE__
#define LINE_N(name)             (__LINE__ - name - 1)

#define FILE_NUM_NEXT(g_s_value) ((g_s_value >= __LINE__) ? (g_s_value) : ((g_s_value)++))
#define GS_NEXT(g_s_value)       ((g_s_value)++)

#endif //!__AFE_COMMON__H__