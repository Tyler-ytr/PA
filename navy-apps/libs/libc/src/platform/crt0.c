#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;

__attribute__((section(".text.unlikely"))) void _start(int argc, char *argv[], char *envp[]) {
//  environ = envp;
  //exit(main(argc, argv, envp));
  char *env[]={NULL};
  environ=env;
  printf("I am in _start\n");
  exit(main(argc,argv,env));
  assert(0);
}
