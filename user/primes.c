#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc != 1){
        fprintf(2, "usage: prime\n");
        exit(1);
    }
    int p[2];
    pipe(p);
    if(fork() == 0){
        int p_read = p[0], p_write = -1;
        int now = 0, num = 0;
        while(1){
            int siz = read(p_read, &now, sizeof(now));
            if(siz == 0) break;
            if(num == 0){
                num = now;
                printf("prime: %d\n", num);
                continue;
            }
            else if(now % num == 0){
                continue;
            }
            if(p_write < 0){
                pipe(p);
                if(fork() == 0){ // only fork once for a process
                    p_read = p[0];
                }
                else{
                    p_write = p[1];
                }
            }
            if(p_write >= 0){
                write(p_write, &now, sizeof(now));
            }
        }
        if(p_write >= 0){
            wait(0);
        }
        if(p_read >= 0){
            close(p_read);
        }
        if(p_write >= 0){
            close(p_write);
        }
        exit(0);
    }
    else{
        int i;
        for(i = 2; i < 35; i++){
            //printf("write %d\n", i);
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}