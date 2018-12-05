#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <string.h>
#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
    int i;
    for (i = 0; i < NR_WP; i ++) {
        wp_pool[i].NO = i;
        wp_pool[i].next = &wp_pool[i + 1];
    }
    wp_pool[NR_WP - 1].next = NULL;

    head = NULL;
    free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *expr, uint32_t value) {
    // printf("#1\n");
    if (free_ == NULL) {
        printf("the watchpoint pool has been full!\n");
        assert(0);
    }
    // printf("#2\n");
    WP* n_wp = NULL;
    // n_wp = (WP*)malloc(sizeof(WP));
    n_wp = free_;
    free_ = free_->next;
    // printf("#2.5\n"); 
    n_wp->next = NULL;
    // printf("#2.6\n");
    n_wp->nv = value;
    // printf("#2.7\n");
    strcpy(n_wp->expr, expr);

    // printf("#3\n");
    if (head != NULL) {
        n_wp->next = head;
    }
    head = n_wp;

    return n_wp;
}

void free_wp(char *expr) {
    if (head == NULL) {
        printf("there doesn't exist any watchpoint!\n");
        return;
    }

    WP* p_wp = head;
    if (!strcmp(p_wp->expr, expr)) {
        printf("delete %d-th watchapoint '%s'\n", p_wp->NO, p_wp->expr);
        head = head->next;
        memset(p_wp->expr, '\0', sizeof(p_wp));
        p_wp->nv = p_wp->ov = 0;
        p_wp->next = free_;
        free_ = p_wp;
        return;
    }

    while(p_wp->next != NULL) {
        if (!strcmp(p_wp->next->expr, expr)) {
            printf("delete %d-th watchapoint '%s'\n", p_wp->next->NO, p_wp->next->expr);
            WP *tmp = p_wp->next;
            p_wp->next = tmp->next;
            memset(tmp->expr, '\0', sizeof(tmp));
            tmp->nv = tmp->ov = 0;
            tmp->next = free_;
            free_ = tmp;
            return;
        } else {
            p_wp = p_wp->next;
        }
    }

    if (p_wp->next == NULL) {
        printf("delete the watchpoint failed! please check the expression.\n");
    }
    return;
}

WP* check_watchpoint(WP *p_wp) {
    if (p_wp == NULL) {
        p_wp = head;
    }
    while (p_wp != NULL) {
        bool success = true;
        uint32_t ans = expr(p_wp->expr, &success);
        if (!success) {
            printf("exprission compute failed!\n");
            assert(0);
        } else if (ans != p_wp->nv) {
            p_wp->ov = p_wp->nv;
            p_wp->nv = ans;
            return p_wp;
        }
        p_wp = p_wp->next;
    }
    return NULL;
}
 
void print_watchpoint(void) {
     WP *p = head;
     if (p != NULL) {
        printf("Num   address        value          old_value\n");
     } else {
         printf("no watchpoint setted!\n");
         return;
     }
     while (p != NULL) {
         printf("%-6d%-15s%-15u%-15u\n", p->NO, p->expr, p->nv, p->ov);
         p = p->next;
     }
     return;
}
