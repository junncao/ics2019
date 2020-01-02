#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern uint32_t isa_reg_str2val(const char *s, bool *success);
int calculate(int i, int j, bool *success);
bool check_parentheses(int i, int j);

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_HEXNUM,
  TK_REG, TK_NEQ, TK_AND, DEREF,REV

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\$[\\$0-9a-zA-z]+", TK_REG}, //registers: Can be more specific
  {"0x[0-9a-fA-F]+", TK_HEXNUM},  //Hex number: Must before number
  {"[0-9]+", TK_NUM}, //numbers
  {"-", '-'},   // minus
  {"\\*", '*'}, // multiply
  {"/", '/'},   // slide
  {"[' ']+", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},         //  equal
  {"!=", TK_NEQ},
  {"&&", TK_AND},
  {"\\(", '('},
  {"\\)", ')'},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        /*
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        */

        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_REG:
          case TK_NUM:
          case TK_HEXNUM:
            if(substr_len >= 32){
              printf("number is too big!\n");
              assert(0);
            }
            for(int j = 0; j < substr_len; j++){
              tokens[nr_token].str[j] = substr_start[j];
            }
            tokens[nr_token].str[substr_len] = 0;
          default:
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
          case TK_NOTYPE:
            break;
      }
      break;
    }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  for(int i = 0; i < nr_token; i++){
    if(tokens[i].type == '*' && (i==0 || (tokens[i-1].type == '+')|| (tokens[i-1].type == '-')|| (tokens[i-1].type == '(') || (tokens[i-1].type == '*'))){
      tokens[i].type = DEREF;
    }
    if(tokens[i].type == '-' && (i==0 || (tokens[i-1].type == '+')|| (tokens[i-1].type == '-')|| (tokens[i-1].type == '('))){
      tokens[i].type = REV;
    }
  }
  return calculate(0, nr_token-1, success);
}

int calculate(int i, int j, bool *success){
  if(i > j){
    *success = false;
    return 0;
  }
  else if(i == j){
    if(tokens[i].type != TK_NUM && tokens[i].type != TK_HEXNUM && tokens[i].type != TK_REG){
      *success = false;
      return 0;
    }
    int number;
    *success = true;

    if(tokens[i].type == TK_REG)
      number = isa_reg_str2val(&(tokens[i].str[1]), success);
    else if(tokens[i].type == TK_NUM)
      sscanf(tokens[i].str, "%d", &number);
    else
      sscanf(&(tokens[i].str[2]), "%x", &number); //delete "0x"
    //printf("%s number: %d\n", tokens[i].str, number);
    return number;
  }
  /*
  else if(tokens[i].type == DEREF){
    int addr = calculate(i+1, j, success);
    if(!(*success)){
      return 0;
    }
    return paddr_read(addr, 4);
  }
  */
  else if(check_parentheses(i,j)){
    return calculate(i+1, j-1, success);
  }
  else{
    //printf("search op\n");
    //printf("i = %d, j = %d\n", i, j);
    //printf("type = %d, * = %d\n", tokens[1].type, '*');
    int bracketNum = 0, op = -1; // op is the position of main opcode
    int flag = 5; // flag is 1 only when the main opcode is * or /
    bool success1, success2;
    int value1, value2;
    // &&  <   !=,==  <  *,/  <  +,-
    for(int k = i; k <=j; k++){
      if(tokens[k].type == '('){
        bracketNum++;
      }
      else if(tokens[k].type == ')'){
        bracketNum--;
      }
      else if(bracketNum == 0){
        if(tokens[k].type == '+' || tokens[k].type == '-'){
          op = k;
          flag = 1;
        }
        if(flag>1 && (tokens[k].type == '*' || tokens[k].type == '/')){
          op = k;
          flag = 2;
        }
        if(flag > 2 && (tokens[k].type == TK_EQ || tokens[k].type == TK_NEQ)){
          op = k;
          flag = 3;
        }
        if(flag == 4 && (tokens[k].type == TK_AND)){
          op = k;
          flag = 4;
        }
        if(flag == 5 &&(tokens[k].type == DEREF)){
            op = k;
            flag = 5;
        }
        if(flag == 6 &&(tokens[k].type == REV)){
            op = k;
            flag = 6;
        }
      }
    }

    if(op == -1){
      *success = false;
      return 0;
    }

    if(flag == 5){
        int addr = calculate(i+1, j, success);
        if(!(*success)){
          return 0;
        }
        return paddr_read(addr, 4);
    }

    if(flag == 6){
        int val = calculate(i+1, j, success);
        if(!(*success)){
          return 0;
        }
        return -val;
    }

    value1 = calculate(i, op-1, &success1);
    value2 = calculate(op+1, j, &success2);
    if(!success1 || !success2){
      *success = false;
      return 0;
    }
    *success = true;
    switch(tokens[op].type){
      case '+': return value1 + value2;
      case '-': return value1 - value2;
      case '*': return value1*value2;
      case '/':
        if(value2 == 0){
          printf("Divide 0!\n");
          assert(0);
        }
        return value1/value2;
      case TK_EQ: return (value1 == value2);
      case TK_NEQ: return (value1 != value2);
      case TK_AND: return (value1 && value2);
      default: assert(0);
    }
  }
}

bool check_parentheses(int i, int j){
  int bracketNum = 0;
  if(tokens[i].type != '(' || tokens[j].type != ')')
    return false;
  for(int k = i+1; k < j;k++){
    if(bracketNum < 0)
      return false;
    if(tokens[k].type == '('){
      bracketNum++;
    }
    if(tokens[k].type == ')'){
      bracketNum--;
    }
  }
  if(bracketNum != 0)
    return false;
  return true;
}
