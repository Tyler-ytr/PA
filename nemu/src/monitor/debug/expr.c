#include "nemu.h"
#define COLORRED " \033[1;33m"
#define COLORNORMAL "\033[0m"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
enum {
  TK_NOTYPE = 256, TK_EQ,NAT_NUM,NEG

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},           //sub and neg
  {"\\*", '*'},         //multiply
  {"\\/", '/'},         //divide
  {"\\(", '('},         //left-bracket
  {"\\)", ')'},         //right-bracket
  {"[0-9]+",NAT_NUM} ,  //natural number
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* R333ules are used for many times.
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

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
	
	  	 /* if(nr_token>31){
			printf("\033[01;31m TOO MANY INPUTS!!!PLEASE LESS OR EQUAL THAN 32!!! \033[0m \n");
			return false;}
*/
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
			case TK_NOTYPE: break;
			case NAT_NUM:
					tokens[nr_token].type=rules[i].token_type;
					if(substr_len<32){memcpy(tokens[nr_token].str,substr_start,substr_len);}
					else{Log("Error");return false;}
					nr_token++;break;
			case '-':if(nr_token==0||((tokens[nr_token-1]).type!=NAT_NUM&&(tokens[nr_token-1]).type!=')'))
					 {tokens[nr_token].type=NEG;
					 nr_token++;break;
					 }
			case '+':;
			case '*':;
			case '/':;

	   		default: tokens[nr_token].type=rules[i].token_type;
					 nr_token++;
				   break;
        }
    
        break;
      }
	 // ++nr_token;
	 //plus
	 printf("nr_token: %d\n",nr_token);
	  // break;
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
//begin
bool check_parentheses(int p,int q){
	int i,par=0;
	if(tokens[p].type!='('||tokens[q].type!=')')
	return false;
	for(i=p;i<q;++i){
		if(tokens[i].type=='(')
		par++;
		else if(tokens[i].type==')')
		par--;
		if(par==0&&i<q)
		return false;
}
//plus
printf("out of check par\n");
	return true;
	}
  
int priority(int i){
switch (tokens[i].type){
	case (int)'+':return 5;
	case (int)'-':return 5;
	case (int)'*':return 2;
	case (int)'/':return 2;
	default:return -1;
}
}
  

	
uint32_t eval(int p,int q){
//plus
printf("orignal p=%d,q=%d",p,q);

	if(p>q){
		printf("overflow:p>q,p=%d,q=%d\n",p,q);
		assert(0);
	}
	else if(p==q){
//  plus
	printf("stuck in p==q");

	int tempresult=0;
	sscanf(tokens[p].str,"%d",&tempresult);
	return tempresult;

//	}
	}
	else if(check_parentheses(p,q)==true){

		return eval(p+1,q-1);
			}
	else if(tokens[p].type==NEG){
	return -eval(p+1,q);
	}
	 else {
int if_check=0;
int if_main=true;
int main_op=p;
for (int i=p;i<=q;i++)
{
	if_main=(if_check==0);
	if(tokens[i].type=='('){
		if_check++;}
	if(tokens[i].type==')'){
		if_check--;}
	if(if_main){
		if(tokens[i].type=='+'||tokens[i].type=='-'){
			main_op=i;
			break;
		}else if(tokens[i].type=='*'||tokens[i].type=='/'){
			main_op=i;
		}
		
	}
}


	//plus
	printf("stuck in op");
	printf("p=%d,q=%d",p,q);
	printf("mainop=%d\n",main_op);

		int val1=eval(p,main_op-1);
		//plus
		printf("val1=%d\n ",val1);
		int val2=eval(main_op+1,q);
		printf("val2=%d\n ",val2);
		switch(tokens[main_op].type){
			case '+':return val1+val2;
			case '-':return val1-val2;
			case '*':return val1*val2;
			case '/':return val1/val2;



			default:assert(0);
			}
		}
}

uint32_t expr(char *e, bool *success) {

  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
 // TODO();
  else{*success=true;
	  return eval(0,nr_token-1);
  }
  return 0;
}
