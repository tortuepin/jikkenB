#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

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
/* コスト最小 */
void minCostMatch(char *buf, int len, int dicsize, wordinfo* info[]);
/* バイト数チェック */
int checkBite(unsigned char c);
/* オプションチェック */
int checkOption(char* opt);

int main(int argc, char* argv[]){
    wordinfo *info[LINE];
    char buf[10000];
    int len, hit, optnum;
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
        minCostMatch(buf, len, dicsize, info);
        //hit = longestMatch(buf, len, dicsize, info, optnum);
        //if (hit != -1){
        //    printWordInfo(*info[hit]);
        //}

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


void minCostMatch(char *buf, int len, int dicsize, wordinfo* info[]){
    double minCost[1000];
    int minWordNum[1000];
    char tmpWord[1000];
    double wordCost[1000][50];
    int lattice[1000][50];//[文字位置][単語数]
    int wordLen[1000][50];//lattice[n][i]の単語の長さ
    int latticeNum[1000];//lattice[n]に何個単語が入ってるか
    int i, k, hit, j=0;
    int place[1000];
    int p=0, kp=0;
    double tmpCost;
    int revans[1000];
    int ans[1000];

    //初期化
    //ラティスを作る
    //printf("len = %d ", len);
    for(i=0; i<len; i++){
        if(checkBite(buf[i]) == 2){
            i++;
        }
        place[p] = i;
        j=0;
        kp=0;
        for(k=0; k<=i; k++){
            //kからiまでの単語があるかどうか調べる
            strncpy(tmpWord, buf+k, i-k+1);
            tmpWord[i-k+1] = '\0';
            hit = binaryLookup(info, tmpWord, dicsize);
            //あったらその辞書番号とコストを保存
            if(hit != -1){
                lattice[p][j] = hit;
                wordCost[p][j] = info[hit]->cost;
                wordLen[p][j] = p-kp+1;
                j++;
                //printf("lettice %d %d = ", p, j-1);
                //printWordInfo(*info[hit]);
            }
            if(checkBite(buf[i]) == 2){
                k++;
            }
            kp++;
        }
        latticeNum[p] = j;
        p++;
    }
    //ここまでで初期化完了
    //最小を見つける
    for(i=0; i<=p; i++){
        minCost[i] = INT_MAX;
        for(k=0; k<latticeNum[i]; k++){
            tmpCost = wordCost[i][k];
            if(i-wordLen[i][k]>=0)tmpCost += minCost[i-wordLen[i][k]];
            if(tmpCost < minCost[i] || minCost[i] < 0){
                minCost[i] = tmpCost;
                minWordNum[i] = k;
            }
        }
    }
    
    //バックトラック
    i = p-1;
    k = 0;
    while(1){
        revans[k] = lattice[i][minWordNum[i]];
        i -= wordLen[i][minWordNum[i]];
        if(i<0)break;
        k++;
    }

    for(i=0; i<=k; i++){
        ans[i] = revans[k-i];
        printWordInfo(*info[ans[i]]);
    }

}

