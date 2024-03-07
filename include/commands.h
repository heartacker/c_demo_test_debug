#ifndef __COMMANDS__H__
#define __COMMANDS__H__

#include "afe_co_int.h"

#include "afe_common.h"

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

#define __COMMAND_HANDLER_V1(name, extra...) int name(struct command_invocation *cmd, ##extra)
#define __COMMAND_HANDLER_V2(name, extra...) int name(const int argc, const char *argv[])

#if USE_ARGCV
/**
 * Command handlers may be defined with more parameters than the base
 * set provided by command.c.  This macro uses C99 magic to allow
 * defining all such derivative types using this macro.
 */
#define __COMMAND_HANDLER(name, extra...) __COMMAND_HANDLER_V2(name, extra)
#else
#define __COMMAND_HANDLER(name, extra...) __COMMAND_HANDLER_V1(name, extra)
#endif

typedef __COMMAND_HANDLER((*command_handler_t));

/**
 * Always use this macro to define new command handler functions.
 * It ensures the parameters are ordered, typed, and named properly, so
 * they be can be used by other macros (e.g. COMMAND_PARSE_NUMBER).
 * All command handler functions must be defined as static in scope.
 */
#define COMMAND_HANDLER(name) __COMMAND_HANDLER(name)

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
    //! subchain
    const char *usage;

    /**
     * If non-NULL, the commands in @c chain will be registered in
     * the same context and scope of this registration record.
     * This allows modules to inherit lists commands from other
     * modules.
     */
    const struct _command_registration *chain;
} command_registration;

#define DEFINE_REG_FUNC(module, func_name, feild, indexx, helpstr)                                                     \
    void register_commandhandler##_##indexx()                                                                          \
    {                                                                                                                  \
        feild[indexx].name = #module "_" #func_name;                                                                   \
        feild[indexx].handler = module##_##func_name;                                                                  \
        feild[indexx].help = #helpstr;                                                                                 \
    }

#define COMMAND_HANDLER2(thismodule, func_name, feild, indexx, helpstr)                                                \
    extern __COMMAND_HANDLER(thismodule##_##func_name);                                                                \
    DEFINE_REG_FUNC(thismodule, func_name, feild, indexx, helpstr)                                                     \
    __COMMAND_HANDLER(thismodule##_##func_name)

#define DEFINE_REG_FUNC_V2(thismodule, func_name, indexx, usagestr, helpstr, subchain)                                 \
    static void module##_##register_commandhandler##_##indexx()                                                        \
    {                                                                                                                  \
        thismodule##_commands[indexx].module = #thismodule;                                                            \
        thismodule##_commands[indexx].name = #func_name;                                                               \
        thismodule##_commands[indexx].handler = func_name;                                                             \
        thismodule##_commands[indexx].help = helpstr;                                                                  \
        thismodule##_commands[indexx].usage = usagestr;                                                                \
        thismodule##_commands[indexx].chain = subchain;                                                                \
    }

#define COMMAND_HANDLER_V2(thismodule, func_name, indexx, usagestr, helpstr, subchain)                                 \
    extern __COMMAND_HANDLER(func_name);                                                                               \
    DEFINE_REG_FUNC_V2(thismodule, func_name, indexx, usagestr, helpstr, subchain)                                     \
    __COMMAND_HANDLER(func_name)

#define DEFINE_REG_FUNC_V3(indexx, helpstr, subchain, usagestr, thismodule, func_name, extra...)                       \
    static void module##_##register_commandhandler##_##indexx()                                                        \
    {                                                                                                                  \
        thismodule##_commands[indexx].module = #thismodule;                                                            \
        thismodule##_commands[indexx].name = #thismodule "_" #func_name;                                               \
        thismodule##_commands[indexx].handler = thismodule##_##func_name;                                              \
        thismodule##_commands[indexx].help = helpstr;                                                                  \
        thismodule##_commands[indexx].usage = usagestr;                                                                \
        thismodule##_commands[indexx].chain = subchain;                                                                \
    }

#define COMMAND_HANDLER_V3(indexx, helpstr, fatherchain, usagestr, thismodule, func_name, extra...)                    \
    extern __COMMAND_HANDLER(thismodule##_##func_name);                                                                \
    DEFINE_REG_FUNC_V3(indexx, helpstr, fatherchain, usagestr, thismodule, func_name, extra)                           \
    __COMMAND_HANDLER(thismodule##_##func_name)

#define DEFINE_REG_FUNC_V4(indexx, subchain, helpstr, usagestr, thismodule, func_name, extra...)                       \
    static void module##_##register_commandhandler##_##indexx()                                                        \
    {                                                                                                                  \
        thismodule##_commands[indexx].module = #thismodule;                                                            \
        thismodule##_commands[indexx].name = #thismodule "_" #func_name;                                               \
        thismodule##_commands[indexx].handler = thismodule##_##func_name##_command_handler;                            \
        thismodule##_commands[indexx].help = helpstr;                                                                  \
        thismodule##_commands[indexx].usage = usagestr;                                                                \
        thismodule##_commands[indexx].chain = subchain;                                                                \
    }

#define COMMAND_HANDLER_V4(indexx, subchain, helpstr, usagestr, thismodule, func_name, full_func_name...)              \
    extern __COMMAND_HANDLER(thismodule##_##func_name##_command_handler);                                              \
    extern int thismodule##_##func_name();                                                                                       \
    DEFINE_REG_FUNC_V4(indexx, subchain, helpstr, usagestr, thismodule, func_name)                                     \
    __COMMAND_HANDLER(thismodule##_##func_name##_command_handler)                                                      \
    {                                                                                                                  \
        return thismodule##_##func_name();                                                                                       \
    }                                                                                                                  \
    int thismodule##_##func_name

#define ARGC (argc)
#define ARGV (argv)

/** Use this as the last entry in an array of command_registration records. */
#define COMMAND_REGISTRATION_DONE                                                                                      \
    {                                                                                                                  \
        .module = NULL, .name = NULL, .handler = NULL, .usage = NULL, .help = NULL, .chain = NULL                      \
    }

#define COMMAMDS_IS_NULL(thecmd) (thecmd->name == NULL || thecmd->handler == NULL)

#define START_REGISTE_CMD(funcname, fieldname, iid, ...)                                                               \
    int funcname##_##fieldname()                                                                                       \
    {                                                                                                                  \
                                                                                                                       \
        int __i = iid;                                                                                                 \
        command_registration *_thecmd = &fieldname[__i];                                                               \
        if (COMMAMDS_IS_NULL(_thecmd)) {                                                                               \
            return -1;                                                                                                 \
        }                                                                                                              \
        return 0;                                                                                                      \
    }

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
#endif //!__COMMANDS__H__