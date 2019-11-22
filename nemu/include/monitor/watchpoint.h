#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  int last_value;
  char wp_expr[100];

  /* TODO: Add more members if necessary */


} WP;

void init_wp_pool();
WP* new_wp();
void free_wp(WP*);

#endif
