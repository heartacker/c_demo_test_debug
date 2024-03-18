
#include "stdio.h"
#include "string.h"

#include "commands.h"

#if defined(PC_LINUX)
#define printf(aaa...) printf(aaa)
#else // defined(FW)
#include "uart.h"
#define printf(aaa...) yc_printf(aaa)
#endif // PC_LINUX

#define DEBUG_CMD_PRINT(aaa...)
#ifndef DEBUG_CMD_PRINT
#define DEBUG_CMD_PRINT(aaa...) printf(aaa...)
#endif // DEBUG_CMD_PRINT

void display_commands(command_registration const *commands, int argc, char *argv[], int level)
{
    command_registration *thandler = (command_registration *)commands;
    while (!IS_COMMANDP_NULL(thandler)) {
        if (thandler->chain) {
            for (int i = 0; i < level - 1; i++) {
                printf("  ");
            }
            if (level > 0)
                printf("  >");
            printf("%s.%s\n", thandler->module, thandler->name);
            display_commands(thandler->chain, argc, argv, level + 1);
        } else {
            printf("  ");
            for (int i = 0; i < level - 1; i++) {
                printf("  ");
            }

            // printf("%s", thandler->module);
            printf(".%s", thandler->name);
            // printf("\t%p", thandler->handler);
            // printf("\t%d", 0x1000_2000);
            printf("\t%s", thandler->usage);
            printf("\t%s", thandler->help);
            printf("\t%p", thandler->chain);
            printf("\n");
            // thandler->handler(argc, argv);
        }
        thandler++;
    }
}

void goto_commands(command_registration const *commands, command_registration **targcommand, int argc, char *argv[],
                   int level)
{
    // int cnt = ARGC;
    // const char **argv = ARGV;

    // printf("file: %s, line: %d\n", __FILE__, __LINE__);
    // DPRINT_FUNCNAME(AAA);

    command_registration *iterhandler = (command_registration *)(commands);
    command_registration *chainiterhandler = NULL;

    if (*argv) {
        DEBUG_CMD_PRINT("$ %s\n", *argv);
        DEBUG_CMD_PRINT(" # Current Block: %s\n", iterhandler->module);
        DEBUG_CMD_PRINT("START SEARCH======\n");

        // check all block
        while (!IS_COMMANDP_NULL((iterhandler))) {
            DEBUG_CMD_PRINT(" # Current Block: %s\n", iterhandler->module);
            if ((iterhandler)->chain && (0 == strcmp((iterhandler)->module, *argv))) {
                (iterhandler) = (iterhandler)->chain;
                goto_commands((iterhandler), &chainiterhandler, argc - 1, argv + 1, level + 1);
                if (!IS_COMMANDP_NULL(chainiterhandler)) {
                    goto RETURNCURRENT;
                }
            }
            (iterhandler)++;
        }

        // check all function

        iterhandler = (command_registration *)(commands);

        while (!IS_COMMANDP_NULL((iterhandler))) {
            DEBUG_CMD_PRINT(" - Current Funct: %s\n", iterhandler->name);

            if (iterhandler->chain == NULL && 0 == strcmp((iterhandler)->name, *argv)) { // function
                DEBUG_CMD_PRINT("execute: %s.%s\n", (iterhandler)->module, (iterhandler)->name);
                (iterhandler)->rev = CLR_BEFORE_EXE;
                if (iterhandler->handler) {
                    (iterhandler)->rev = (iterhandler)->handler(argc, argv);
                } else {
                    (iterhandler)->rev = FIND_NULL_EXE;
                }
                chainiterhandler = iterhandler;
                goto RETURNCURRENT;
            }
            iterhandler++;
        }

        // check all subchain
        iterhandler = (command_registration *)(commands);

        while (!IS_COMMANDP_NULL((iterhandler))) {

            if ((iterhandler)->chain) {
                DEBUG_CMD_PRINT(" > Current Chain: %s\n", iterhandler->name);
                if (0 == strcmp((iterhandler)->name, *argv)) {
                    goto_commands((iterhandler)->chain, &chainiterhandler, argc - 1, argv + 1, level + 1);
                } else {
                    goto_commands((iterhandler)->chain, &chainiterhandler, argc, argv, level + 1);
                }
                if (IS_COMMANDP_NULL((chainiterhandler))) {
                    (iterhandler)++;
                    continue;
                } else {
                    goto RETURNCURRENT;
                }
            }
            iterhandler++;
        }

#if 0
        // check all UPPERCHAIN
        iterhandler = (command_registration *)(commands);

        while (!IS_COMMANDP_NULL((iterhandler))) {

            if ((iterhandler)->upperchain) {
                DEBUG_CMD_PRINT("## %s\n", (iterhandler)->name);
                goto_commands((iterhandler)->upperchain, &chainiterhandler, argc, argv, level + 1);
                if (IS_COMMANDP_NULL((chainiterhandler))) {
                    (iterhandler)++;
                    continue;
                } else {
                    goto RETURNCURRENT;
                }
            }
            (iterhandler)++;
        }
#endif // 0
    }

RETURNCURRENT:
#if 0
    if (IS_COMMANDP_NULL((chainiterhandler))) {
        if (level == 0) {
            printf("cannot find the command: %s\n", *argv);
            display_commands(iterhandler, argc, argv, 0);
        }
    }
#endif // 0

    *targcommand = chainiterhandler;
}

#undef printf
