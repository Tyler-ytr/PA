#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
    static char *line_read = NULL;

    if (line_read) {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("(nemu) ");

    if (line_read && *line_read) {
        add_history(line_read);
    }

    return line_read;
}

static int cmd_c(char *args) {
    cpu_exec(-1);
    return 0;
}

static int cmd_q(char *args) {
    return -1;
}

static int cmd_si(char *args) {
    int i = 0;
    if (args == NULL) {
        i = 1;
    } else {
        sscanf(args, "%d", &i);
    }
    cpu_exec(i);
    return 0;    
}

static int cmd_info(char *args) {
    if (args == NULL) {
        printf("'r' -- registers or 'w' -- watchpoint\n");
        return 0;
    } else if (!strcmp(args, "r")) {
        printf("EAX: 0x%08x \n", cpu.eax);
        printf("ECX: 0x%08x \n", cpu.ecx);
        printf("EDX: 0x%08x \n", cpu.edx);
        printf("EBX: 0x%08x \n", cpu.ebx);
        printf("ESP: 0x%08x \n", cpu.esp);
        printf("EBP: 0x%08x \n", cpu.ebp);
        printf("ESI: 0x%08x \n", cpu.esi);
        printf("EDI: 0x%08x \n", cpu.edi);
        printf("EIP: 0x%08x \n", cpu.eip);
    } else if (!strcmp(args, "w")) {
        print_watchpoint();
    }
    return 0;
}

static int cmd_x(char *args) {
    if (args == NULL) {
        printf("please input n and expr!\n");
        return 1;
    }
    char *n = strtok(args, " ");
    if (n == NULL) {
        printf("please input n!\n");
        return 1;
    }
    char *exp = n + strlen(n) + 1;
    if (exp == NULL) {
        printf("please input addr!\n");
        return 1;
    }
    int i = 0;
    bool success = true;
    uint32_t addr = expr(exp, &success);
    if (!success) {
        printf("expression computation failed!\n");
        return 1;
    }
    sscanf(n, "%d", &i);
    for (int k = 0; k < i; k++) {
        uint32_t value = vaddr_read(addr + k * 4, 4);
        printf("0x%08x: ", addr + k * 4);
        for (int j = 0; j < 4; j++) {
            printf("0x%02x ", value & 0xff);
            value = value >> 8;
        }
        printf("\n");
    }
    return 0;
}

static int cmd_p(char *args) {
    bool success = true;
    uint32_t ans = 0;
    ans = expr(args, &success);
    if (!success) {
        printf("correct expression is needed!\n");
    } else {
        printf("the value is %u\n", ans);
    }
    return 0;
}

static int cmd_w(char *args) {
    if (args == NULL) {
        printf("please write the expression!\n");
        return 0;
    } else {
        bool success = true;
        uint32_t ans = expr(args, &success);

        if (!success) {
            printf("correct expression is needed!\n");
        } else {
            // printf("#success\n");
            WP* n_wp = new_wp(args, ans);
            printf("the %d-th watchpoint was added, '%s' store the value: %d\n", 
                    n_wp->NO, n_wp->expr, n_wp->nv);
        }
        return 0;
    }
}

static int cmd_d(char *args) {
    if (args == NULL) {
        printf("please write the expression!\n");
        return 0;
    } else if (strlen(args) > 65530) {
        printf("the expression is too long!\n");
        return 0;
    }
    free_wp(args);
    return 0;
}

static int cmd_help(char *args);

static struct {
    char *name;
    char *description;
    int (*handler) (char *);
} cmd_table [] = {
    { "help", "Display informations about all supported commands", cmd_help },
    { "c", "Continue the execution of the program", cmd_c },
    { "q", "Exit NEMU", cmd_q },
    { "si", "excute the same command for n times", cmd_si },
    { "info", "print the state of the program", cmd_info },
    { "x", "examine the memory", cmd_x },
    { "p", "solve the value of the expression", cmd_p },
    { "w", "set the watchpoint", cmd_w },
    { "d", "delete the watchpoint", cmd_d },
    /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL) {
        /* no argument given */
        for (i = 0; i < NR_CMD; i ++) {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else {
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(arg, cmd_table[i].name) == 0) {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}

void ui_mainloop(int is_batch_mode) {
    
    if (is_batch_mode) {
       cmd_c(NULL);
       return;
    }
     
    while (1) {
        char *str = rl_gets();
        char *str_end = str + strlen(str);
        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL) { continue; }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL;
        }

#ifdef HAS_IOE
        extern void sdl_clear_event_queue(void);
        sdl_clear_event_queue();
#endif

        int i;
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) { return; }
                break;
            }
        }

        if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
    }
}
