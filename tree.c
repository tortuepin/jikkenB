#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define MASK0 255
#define MASK8 128
#define MASK7 64
#define MASK6 36
#define MASK5 16
#define MASK4 8
#define MASK3 4
#define MASK2 2
#define MASK1 1

#define N 256
#define INFOLEN N*5
#define LINE 120000
#define FILENAME "../dic/jdic.txt"


typedef struct {
    char word[N];
    char yomi[N];
    char kihon[N];
    char hinshi[N];
    double cost;
} wordinfo;

// 辞書を読み込む
int readDic(char* filename, wordinfo* info[]){ // {{{
    FILE *fp;
    int i, k;
    char word[INFOLEN];

    if((fp = fopen(filename, "r")) == NULL){
        printf("CAN NOT OPEN %s\n", filename);
        return -1;
    }

    for(i=0; i<LINE; i++){
        info[i] = (wordinfo *)malloc(sizeof(wordinfo));
        if(fgets(word, N, fp) == NULL){
            break;
        }
        strcpy(info[i]->word, strtok(word, " \t "));
        strcpy(info[i]->yomi, strtok(NULL, " \t "));
        strcpy(info[i]->kihon, strtok(NULL, " \t "));
        strcpy(info[i]->hinshi, strtok(NULL, " \t "));
        info[i]->cost = atof(strtok(NULL, " \t "));
    }
    
    return i;
} // }}}
// 辞書の解放
void freeDic(wordinfo* info[], int size){ //{{{
    int i;
    for(i=0; i<size-1; i++){
        free(info[i]);
    }
} //}}}
// ビット表示
 void show_signed_char(signed char x) // {{{
 {
   int i;
 
   printf("%4d  %02x  ", x, (unsigned char)x);
   for (i = 7; i >= 0; i--) {
     printf("%d", (x>>i) & 1);
   }
   printf("\n");
 } //}}}
// cのnビット目が何か
 int checkMask(unsigned char c, int n){ //{{{
     switch (n) {
         case 1:
             if((c & MASK1) == 0){
                 return 0;
             }else{
                return 1;
            }
            break;
        case 2:
            if((c & MASK2) == 0){
                return 0;
            }else{
                return 1;
            }
            break;

        case 3:
            if((c & MASK3) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 4:
            if((c & MASK4) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 5:
            if((c & MASK5) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 6:
            if((c & MASK6) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 7:
            if((c & MASK7) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 8:
            if((c & MASK8) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
    }
    return -1;
} //}}}
// cのnビット目をkにする
unsigned char setBit(unsigned char c, int n, int k){ //{{{
    switch(n){
        case 1:
            if(k == 0){
                c &= ~MASK1;
            }else{
                c |= MASK1;
            }
            break;
        case 2:
            if(k == 0){
                c &= ~MASK2;
            }else{
                c |= MASK2;
            }
            break;
        case 3:
            if(k == 0){
                c &= ~MASK3;
            }else{
                c |= MASK3;
            }
            break;
        case 4:
            if(k == 0){
                c &= ~MASK4;
            }else{
                c |= MASK4;
            }
            break;
        case 5:
            if(k == 0){
                c &= ~MASK5;
            }else{
                c |= MASK5;
            }
            break;
        case 6:
            if(k == 0){
                c &= ~MASK6;
            }else{
                c |= MASK6;
            }
            break;
        case 7:
            if(k == 0){
                c &= ~MASK7;
            }else{
                c |= MASK7;
            }
            break;
        case 8:
            if(k == 0){
                c &= ~MASK8;
            }else{
                c |= MASK8;
            }
            break;
    }
    return c;
} //}}}
// バイト数チェック
int checkBite(unsigned char c){ // {{{
    if((int)c >= 128){
        return 2;
    }
    return 1;
} //}}}

// 辞書ファイルから重複がないようにn文字目をとってくる
// 必ずfreeExtractedすること
int extractChar(wordinfo *dic[], int n, char** c, int dicsize, int head, int tail){ //{{{
    int i;
    int biteSize;
    int k=0;
    char tmp[2];

    
    for(i=head; i<tail+1; i++){
        
        biteSize = checkBite(dic[i]->word[0]);

        strncpy(tmp, dic[i]->word, sizeof(char)*2);
        //もし重複していなかったらc[k]にこぴー
        if(k==0 || strcmp(c[k-1], tmp) != 0){
            c[k] = (char*)malloc(sizeof(char)*2);
            strncpy(c[k], tmp, sizeof(char)*2);
            k++;
        }
    }
    printf("%d", k);

    return k;

} //}}}
// extractedCharのcを開放する
freeExtracted(char **c, int size){ //{{{
    int i;
    for(i=0; i<size; i++){
        free(c[i]);
    }
} //}}}


int main(void){
    wordinfo *info[LINE];
    int dicsize;
    int a;
    char *c[LINE];




    if((dicsize=readDic(FILENAME, info)) < 0){
        printf("cannot read dict");
        return -1;
    }

    a = extractChar(info, 0, c, dicsize, 0, 10);
    printf("hoge = %s ", c[5]);
    freeExtracted(c, a);


    printf("hoge = %s ", c[5]);


    freeDic(info, dicsize);
    
    return 0;
}


// vim: set foldmethod=marker :