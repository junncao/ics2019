#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
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

static int cmd_si(char *args){
  int lineNum = 1;
  if(args){
    sscanf(args, "%d", &lineNum);
  }
  cpu_exec(lineNum);
  return 0;
}

static int cmd_info(char *args){
  if(args){
    if(args[0] == 'r'){
      isa_reg_display();
    }
    else if(args[0] == 'w'){
      //TODO
    }
    else{
      printf("Wrong argument!\n");
    }
  }
  else{
    printf("Lack argument!\n");
  }
  return 0;
}

static int cmd_x(char *args){
  int number, index;
  number = index = -1;
  sscanf(args, "%d 0x%x", &number, &index);
  if(index == -1 || number <= 0 ){
    printf("Wrong argument!\n");
    return 0;
  }
  for(int i = 0; i < number; i++){
    if(i%4 == 0){
      printf("0x%-10x:", index + i*4);
    } 
    printf("0x%-10x", paddr_read(index + i*4, 4)); 
    if( ((i-3)%4 == 0) && (i != number-1))
      printf("\n");
  }
  printf("\n");
  return 0;
}

static int cmd_p(char *args){
  bool success; 
  int res;
  res = expr(args, &success);
  if(!success){
    printf("Wrong experssion!\n");
    return 0;
  }
  printf("%d\n", res);
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
  { "si", "Execute N steps", cmd_si },
  { "info", "Show the information of register", cmd_info },
  { "x", "Show the details of memory", cmd_x },
  { "p", "Print the result of an experssion", cmd_p },

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

  for (char *str; (str = rl_gets()) != NULL; ) {
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
