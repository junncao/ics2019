int init_monitor(int, char *[]);
void expr_test();
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  expr_test();
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}

//void expr_test(){
//  int result;
//  int mresult;
//  char expr[1000];
//  bool success;
//  FILE *fp = fopen("../tools/gen-expr/input", "r");
//  while(sscanf(fp, "%d %s", &result, expr) != EOF){
//    mresult = expr(expr, &success);
//    if(mresult == result && success){
//      printf("OK!\n");
//    }
//    else{
//      printf("BAD!\n");
//    }
//  }
//}
