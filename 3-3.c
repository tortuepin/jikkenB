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
/* 情報を表示 */ 
void printWordInfo(wordinfo info);
int fgetline(char *buf, size_t size, FILE *stream);
/* 最長一致 */
int longestMatch(char *buf, int len, int dicsize, wordinfo* info[], int optnum);
/* バイト数チェック */
int checkBite(unsigned char c);
/* オプションチェック */
int checkOption(char* opt);

int main(int argc, char* argv[]){
    wordinfo *info[LINE];
    char buf[10000];
    char buf2[10000];
    char euc[10000];
    int i, len, hit, optnum;
    int dicsize;

    if(argc < 1){
        return -1;
    }else{
        optnum = checkOption(argv[1]);
    }

    if((dicsize=readDic(FILENAME, info)) < 0){
        printf("cannot read dic");
        return -1;
    }
    

    while(fgetline(buf, sizeof(buf), stdin) != -1){
        len = strlen(buf);
        hit = longestMatch(buf, len, dicsize, info, optnum);
        if (hit != -1){
            printWordInfo(*info[hit]);
        }

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
    for(i=0; i<size-1; i++){
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
    printf("%s\t%s\t%s\t%s\t%f\n", info.word, info.yomi, info.kihon, info.hinshi, info.cost);
}

int fgetline(char *buf, size_t size, FILE *stream){
    int len;
    if(fgets(buf, size, stream) == NULL) return -1;
    len = strlen(buf);
    if(len == 0) return len;
    if(buf[len-1] == '\n'){
        buf[len-1] = '\0';
        return len-1;
    } else {
        return len;
    }
}

int longestMatch(char *buf, int len, int dicsize, wordinfo* info[], int optnum){
    char *p = buf;
    char w[1000];
    unsigned char c;
    int i=0;
    int hit, k, hitnum=0;
    while(i < len){
        for(k=len; i<k; k--){
            strncpy(w, p+i, k-i);
            w[k-i] = '\0';
            hit = lookup(info, w, dicsize);
            if(hit != -1){
                break;
            }
            c = w[k-i-1];
            if(checkBite(c) == 2){
                k--;
            }
        }
        //見つかったらbufに残りを入れる
        //見つからなかったら一つ進める
        if(hit != -1){
            hitnum++;
            if(hitnum == optnum){
                return hit;
            }
            i = k;
        }else{
            i = i+1;
        }
    }
    return -1;
}

int checkBite(unsigned char c){
    if((int)c >= 128){
        return 2;
    }
    return 1;
}


int checkOption(char* opt){
    if(opt[0] == '-'){
        return atoi(&opt[1]);
    }
    return -1;
}
