#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
  if(argc != 1){
    fprintf(2, "usage: pingpong\n");
    exit(1);
  }
  int p[2];
  pipe(p);
  if(fork() == 0){
    int pid_c = getpid();
    char str[5];
    read(p[0], str, 4);
    printf("%d: received %s\n", pid_c, str);
    write(p[1], "pong", 4);
    exit(0);
  }
  else{
    int pid_p = getpid();
    char str[5];
    write(p[1], "ping", 4);
    wait(0);
    read(p[0], str, 4);
    printf("%d: received %s\n", pid_p, str);
    close(p[0]);
    close(p[1]);
  }
  exit(0);
}
