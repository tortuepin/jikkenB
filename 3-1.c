#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 256 //一行
#define INFOLEN N*5
#define LINE  120000//辞書の行数
#define FILENAME "../dic/jdic.txt"

typedef struct {
    char word[N];
    char yomi[N];
    char kihon[N];
    char hinshi[N];
    double cost;
} wordinfo;

/* 辞書をinfoに読み込む 必ずfreeDicすること*/
int readDic(char* filename, wordinfo* info[]);
/* 辞書メモリを解放 */
void freeDic(wordinfo* info[], int size);
/* wordを検索 線形*/
int lookup(wordinfo* info[], char* word, int size);
/* wordを検索 二分*/
int binaryLookup(wordinfo* info[], char* word, int size);
/* 二分探索 */
int binarySearch(wordinfo* info[], char* word, int head, int tail);
/* 情報を表示 */ 
void printWordInfo(wordinfo info);



int main(void){
    wordinfo *info[LINE];
    char euc[10];
    int i, len, hit;


    int dicsize;
    if((dicsize=readDic(FILENAME, info)) < 0){
        printf("cannot read dic");
        return -1;
    }

    fgets(euc, 10, stdin);
    len=strlen(euc);
    if(euc[len-1] == '\n'){
        euc[len-1] = '\0';
    }
    hit = binaryLookup(info, euc, dicsize);


    if(hit > 0){
        printWordInfo(*info[hit]);
    }else{
        printf("mitsukaranai");
        return -1;
    }

    freeDic(info, dicsize);
}

int readDic(char* filename, wordinfo* info[]){
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
}

void freeDic(wordinfo* info[], int size){
    int i;
    for(i=0; i<size; i++){
        free(info[i]);
    }
}
int lookup(wordinfo* info[], char* word, int size){
    int i;
    for(i=0; i<size; i++){
        if(strcmp(info[i]->word, word) == 0){
            return i;
        }
    }
    return -1;
}


int binaryLookup(wordinfo* info[], char* word, int size){
    int i = (size-1)/2;
    int result, head, tail;
    head = 0;
    tail = size-1;

    while(1){
        result = strcmp(info[i]->word, word);

        if(head > tail){
            printf("head > tail");
            return -1;
        }
        if(result == 0){
            return i;
        }else if(result > 0){
            tail = i-1;
        }else if(result < 0){
            head = i+1;
        }
        i = (head+tail)/2;
    }
    printf("konai");
    return -1;
}

void printWordInfo(wordinfo info){
    printf("%s\t%s\t%s\t%s\t%f", info.word, info.yomi, info.kihon, info.hinshi, info.cost);
}

int binarySearch(wordinfo* info[], char* word, int head, int tail){
    int center = head+tail/2;
    int result = strcmp(info[center]->word, word);
    
    if(result > 0){
        if(center+1 == tail)
            return -1;
        return binarySearch(info, word, center+1, tail);
    }else if(result < 0){
        if(center-1 == head){
            return -1;
        }
        return binarySearch(info, word, head, center-1);
    }else{
        return center;
    }

}
