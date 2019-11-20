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
