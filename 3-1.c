#include<stdio.h>
#include<stdlib.h>

#define N 256 //一行
#define LINE 10000 //辞書の行数
#define FILENAME "../dic/jdic.txt"


int readDic(char* filename, char word[][N]);

int main(void){
    char word[LINE][N];
    printf("%d", readDic(FILENAME, word));

    printf("%s", word[20]);
    

}

int readDic(char* filename, char word[][N]){
    FILE *fp;
    int i;

    if((fp = fopen(filename, "r")) == NULL){
        printf("CAN NOT OPEN %s\n", filename);
        return -1;
    }

    for(i=0; i<LINE; i++){
        if(fgets(word[i], N, fp) == NULL){
            break;
        }
    }
    
    return i;
}
