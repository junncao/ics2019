#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
WP *head = NULL;
static WP *free_ = NULL;

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

WP* new_wp(){
  if(!free_){
    printf("Wp_pool is empty!\n");
    assert(0);
  }
  WP *tmp = free_;
  free_ = free_->next;
  tmp->next = head;
  head = tmp;
  return tmp;
}

void free_wp(WP *wp){
  if(!wp)
    return;
  //delete wp from wp list
  int flag = 0;
  WP *tmp = head;
  for(;tmp;tmp = tmp->next, flag++){
    if(tmp == wp)
      break;
  }
  //haven't found wp, no need to delete
  if(!tmp)
    return;
  //wp is the first element in wp list
  if(flag == 0)
    head = head->next;
  else{
    tmp = head;
    for(int i = 0; i < flag-1; i++){
      tmp = tmp->next;
    }
    tmp->next = tmp->next->next;
  }
  wp->next = free_;
  free_ = wp;
}

