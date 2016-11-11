#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios CookedTermios;
struct termios RawTermios;

int main(void){
    int c;
    char euc_ch[3];
    
    //rawモードにする//ここから
    tcgetattr(STDIN_FILENO, &CookedTermios);
    
    RawTermios = CookedTermios;
    cfmakeraw(&RawTermios);

    tcsetattr(STDIN_FILENO, 0, &RawTermios);
    //ここまで//


    while((c = getchar()) != EOF){
        if(c >= 128){
            euc_ch[0] = c;
            if((c = getchar()) != EOF){
                euc_ch[1] = c;
                euc_ch[2] = '\0';
            }else{
                fprintf(stderr, "Input text isn't encoded in EUC code. \n");
                exit(2);
            }
        }else if(c == '\n'){
            continue;
        }else{
            euc_ch[0] = c;
            euc_ch[1] = '\0';
        }
        printf("%s\n", euc_ch);
    }
    //rawモード解除
    tcsetattr(STDIN_FILENO, 0, &CookedTermios);
}
