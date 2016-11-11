#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios CookedTermios;
struct termios RawTermios;
enum ChType { /*文字種*/
    ASCII,  /*ASCII文字*/
    HIRA,   /*ひらがな*/
    KATA,   /*カタカナ*/
    BAR,    /*横棒*/
    NUM,    /*数字*/
    ALPHA,  /*2byteアルファベット*/
    KANJI,  /*漢字*/
    DOT,    /*句読点*/
    OTHER     /*その他*/
};

/*
 * 文字タイプをチェックする
 *
 * return   enum ChType
 * arg      int c1, c2
 */
enum ChType checkChType(int c1, int c2);
void printfChType(enum ChType type);

int main(void){
    int c1;
    int c2;
    char euc_ch[3];
    enum ChType type = ASCII;
    
    
    //rawモードにする//ここから
    tcgetattr(STDIN_FILENO, &CookedTermios);
    
    RawTermios = CookedTermios;
    cfmakeraw(&RawTermios);

    tcsetattr(STDIN_FILENO, 0, &RawTermios);
    //ここまで//


    while((c1 = getchar()) != EOF){
        if(c1 >= 128){
            euc_ch[0] = c1;
            if((c2 = getchar()) != EOF){
                euc_ch[1] = c2;
                euc_ch[2] = '\0';
            }else{
                fprintf(stderr, "Input text isn't encoded in EUC code. \n");
                exit(2);
            }
        }else if(c1 == '\n'){
            continue;
        }else{
            euc_ch[0] = c1;
            euc_ch[1] = '\0';
        }
        printf("%s  ", euc_ch);
        printfChType(checkChType(c1, c2));
        printf("\n");
    }
    //rawモード解除
    tcsetattr(STDIN_FILENO, 0, &CookedTermios);
}



enum ChType checkChType(int c1, int c2){
        if(c1 < 128)
            return ASCII;
        if(c1 == 161){
            if (c2 == 188)
                return BAR;
            if(c2 == 162 || c2 == 163 ||
                    c2 == 169 || c2 == 170)
                return DOT;
            return OTHER;
        }
        if(c1 == 163){
            if (c2 >= 176 && c2 <= 185)
                return NUM;
            if (c2 >= 193 && c2 <= 250)
                return ALPHA;
            return OTHER;
        }
        if(c1 == 164){
            if (c2 >= 161 && c2 <= 243)
                return HIRA;
            return OTHER;
        }
        if(c1 == 165){
            if (c2 >= 161 && c2 <= 246)
                return KATA;
            return OTHER;;
        }
        if(c1 >= 176 && c1 <= 244){
            if (c2 >= 161 && c2 <= 254)
                return KANJI;
            return OTHER;
        }
    return OTHER;
}

void printfChType(enum ChType type){
    switch(type){
        case ASCII:
            printf("ASCII");
            break;
        case HIRA:
            printf("hiragana");
            break;
        case KATA:
            printf("katakana");
            break;
        case BAR:
            printf("yokobou");
            break;
        case NUM:
            printf("suuzi");
            break;
        case ALPHA:
            printf("alphabet");
            break;
        case KANJI:
            printf("kanji");
            break;
        case DOT:
            printf("dot");
            break;
        case OTHER:
            printf("other");
            break;
    }
}

    
    
