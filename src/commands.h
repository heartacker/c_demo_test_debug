#ifndef __COMMANDS__H__
#define __COMMANDS__H__

#define USE_ARGCV                    1
#define BETTER_COMMAND_HANDLER_MACRO 1 // 是否使用宏注册
#define DEBUG_WITH_COMMANDS          1 // 全局命令

#define nameof(obj)                  #obj

// typedef struct _command {
//     char *cmd;
//     char *usages;
//     unsigned int *command_handler;
// } command;

// START_REGISTE_CMD(a, b,c);

struct command_invocation {
    // struct command_context *ctx;
    // struct command *current;
    const char *name;
    unsigned argc;
    const char **argv;
    // Jim_Obj * const *jimtcl_argv;
    // Jim_Obj *output;
};

#define __COMMAND_HANDLER_V0(name, extra...) int name(struct command_invocation *cmd)
#define __COMMAND_HANDLER_V1(name, extra...) int name(struct command_invocation *cmd, ##extra)
#define __COMMAND_HANDLER_V2(name, extra...) int name(int argc, char *argv[])

#if USE_ARGCV
/**
 * Command handlers may be defined with more parameters than the base
 * set provided by command.c.  This macro uses C99 magic to allow
 * defining all such derivative types using this macro.
 */
#define __COMMAND_HANDLER(name, extra...) __COMMAND_HANDLER_V2(name, extra)
#else
#define __COMMAND_HANDLER(name, extra...) __COMMAND_HANDLER_V0(name, extra)
#endif

typedef __COMMAND_HANDLER((*command_handler_t));

/**
 * Use this to macro to call a command helper (or a nested handler).
 * It provides command handler authors protection against reordering or
 * removal of unused parameters.
 *
 * @b Note: This macro uses lexical capture to provide some arguments.
 * As a result, this macro should be used @b only within functions
 * defined by the COMMAND_HANDLER or COMMAND_HELPER macros.  Those
 * macros provide the expected lexical context captured by this macro.
 * Furthermore, it should be used only from the top-level of handler or
 * helper function, or care must be taken to avoid redefining the same
 * variables in intervening scope(s) by accident.
 */
#define CALL_COMMAND_HANDLER(name, extra...) name(cmd, ##extra)

typedef struct _command_registration {

    const char *module;

    const char *name;

    command_handler_t handler;

    // Jim_CmdProc *jim_handler;
    // enum command_mode mode;

    const char *help;

    /** a string listing the options and arguments, required or optional */
    //! upchain
    const char *usage;

    /**
     * If non-NULL, the commands in @c chain will be registered in
     * the same context and scope of this registration record.
     * This allows modules to inherit lists commands from other
     * modules.
     */
    struct _command_registration *chain;

    struct _command_registration *upperchain;

    int rev;
} command_registration;

extern void display_commands(command_registration const *commands, int argc, char *argv[], int level);
extern void goto_commands(command_registration const *commands, command_registration **iterhandler, int argc,
                          char *argv[], int level);

#include "string.h"
#include "strings.h"

#define ARGC                  (argc)
#define ARGV                  (argv)
#define ARGV2_uint32_t(index) strtol(argv[index], NULL, 10)
#define ARGV2_int32_t(index)  atoi(argv[index])
#define ARGV2_pchar(index)    argv[index]
#define ARGV2_char(index)     *argv[index]

typedef enum _COMMAND_RESULT {
    CLR_BEFORE_EXE = (0x00ACCE55),
    CANOT_FIND_EXE = (0xEEACCE55),
    FIND_NULL_EXE = (0xFAACCE55),
} COMMAND_RESULT;

#define IS_COMMANDP_NULL(cmdd) (cmdd == NULL || (cmdd->module == NULL && cmdd->name == NULL && cmdd->chain == NULL))

#define IS_COMMAND_NULL(cmdd)  ((&cmdd == NULL) || (cmdd.module == NULL && cmdd.name == NULL && cmdd.chain == NULL))

#define ARGV2_bool(index)      ((strcmp(argv[index], "1") == 0) || (strcasecmp(argv[index], "true") == 0))

#if 0
#define ARGV2_bool(index)                                                                                              \
    (!(strcmp(argv[index], "0") == 0) || (strcasecmp(argv[index], "true") == 0) ||                                     \
     (strcmp(argv[index], "TRUE") == 0) || (strcmp(argv[index], "True") == 0))
#endif // 0

#if defined(BETTER_COMMAND_HANDLER_MACRO) && (BETTER_COMMAND_HANDLER_MACRO)

/** useful MACRO for command register*/
#define START_REGISTER_COMMANDS(blocks)                                                                                \
    extern command_registration blocks##_##commands[];                                                                 \
    extern void blocks##_register_all_commands(void *owner)

#define END_REGISTER_COMMANDS(blocks, number) command_registration blocks##_##commands[number]

#define __COMMAND_HANDLER_REGISTER(indexx, upchain, helpstr, usagestr, thismodule, func_name, extra...)                \
    static void module##_##register_commandhandler##_##indexx()                                                        \
    {                                                                                                                  \
        thismodule##_commands[indexx].module = #thismodule;                                                            \
        thismodule##_commands[indexx].name = #thismodule "_" #func_name;                                               \
        thismodule##_commands[indexx].handler = thismodule##_##func_name##_command_handler;                            \
        thismodule##_commands[indexx].help = helpstr;                                                                  \
        thismodule##_commands[indexx].usage = usagestr;                                                                \
        thismodule##_commands[indexx].chain = NULL;                                                                    \
        thismodule##_commands[indexx].upperchain = upchain;                                                            \
    }

#define __COMMAND_HANDLER_REGISTER_ARGC_ARGV(indexx, upchain, helpstr, usagestr, thismodule, func_name, extra...)      \
    static void module##_##register_commandhandler##_##indexx(void *pupperchain)                                       \
    {                                                                                                                  \
        thismodule##_commands[indexx].module = #thismodule;                                                            \
        thismodule##_commands[indexx].name = #thismodule "_" #func_name;                                               \
        thismodule##_commands[indexx].handler = thismodule##_##func_name;                                              \
        thismodule##_commands[indexx].help = helpstr;                                                                  \
        thismodule##_commands[indexx].usage = usagestr;                                                                \
        thismodule##_commands[indexx].chain = NULL;                                                                    \
        thismodule##_commands[indexx].upperchain = pupperchain;                                                        \
    }

#define COMMAND_HANDLER2(thismodule, func_name, feild, indexx, helpstr)                                                \
    extern __COMMAND_HANDLER(thismodule##_##func_name);                                                                \
    __COMMAND_HANDLER_REGISTER(thismodule, func_name, feild, indexx, helpstr)                                          \
    __COMMAND_HANDLER(thismodule##_##func_name)

/*====================================================================================================================*/
#define __COMMAND_HANDLER_REGISTER_UP(indexx, upchain, helpstr, usagestr, thismodule, func_name, extra...)             \
    static void module##_##register_commandhandler##_##indexx(void *pupperchain)                                       \
    {                                                                                                                  \
        thismodule##_commands[indexx].module = #thismodule;                                                            \
        thismodule##_commands[indexx].name = #thismodule "_" #func_name;                                               \
        thismodule##_commands[indexx].handler = thismodule##_##func_name##_command_handler;                            \
        thismodule##_commands[indexx].help = helpstr;                                                                  \
        thismodule##_commands[indexx].usage = usagestr;                                                                \
        thismodule##_commands[indexx].chain = NULL;                                                                    \
        thismodule##_commands[indexx].upperchain = pupperchain;                                                        \
    }

#define COMMAND_HANDLER_V4(thismodule, func_name, full_func_name, args...)                                             \
    extern int thismodule##_##func_name##full_func_name;                                                               \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)

#define COMMAND_REGISTER_V4(indexx, upchain, helpstr, usagestr, thismodule, func_name, full_func_name, args...)        \
    /* extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler); */                                        \
    /* extern int thismodule##_##func_name##full_func_name; */                                                         \
    __COMMAND_HANDLER_REGISTER(indexx, upchain, helpstr, usagestr, thismodule, func_name)                              \
                                                                                                                       \
    int thismodule##_##func_name##full_func_name

#if 1 //! V6
/**
 * ! register function arg and argc
 */
#define COMMAND_HANDLER_V6(thismodule, func_name, full_func_name, args...)                                             \
    extern full_func_name;                                                                                             \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)

/**
 * ! register function @brief COMMAND_HANDLER_V6
 */
#define COMMAND_REGISTER_V6(indexx, upchain, helpstr, usagestr, thismodule, func_name, full_func_name, args...)        \
    /* extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler); */                                        \
    /* extern int thismodule##_##func_name##full_func_name; */                                                         \
    __COMMAND_HANDLER_REGISTER_UP(indexx, upchain, helpstr, usagestr, thismodule, func_name)                           \
    full_func_name

#endif // 0

#define ARG_DIV(TYPE, NAME) TYPE NAME

/**
 * ! register (maybe just for debug) function  with (argc/argv)
 */

#define COMMAND_ARGC_ARGV(indexx, upchain, helpstr, usagestr, thismodule, func_name, extra...)                         \
    extern __COMMAND_HANDLER(thismodule##_##func_name);                                                                \
    __COMMAND_HANDLER_REGISTER_ARGC_ARGV(indexx, upchain, helpstr, usagestr, thismodule, func_name, extra)             \
    __COMMAND_HANDLER(thismodule##_##func_name)

/**
 * ! register function related commandhandler without args
 */
#define COMMAND_HANDLER_NOARGS(indexx, upchain, helpstr, usagestr, thismodule, func_name, full_func_name, arg_t_n...)  \
    extern int full_func_name;                                                                                         \
    extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler);                                              \
    __COMMAND_HANDLER_REGISTER_UP(indexx, upchain, helpstr, usagestr, thismodule, func_name)                           \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)                                                      \
    {                                                                                                                  \
        return full_func_name;                                                                                         \
    }

/**
 * ! register function related commandhandler with args
 *
 * Always use this macro to define new command handler functions.
 * It ensures the parameters are ordered, typed, and named properly, so
 * they be can be used by other macros (e.g. COMMAND_PARSE_NUMBER).
 * All command handler functions must be defined as static in scope.
 */
#define COMMAND_HANDLER(indexx, upchain, helpstr, usagestr, thismodule, func_name, full_func_name, arg_t_n...)         \
    extern int full_func_name;                                                                                         \
    extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler);                                              \
    __COMMAND_HANDLER_REGISTER_UP(indexx, upchain, helpstr, usagestr, thismodule, func_name)                           \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)

/**
 * ! register function related commandhandler with args
 *
 * Always use this macro to define new command handler functions.
 * It ensures the parameters are ordered, typed, and named properly, so
 * they be can be used by other macros (e.g. COMMAND_PARSE_NUMBER).
 * All command handler functions must be defined as static in scope.
 */
#define COMMAND_HANDLER_LAMDA(indexx, upchain, helpstr, usagestr, thismodule, func_name, full_func_name, lamda,        \
                              arg_t_n...)                                                                              \
    extern int full_func_name;                                                                                         \
    extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler);                                              \
    __COMMAND_HANDLER_REGISTER_UP(indexx, upchain, helpstr, usagestr, thismodule, func_name)                           \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)                                                      \
    {                                                                                                                  \
        lamda                                                                                                          \
    }

#define CONNECT_2_MACRO(A_MACRO, B_MACRO)                     A_MACRO##B_MACRO
#define __LOCAL_VAR(__LOCAL_TYPE, __LOCAL_NAME, __INDEX)      __LOCAL_TYPE __LOCAL_NAME = ARGV2_##__LOCAL_TYPE(__INDEX)
#define __FUNC_ARG_VAR(__LOCAL_TYPE, __LOCAL_NAME, __INDEX)   (__LOCAL_TYPE __LOCAL_NAME)
#define __LOCAL_VAR_REAL(__LOCAL_TYPE, __LOCAL_NAME, __INDEX) __LOCAL_NAME

/**
 * ! register function related commandhandler with args
 *
 * Always use this macro to define new command handler functions.
 * It ensures the parameters are ordered, typed, and named properly, so
 * they be can be used by other macros (e.g. COMMAND_PARSE_NUMBER).
 * All command handler functions must be defined as static in scope.
 */
#define COMMAND_HANDLER_AUTO(indexx, upchain, helpstr, usagestr, thismodule, func_name, full_func_name, arg_t_n...)    \
    extern int full_func_name __FUNC_ARG_VAR##arg_t_n;                                                                 \
    extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler);                                              \
    __COMMAND_HANDLER_REGISTER_UP(indexx, upchain, helpstr, usagestr, thismodule, func_name)                           \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)                                                      \
    {                                                                                                                  \
        __LOCAL_VAR##arg_t_n;                                                                                          \
        return full_func_name __LOCAL_VAR_REAL##arg_t_n;                                                               \
    }

/** Use this as the last entry in an array of command_registration records. */
#define COMMAND_REGISTRATION_DONE                                                                                      \
    {                                                                                                                  \
        .module = NULL, .name = NULL, .handler = NULL, .usage = NULL, .help = NULL, .chain = NULL                      \
    }

/** Use this as the last entry in an array of command_registration records. */
#define COMMAND_REGISTRATION_NONE                                                                                      \
    {                                                                                                                  \
        .module = NULL, .name = NULL, .handler = NULL, .usage = NULL, .help = "FORGET TO ADD THIS?", .chain = NULL     \
    }

// #define COMMAMDS_IS_NULL(thecmd) (thecmd->name == NULL || thecmd->handler == NULL)

#define START_REGISTE_CMD(funcname, fieldname, iid, ...)                                                               \
    int funcname##_##fieldname()                                                                                       \
    {                                                                                                                  \
        int __i = iid;                                                                                                 \
        command_registration *_thecmd = &fieldname[__i];                                                               \
        if (IS_COMMANDP_NULL(_thecmd)) {                                                                               \
            return -1;                                                                                                 \
        }                                                                                                              \
        return 0;                                                                                                      \
    }

#endif // BETTER_COMMAND_HANDLER_MACRO

// 参数拼接
// 参数拼接
#define FL_CONCAT_(l, r) l##r

#define FL_CONCAT(l, r)  FL_CONCAT_(l, r)

#define FL_DOARG0(s, f, a, o)
#define FL_DOARG1(s, f, a, v, ...)  f(a, v)
#define FL_DOARG2(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG1(s, f, a, __VA_ARGS__)
#define FL_DOARG3(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG2(s, f, a, __VA_ARGS__)
#define FL_DOARG4(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG3(s, f, a, __VA_ARGS__)
#define FL_DOARG5(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG4(s, f, a, __VA_ARGS__)
#define FL_DOARG6(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG5(s, f, a, __VA_ARGS__)
#define FL_DOARG7(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG6(s, f, a, __VA_ARGS__)
#define FL_DOARG8(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG7(s, f, a, __VA_ARGS__)
#define FL_DOARG9(s, f, a, v, ...)  FL_DOARG1(s, f, a, v) s FL_DOARG8(s, f, a, __VA_ARGS__)
#define FL_DOARG10(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG9(s, f, a, __VA_ARGS__)
#define FL_DOARG11(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG10(s, f, a, __VA_ARGS__)
#define FL_DOARG12(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG11(s, f, a, __VA_ARGS__)
#define FL_DOARG13(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG12(s, f, a, __VA_ARGS__)
#define FL_DOARG14(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG13(s, f, a, __VA_ARGS__)
#define FL_DOARG15(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG14(s, f, a, __VA_ARGS__)
#define FL_DOARG16(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG15(s, f, a, __VA_ARGS__)
#define FL_DOARG17(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG16(s, f, a, __VA_ARGS__)
#define FL_DOARG18(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG17(s, f, a, __VA_ARGS__)
#define FL_DOARG19(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG18(s, f, a, __VA_ARGS__)
#define FL_DOARG20(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG19(s, f, a, __VA_ARGS__)
#define FL_DOARG21(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG20(s, f, a, __VA_ARGS__)
#define FL_DOARG22(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG21(s, f, a, __VA_ARGS__)
#define FL_DOARG23(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG22(s, f, a, __VA_ARGS__)
#define FL_DOARG24(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG23(s, f, a, __VA_ARGS__)
#define FL_DOARG25(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG24(s, f, a, __VA_ARGS__)
#define FL_DOARG26(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG25(s, f, a, __VA_ARGS__)
#define FL_DOARG27(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG26(s, f, a, __VA_ARGS__)
#define FL_DOARG28(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG27(s, f, a, __VA_ARGS__)
#define FL_DOARG29(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG28(s, f, a, __VA_ARGS__)
#define FL_DOARG30(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG29(s, f, a, __VA_ARGS__)
#define FL_DOARG31(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG30(s, f, a, __VA_ARGS__)
#define FL_DOARG32(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG31(s, f, a, __VA_ARGS__)
#define FL_DOARG33(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG32(s, f, a, __VA_ARGS__)
#define FL_DOARG34(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG33(s, f, a, __VA_ARGS__)
#define FL_DOARG35(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG34(s, f, a, __VA_ARGS__)
#define FL_DOARG36(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG35(s, f, a, __VA_ARGS__)
#define FL_DOARG37(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG36(s, f, a, __VA_ARGS__)
#define FL_DOARG38(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG37(s, f, a, __VA_ARGS__)
#define FL_DOARG39(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG38(s, f, a, __VA_ARGS__)
#define FL_DOARG40(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG39(s, f, a, __VA_ARGS__)
#define FL_DOARG41(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG40(s, f, a, __VA_ARGS__)
#define FL_DOARG42(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG41(s, f, a, __VA_ARGS__)
#define FL_DOARG43(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG42(s, f, a, __VA_ARGS__)
#define FL_DOARG44(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG43(s, f, a, __VA_ARGS__)
#define FL_DOARG45(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG44(s, f, a, __VA_ARGS__)
#define FL_DOARG46(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG45(s, f, a, __VA_ARGS__)
#define FL_DOARG47(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG46(s, f, a, __VA_ARGS__)
#define FL_DOARG48(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG47(s, f, a, __VA_ARGS__)
#define FL_DOARG49(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG48(s, f, a, __VA_ARGS__)
#define FL_DOARG50(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG49(s, f, a, __VA_ARGS__)
#define FL_DOARG51(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG50(s, f, a, __VA_ARGS__)
#define FL_DOARG52(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG51(s, f, a, __VA_ARGS__)
#define FL_DOARG53(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG52(s, f, a, __VA_ARGS__)
#define FL_DOARG54(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG53(s, f, a, __VA_ARGS__)
#define FL_DOARG55(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG54(s, f, a, __VA_ARGS__)
#define FL_DOARG56(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG55(s, f, a, __VA_ARGS__)
#define FL_DOARG57(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG56(s, f, a, __VA_ARGS__)
#define FL_DOARG58(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG57(s, f, a, __VA_ARGS__)
#define FL_DOARG59(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG58(s, f, a, __VA_ARGS__)
#define FL_DOARG60(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG59(s, f, a, __VA_ARGS__)
#define FL_DOARG61(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG60(s, f, a, __VA_ARGS__)
#define FL_DOARG62(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG61(s, f, a, __VA_ARGS__)
#define FL_DOARG63(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG62(s, f, a, __VA_ARGS__)
#define FL_DOARG64(s, f, a, v, ...) FL_DOARG1(s, f, a, v) s FL_DOARG63(s, f, a, __VA_ARGS__)

#define FL_FOREACH_(sepatator, fun, funarg, ...)                                                                       \
    FL_CONCAT(FL_DOARG, FL_ARG_COUNT(__VA_ARGS__))(sepatator, fun, funarg, __VA_ARGS__)
#define FL_FOREACH(sepatator, fun, funarg, ...) FL_FOREACH_(sepatator, fun, funarg, __VA_ARGS__)
// 为动态参数 __VA_ARGS__ 每一个参数调用 fun 宏,最大支持64个参数
// sepatator 分隔符
// fun 函数宏
// funarg 函数宏的参数
#define enum_elem(p, n) p##n,

// 定义一个名为clsName的枚举类型,动态参数提供枚举类型的元素，最多支持64个元素
// clsName##_为元素名前缀
#define FL_DEF_ENUM(clsName, ...)                                                                                      \
    enum _##clsName{                                                                                                   \
        FL_FOREACH(, enum_elem, clsName##_, __VA_ARGS__) /* must be last position,for competing enum's number  */      \
        clsName##_LAST_ID} clsName;

#define PP_ARG_X(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t,   \
                 u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, XX,   \
                 ...)                                                                                                  \
    XX
#define PP_ARG_N(...)                                                                                                  \
    PP_ARG_X("ignored", ##__VA_ARGS__, Z, Y, X, W, V, U, T, S, R, Q, P, O, N, M, L, K, J, I, H, G, F, E, D, C, B, A,   \
             z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a, 9, 8, 7, 6, 5, 4, 3, 2, 1,  \
             0)

#define PP_VA_NAME(prefix, ...) PP_CAT2(prefix, PP_ARG_N(__VA_ARGS__))
#define PP_CAT2(a, b)           PP_CAT2_1(a, b)
#define PP_CAT2_1(a, b)         a##b

// more func_N ...
#define ABC(...)              PP_VA_NAME(func_, __VA_ARGS__)(__VA_ARGS__)

#define PP_MAP(map, ctx, ...) PP_VA_NAME(PP_MAP_, __VA_ARGS__)(map, ctx, ##__VA_ARGS__)

/*!
 * calc argc
 */
#define __ARG_X(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, \
                v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, XX, ...)  \
    XX

/*!
 * calc argc
 */
#define ARG_ALPHABET(...)                                                                                              \
    __ARG_X("ignored", ##__VA_ARGS__, Z, Y, X, W, V, U, T, S, R, Q, P, O, N, M, L, K, J, I, H, G, F, E, D, C, B, A, z, \
            y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
/*!
 * calc argc
 */
#define ARG_CNT(...)                                                                                                   \
    __ARG_X("ignored", ##__VA_ARGS__, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,  \
            41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,    \
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
/*!
 * calc argc
 */
#define ARG_CNT_EXPAND(...)                                                                                            \
    __ARG_X("ignored", __VA_ARGS__, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,    \
            41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,    \
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/*!
 * calc argc FLOOR 2
 */
#define ARG_CNT_FLOOR2(...)                                                                                            \
    __ARG_X("ignored", ##__VA_ARGS__, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21,  \
            20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7,  \
            6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0)
/*!
 * calc argc ROUND 2
 */
#define ARG_CNT_ROUND2(...)                                                                                            \
    __ARG_X("ignored", ##__VA_ARGS__, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21,  \
            21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, \
            7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0)

#define __CON(A...) A

/* clang-format off */
#define __SEQ_NUM0(N, __pre, __pos, __spl, ext...)       error "[ SEQ_NUM0 ]: at least have one member, check function: __FUNCITON__ in file: __FILE__ at line: __LINE__"
#define __SEQ_NUM1(N, __pre, __pos, __spl, ext...)      __CON##__pre0__CON##__pos
// #define __SEQ_NUM2(N, __pre, __pos, __spl, ext...)      __SEQ_NUM1(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##1#__CON##__pos
#if 0
#define __SEQ_NUM3(N, __pre, __pos, __spl, ext...)      __SEQ_NUM2(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##2#__CON##__pos
#define __SEQ_NUM4(N, __pre, __pos, __spl, ext...)      __SEQ_NUM3(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##3#__CON##__pos
#define __SEQ_NUM5(N, __pre, __pos, __spl, ext...)      __SEQ_NUM4(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##4#__CON##__pos
#define __SEQ_NUM6(N, __pre, __pos, __spl, ext...)      __SEQ_NUM5(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##5#__CON##__pos
#define __SEQ_NUM7(N, __pre, __pos, __spl, ext...)      __SEQ_NUM6(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##6#__CON##__pos
#define __SEQ_NUM8(N, __pre, __pos, __spl, ext...)      __SEQ_NUM7(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##7#__CON##__pos
#define __SEQ_NUM9(N, __pre, __pos, __spl, ext...)      __SEQ_NUM8(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##8#__CON##__pos
#define __SEQ_NUM10(N, __pre, __pos, __spl, ext...)     __SEQ_NUM9(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##9#__CON##__pos
#define __SEQ_NUM11(N, __pre, __pos, __spl, ext...)     __SEQ_NUM10(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##10#__CON##__pos
#define __SEQ_NUM12(N, __pre, __pos, __spl, ext...)     __SEQ_NUM11(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##11#__CON##__pos
#define __SEQ_NUM13(N, __pre, __pos, __spl, ext...)     __SEQ_NUM12(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##12#__CON##__pos
#define __SEQ_NUM14(N, __pre, __pos, __spl, ext...)     __SEQ_NUM13(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##13#__CON##__pos
#define __SEQ_NUM15(N, __pre, __pos, __spl, ext...)     __SEQ_NUM14(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##14#__CON##__pos
#define __SEQ_NUM16(N, __pre, __pos, __spl, ext...)     __SEQ_NUM15(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##15#__CON##__pos
#define __SEQ_NUM17(N, __pre, __pos, __spl, ext...)     __SEQ_NUM16(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##16#__CON##__pos
#define __SEQ_NUM18(N, __pre, __pos, __spl, ext...)     __SEQ_NUM17(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##17#__CON##__pos
#define __SEQ_NUM19(N, __pre, __pos, __spl, ext...)     __SEQ_NUM18(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##18#__CON##__pos
#define __SEQ_NUM20(N, __pre, __pos, __spl, ext...)     __SEQ_NUM19(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##19#__CON##__pos
#define __SEQ_NUM21(N, __pre, __pos, __spl, ext...)     __SEQ_NUM20(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##20#__CON##__pos
#define __SEQ_NUM22(N, __pre, __pos, __spl, ext...)     __SEQ_NUM21(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##21#__CON##__pos
#define __SEQ_NUM23(N, __pre, __pos, __spl, ext...)     __SEQ_NUM22(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##22#__CON##__pos
#define __SEQ_NUM24(N, __pre, __pos, __spl, ext...)     __SEQ_NUM23(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##23#__CON##__pos
#define __SEQ_NUM25(N, __pre, __pos, __spl, ext...)     __SEQ_NUM24(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##24#__CON##__pos
#define __SEQ_NUM26(N, __pre, __pos, __spl, ext...)     __SEQ_NUM25(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##25#__CON##__pos
#define __SEQ_NUM27(N, __pre, __pos, __spl, ext...)     __SEQ_NUM26(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##26#__CON##__pos
#define __SEQ_NUM28(N, __pre, __pos, __spl, ext...)     __SEQ_NUM27(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##27#__CON##__pos
#define __SEQ_NUM29(N, __pre, __pos, __spl, ext...)     __SEQ_NUM28(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##28#__CON##__pos
#define __SEQ_NUM30(N, __pre, __pos, __spl, ext...)     __SEQ_NUM29(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##29#__CON##__pos
#define __SEQ_NUM31(N, __pre, __pos, __spl, ext...)     __SEQ_NUM30(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##30#__CON##__pos
#define __SEQ_NUM32(N, __pre, __pos, __spl, ext...)     __SEQ_NUM31(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##31#__CON##__pos
#define __SEQ_NUM33(N, __pre, __pos, __spl, ext...)     __SEQ_NUM32(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##32#__CON##__pos
#define __SEQ_NUM34(N, __pre, __pos, __spl, ext...)     __SEQ_NUM33(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##33#__CON##__pos
#define __SEQ_NUM35(N, __pre, __pos, __spl, ext...)     __SEQ_NUM34(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##34#__CON##__pos
#define __SEQ_NUM36(N, __pre, __pos, __spl, ext...)     __SEQ_NUM35(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##35#__CON##__pos
#define __SEQ_NUM37(N, __pre, __pos, __spl, ext...)     __SEQ_NUM36(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##36#__CON##__pos
#define __SEQ_NUM38(N, __pre, __pos, __spl, ext...)     __SEQ_NUM37(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##37#__CON##__pos
#define __SEQ_NUM39(N, __pre, __pos, __spl, ext...)     __SEQ_NUM38(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##38#__CON##__pos
#define __SEQ_NUM40(N, __pre, __pos, __spl, ext...)     __SEQ_NUM39(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##39#__CON##__pos
#define __SEQ_NUM41(N, __pre, __pos, __spl, ext...)     __SEQ_NUM40(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##40#__CON##__pos
#define __SEQ_NUM42(N, __pre, __pos, __spl, ext...)     __SEQ_NUM41(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##41#__CON##__pos
#define __SEQ_NUM43(N, __pre, __pos, __spl, ext...)     __SEQ_NUM42(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##42#__CON##__pos
#define __SEQ_NUM44(N, __pre, __pos, __spl, ext...)     __SEQ_NUM43(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##43#__CON##__pos
#define __SEQ_NUM45(N, __pre, __pos, __spl, ext...)     __SEQ_NUM44(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##44#__CON##__pos
#define __SEQ_NUM46(N, __pre, __pos, __spl, ext...)     __SEQ_NUM45(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##45#__CON##__pos
#define __SEQ_NUM47(N, __pre, __pos, __spl, ext...)     __SEQ_NUM46(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##46#__CON##__pos
#define __SEQ_NUM48(N, __pre, __pos, __spl, ext...)     __SEQ_NUM47(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##47#__CON##__pos
#define __SEQ_NUM49(N, __pre, __pos, __spl, ext...)     __SEQ_NUM48(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##48#__CON##__pos
#define __SEQ_NUM50(N, __pre, __pos, __spl, ext...)     __SEQ_NUM49(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##49#__CON##__pos
#define __SEQ_NUM51(N, __pre, __pos, __spl, ext...)     __SEQ_NUM50(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##50#__CON##__pos
#define __SEQ_NUM52(N, __pre, __pos, __spl, ext...)     __SEQ_NUM51(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##51#__CON##__pos
#define __SEQ_NUM53(N, __pre, __pos, __spl, ext...)     __SEQ_NUM52(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##52#__CON##__pos
#define __SEQ_NUM54(N, __pre, __pos, __spl, ext...)     __SEQ_NUM53(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##53#__CON##__pos
#define __SEQ_NUM55(N, __pre, __pos, __spl, ext...)     __SEQ_NUM54(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##54#__CON##__pos
#define __SEQ_NUM56(N, __pre, __pos, __spl, ext...)     __SEQ_NUM55(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##55#__CON##__pos
#define __SEQ_NUM57(N, __pre, __pos, __spl, ext...)     __SEQ_NUM56(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##56#__CON##__pos
#define __SEQ_NUM58(N, __pre, __pos, __spl, ext...)     __SEQ_NUM57(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##57#__CON##__pos
#define __SEQ_NUM59(N, __pre, __pos, __spl, ext...)     __SEQ_NUM58(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##58#__CON##__pos
#define __SEQ_NUM60(N, __pre, __pos, __spl, ext...)     __SEQ_NUM59(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##59#__CON##__pos
#define __SEQ_NUM61(N, __pre, __pos, __spl, ext...)     __SEQ_NUM60(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##60#__CON##__pos
#define __SEQ_NUM62(N, __pre, __pos, __spl, ext...)     __SEQ_NUM61(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##61#__CON##__pos
#define __SEQ_NUM63(N, __pre, __pos, __spl, ext...)     __SEQ_NUM62(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##62#__CON##__pos
#define __SEQ_NUM64(N, __pre, __pos, __spl, ext...)     __SEQ_NUM63(N, __pre, __pos, __spl, ext)#__CON##__spl#__CON##__pre##63#__CON##__pos
#endif
/* clang-format on */

/* clang-format off */
#if 0
#define __SEQ_NUM_DEC0(N, __pre, __pos, __spl, ext...)       #error "[ SEQ_NUM_DEC0 ]: at least have one member, check function: __FUNCITON__ in file: __FILE__ at line: __LINE__"
#define __SEQ_NUM_DEC1(N, __pre, __pos, __spl, ext...)     __CON##__pre##0#__CON##__pos
#define __SEQ_NUM_DEC2(N, __pre, __pos, __spl, ext...)     __CON##__pre##1#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC1(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC3(N, __pre, __pos, __spl, ext...)     __CON##__pre##2#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC2(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC4(N, __pre, __pos, __spl, ext...)     __CON##__pre##3#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC3(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC5(N, __pre, __pos, __spl, ext...)     __CON##__pre##4#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC4(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC6(N, __pre, __pos, __spl, ext...)     __CON##__pre##5#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC5(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC7(N, __pre, __pos, __spl, ext...)     __CON##__pre##6#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC6(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC8(N, __pre, __pos, __spl, ext...)     __CON##__pre##7#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC7(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC9(N, __pre, __pos, __spl, ext...)     __CON##__pre##8#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC8(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC10(N, __pre, __pos, __spl, ext...)    __CON##__pre##9#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC9(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC11(N, __pre, __pos, __spl, ext...)    __CON##__pre##10#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC10(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC12(N, __pre, __pos, __spl, ext...)    __CON##__pre##11#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC11(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC13(N, __pre, __pos, __spl, ext...)    __CON##__pre##12#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC12(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC14(N, __pre, __pos, __spl, ext...)    __CON##__pre##13#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC13(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC15(N, __pre, __pos, __spl, ext...)    __CON##__pre##14#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC14(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC16(N, __pre, __pos, __spl, ext...)    __CON##__pre##15#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC15(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC17(N, __pre, __pos, __spl, ext...)    __CON##__pre##16#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC16(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC18(N, __pre, __pos, __spl, ext...)    __CON##__pre##17#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC17(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC19(N, __pre, __pos, __spl, ext...)    __CON##__pre##18#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC18(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC20(N, __pre, __pos, __spl, ext...)    __CON##__pre##19#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC19(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC21(N, __pre, __pos, __spl, ext...)    __CON##__pre##20#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC20(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC22(N, __pre, __pos, __spl, ext...)    __CON##__pre##21#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC21(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC23(N, __pre, __pos, __spl, ext...)    __CON##__pre##22#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC22(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC24(N, __pre, __pos, __spl, ext...)    __CON##__pre##23#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC23(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC25(N, __pre, __pos, __spl, ext...)    __CON##__pre##24#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC24(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC26(N, __pre, __pos, __spl, ext...)    __CON##__pre##25#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC25(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC27(N, __pre, __pos, __spl, ext...)    __CON##__pre##26#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC26(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC28(N, __pre, __pos, __spl, ext...)    __CON##__pre##27#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC27(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC29(N, __pre, __pos, __spl, ext...)    __CON##__pre##28#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC28(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC30(N, __pre, __pos, __spl, ext...)    __CON##__pre##29#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC29(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC31(N, __pre, __pos, __spl, ext...)    __CON##__pre##30#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC30(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC32(N, __pre, __pos, __spl, ext...)    __CON##__pre##31#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC31(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC33(N, __pre, __pos, __spl, ext...)    __CON##__pre##32#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC32(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC34(N, __pre, __pos, __spl, ext...)    __CON##__pre##33#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC33(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC35(N, __pre, __pos, __spl, ext...)    __CON##__pre##34#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC34(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC36(N, __pre, __pos, __spl, ext...)    __CON##__pre##35#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC35(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC37(N, __pre, __pos, __spl, ext...)    __CON##__pre##36#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC36(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC38(N, __pre, __pos, __spl, ext...)    __CON##__pre##37#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC37(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC39(N, __pre, __pos, __spl, ext...)    __CON##__pre##38#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC38(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC40(N, __pre, __pos, __spl, ext...)    __CON##__pre##39#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC39(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC41(N, __pre, __pos, __spl, ext...)    __CON##__pre##40#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC40(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC42(N, __pre, __pos, __spl, ext...)    __CON##__pre##41#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC41(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC43(N, __pre, __pos, __spl, ext...)    __CON##__pre##42#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC42(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC44(N, __pre, __pos, __spl, ext...)    __CON##__pre##43#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC43(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC45(N, __pre, __pos, __spl, ext...)    __CON##__pre##44#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC44(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC46(N, __pre, __pos, __spl, ext...)    __CON##__pre##45#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC45(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC47(N, __pre, __pos, __spl, ext...)    __CON##__pre##46#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC46(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC48(N, __pre, __pos, __spl, ext...)    __CON##__pre##47#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC47(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC49(N, __pre, __pos, __spl, ext...)    __CON##__pre##48#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC48(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC50(N, __pre, __pos, __spl, ext...)    __CON##__pre##49#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC49(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC51(N, __pre, __pos, __spl, ext...)    __CON##__pre##50#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC50(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC52(N, __pre, __pos, __spl, ext...)    __CON##__pre##51#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC51(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC53(N, __pre, __pos, __spl, ext...)    __CON##__pre##52#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC52(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC54(N, __pre, __pos, __spl, ext...)    __CON##__pre##53#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC53(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC55(N, __pre, __pos, __spl, ext...)    __CON##__pre##54#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC54(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC56(N, __pre, __pos, __spl, ext...)    __CON##__pre##55#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC55(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC57(N, __pre, __pos, __spl, ext...)    __CON##__pre##56#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC56(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC58(N, __pre, __pos, __spl, ext...)    __CON##__pre##57#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC57(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC59(N, __pre, __pos, __spl, ext...)    __CON##__pre##58#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC58(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC60(N, __pre, __pos, __spl, ext...)    __CON##__pre##59#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC59(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC61(N, __pre, __pos, __spl, ext...)    __CON##__pre##60#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC60(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC62(N, __pre, __pos, __spl, ext...)    __CON##__pre##61#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC61(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC63(N, __pre, __pos, __spl, ext...)    __CON##__pre##62#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC62(N, __pre, __pos, __spl, ext)
#define __SEQ_NUM_DEC64(N, __pre, __pos, __spl, ext...)    __CON##__pre##63#__CON##__pos#__CON##__spl#__SEQ_NUM_DEC63(N, __pre, __pos, __spl, ext)
#endif
/* clang-format on */

#if 0
/** @brief CREAT SEQ BY GIVEN NUMBER(<= 64),
 * like N=5, you can get 0,1,2,3,4
 */
#define SEQ_NUM(N, __pre, __pos, __spl, ext...)     __SEQ_NUM##N(N, __pre, __pos, __spl, ext)

/** @brief CREAT DECREASE SEQ  BY GIVEN NUMBER (<= 64),
 * like N=5, you can get 4,3,2,1,0
 */
#define SEQ_NUM_DEC(N, __pre, __pos, __spl, ext...) __SEQ_NUM_DEC##N(N, __pre, __pos, __spl, ext)

#endif // 0

#if 0
SEQ_NUM_DEC(64, (_), (_, ))
SEQ_NUM_DEC(0, (_), (), (, ))

SEQ_NUM(2, (), (), (, ))
SEQ_NUM_DEC(64, (abc), (();), ())

ARG_CNT_EXPAND(SEQ_NUM_DEC(1, (), (), (, )))
ARG_CNT()
ARG_CNT(1)
ARG_CNT(1, 1)

RANG_MACRO(abc,1)
ARG_CNT()
ARG_CNT(1)
ARG_CNT(1, 1)
ARG_CNT_FLOOR2()
ARG_CNT_FLOOR2(1)
ARG_CNT_FLOOR2(1, 1)
ARG_CNT_FLOOR2(0, 1, 1)
ARG_CNT_FLOOR2(0, 1, 1)
ARG_CNT_FLOOR2(0, 0, 1, 1)
ARG_CNT_FLOOR2(0, 0, 0, 1, 1)

ARG_CNT_ROUND2()
ARG_CNT_ROUND2(1)
ARG_CNT_ROUND2(1, 1)
ARG_CNT_ROUND2(0, 1, 1)
ARG_CNT_ROUND2(0, 1, 1)
ARG_CNT_ROUND2(0, 0, 1, 1)
ARG_CNT_ROUND2(0, 0, 0, 1, 1)
#endif // 0

#endif //!__COMMANDS__H__