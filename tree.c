#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>


#define MASK0 255
#define MASK8 128
#define MASK7 64
#define MASK6 32
#define MASK5 16
#define MASK4 8
#define MASK3 4
#define MASK2 2
#define MASK1 1
#define N 256
#define INFOLEN N*5
#define LINE 120000
#define FILENAME "../dic/jdic.txt"

#define BITE 8

#define HEAD 80
#define TAIL 100


typedef struct {
    char word[N];
    char yomi[N];
    char kihon[N];
    char hinshi[N];
    double cost;
} wordinfo;

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
             if((c & MASK8) == 0){
                 return 0;
             }else{
                return 1;
            }
            break;
        case 2:
            if((c & MASK7) == 0){
                return 0;
            }else{
                return 1;
            }
            break;

        case 3:
            if((c & MASK6) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 4:
            if((c & MASK5) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 5:
            if((c & MASK4) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 6:
            if((c & MASK3) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 7:
            if((c & MASK2) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
        case 8:
            if((c & MASK1) == 0){
                return 0;
            }else{
                return 1;
            }
            break;
    }
    return -1;
} //}}}
// cのnビット目をkにする
unsigned char _setBit(unsigned char c, int n, int k){ //{{{
    switch(n){
        case 1:
            if(k == 0){
                c &= ~MASK8;
            }else{
                c |= MASK8;
            }
            break;
        case 2:
            if(k == 0){
                c &= ~MASK7;
            }else{
                c |= MASK7;
            }
            break;
        case 3:
            if(k == 0){
                c &= ~MASK6;
            }else{
                c |= MASK6;
            }
            break;
        case 4:
            if(k == 0){
                c &= ~MASK5;
            }else{
                c |= MASK5;
            }
            break;
        case 5:
            if(k == 0){
                c &= ~MASK4;
            }else{
                c |= MASK4;
            }
            break;
        case 6:
            if(k == 0){
                c &= ~MASK3;
            }else{
                c |= MASK3;
            }
            break;
        case 7:
            if(k == 0){
                c &= ~MASK2;
            }else{
                c |= MASK2;
            }
            break;
        case 8:
            if(k == 0){
                c &= ~MASK1;
            }else{
                c |= MASK1;
            }
            break;
    }
    return c;
} //}}}
// char[]のnビット目をkにする
void setBit(unsigned char* c, int n, int k){ // {{{
    int a = n/(sizeof(char)*8);
    int b = n%(sizeof(char)*8);
    //a文字めのbビット目を書き換える
    c[a] = _setBit(c[a], b+1, k);
} //}}}
// char[]のnビット目が何か
int checkBit(unsigned char* c, int n){ /*{{{*/
    int a = n/(sizeof(char)*8);
    int b = n%(sizeof(char)*8);
    
    return checkMask(c[a], b+1);

}/*}}}*/
// n個めの0のインデックスを探す
int getIndexNum(unsigned char* louds, int n, int loudsSize){/* {{{ */ 
    int i;
    int zeroNum=0;

    for(i=0; i<loudsSize*8; i++){
        if(checkBit(louds, i) == 0){
            zeroNum++;
        }
        if(zeroNum>=n)break;
    }

    return i;

}/*}}}*/
// indexまでの1の数(ノード番号)
int getNumofOne(unsigned char* louds, int index){/*{{{*/
    int i;
    int num=0;
    for(i=0;i<index;i++){
        if(checkBit(louds, i)==1){
            num++;
        }
    }
    return num;
}/*}}}*/

int getNumofZero(unsigned char* louds, int index){/*{{{*/
    int i;
    int num=0;
    for (i=0;i<index;i++){
        if(checkBit(louds, i)==0){
            num++;
        }
    }
    return num;
}/*}}}*/
// indexの右にいくつ1があるか(子ノードの数)
int checkChildren(unsigned char* louds, int index){/* {{{*/
    int i=0;
    while(1){
        if(checkBit(louds, index+i+1) == 0){
            break;
        }else{
            i++;
        }
    }
    return i;
}/* }}}*/
// バイト数チェック
int checkBite(unsigned char c){ // {{{
    if((int)c >= 128){
        return 2;
    }
    return 1;
} //}}}

// retにn文字目を入れる
// 返り値はn文字目の初めのbite数
int getN(char* word, int n, char* ret){ /*{{{*/
    int k = sizeof(word)/sizeof(char);
    int i, bite=0;
    char c[3];

    for(i=1; i<n; i++){
        if(checkBite(word[bite])==2){
            bite += 2;
        }else{
            bite += 1;
        }
    }


    ret[0] = word[bite];
    if(checkBite(word[bite]==2)){
        ret[1] = word[bite+1];
        ret[2] = '\0';
    }else{
        ret[1] = '\0';
    }

    return k;
}/*}}}*/
// 辞書ファイルから重複がないようにn文字目をとってくる
// ついでにlouds作る
// 必ずfreeExtractedすること
int extractChar(wordinfo *dic[], int n, char** c, int dicsize, int head, int tail, unsigned char* louds, int* lo, int* d){ //{{{
    int i;
    int biteSize, start;
    int k=0;
    int j, l, q;
    char tmp[N];
    char pre[N];
    char preNext[3];
    char tmpNext[3];

    for(i=0; i<N; i++){
        tmp[i] = '\0';
        pre[i] = '\0';
    }

    j=*lo;
    //辞書の指定された頭から終わりまでたどる
    for(i=head; i<tail; i++){


        start = 0;
        biteSize = checkBite(dic[i]->word[0]);
        for(l=0; l<n; l++){
            start = start+biteSize;
            biteSize = checkBite(dic[i]->word[start]);
        }


        // tmpに単語のn文字目まで入れる
        strcpy(pre, tmp);
        strncpy(tmp, dic[i]->word, sizeof(char)*(start+biteSize));
        tmp[start+biteSize] = '\0';



        //もしwordの長さがn以下だったら次へ
        if(dic[i]->word[start] == '\0'){
            continue;
        }

        getN(dic[i]->word, n+2, tmpNext);
        //もし重複していなかったらc[k]に現在の文字をコピー
        if(k==0 || strcmp(pre, tmp) != 0){

            //c[k]を初期化する
            c[k] = (char*)malloc(sizeof(char)*biteSize+1);
            for(q=0;q<sizeof(char)*biteSize+1;q++)c[k][q]=0;

            //c[k]に次の文字をコピー
            strncpy(c[k], tmp+start, sizeof(char)*biteSize+1);
            c[k][biteSize] = '\0';


            setBit(louds, j, 0);
            j++;
            // もしn+1文字めがあったら1を立てる
            if(dic[i]->word[start+biteSize] != 0){
                //if(strcmp(tmpNext, preNext)!=0){
                    setBit(louds, j, 1);
                    j++;
                //}
                strcpy(preNext, tmpNext);
            }else{
                //なければd[k]にiを入れる
                d[k] = i;
            }
            k++;
        }else{
            // もしn+1文字目があったら1を立てる
            if(dic[i]->word[start+biteSize] != 0){
                // もし次の文字も一緒だったらだったら立てる
                if(strcmp(tmpNext, preNext)!=0){
                    setBit(louds, j, 1);
                    j++;
                }
            }
                
        }

        strcpy(preNext, tmpNext);


    }
    *lo = j;
    return k;

} //}}}
// extractedCharのcを開放する
void freeExtracted(char **c, int size){ //{{{
    int i;
    for(i=0; i<size; i++){
        free(c[i]);
    }
} //}}}


// Loudsを作る
// freeすること
int makeLouds(wordinfo *dic[], char** c ,int dicsize, unsigned char* louds, int* lnum, int* d){ /* {{{ */
    int i, k;
    int a=0, b;
    int lo=0;
    char pre[3], tmp[3];


    for(i=0; i<50; i++){
        b = extractChar(dic, i, c+a, dicsize, 0, dicsize, louds, &lo, d+a);
        lnum[i] = b;
        a += b;

        if(b==0)break;
    }


    return a;
} /*}}}*/



// 子をたどる
int searchLouds(char ** c, int csize, unsigned char* louds, int* lnum, char *word, int* ans){/* {{{*/
    char first[3], Nchar[3];
    int i, index, node, numChild, k=0, l, bini, binhead, bintail, binresult;
    int ansNum, oneNum;
    clock_t start, end;


    getN(word, 1, first);


    //一文字目をcから探す
    start = clock();

    for(i=0; i<csize; i++){
        if(strcmp(c[i], first)==0){
            break;
        }
    }
    end = clock();
    printf("1Mozime %lu\n", end-start);

    //bini = (csize-1)/2;
    //binhead = 0;
    //bintail = csize-1;
    //while(1){
    //    binresult = strcmp(c[bini], first);
    //    if(binhead > bintail){
    //        break;
    //    }
    //    if(binresult == 0){
    //        break;
    //    }else if(binresult > 0){
    //        bintail = bini-1;
    //    }else if(binresult < 0){
    //        binhead = bini+1;
    //    }
    //    bini = (binhead+bintail)/2;
    //}

    //i = bini;


    //この時点でc[i]が一文字め

    ansNum = i;
    ans[0] = i;


    for(i=0; i<N; i++){
        strcpy(Nchar, "");
    start = clock(); 
        index = getIndexNum(louds, ansNum+1, LINE*3);
        
    end = clock();
    printf("1getIndex %lu\n", end-start);
        // 子ノードの個数

        numChild = checkChildren(louds, index);

    start = clock();
        oneNum = getNumofOne(louds, index);
    end = clock();
    printf("getNumofOne %lu\n", end-start);

        node = oneNum + lnum[0];
        getN(word, i+2, Nchar);

        for(k=0; k<numChild; k++){
            if(strcmp(c[node+k], Nchar)==0){
                break;
            }
        }
        if(k==numChild)break;
        ansNum = node + k;


        ans[i+1] = ansNum;
        

    }
    return i+1;

}/*}}}*/

void printWordInfo(wordinfo info){ /* {{{*/
    printf("%s\t%s\t%s\t%s\t\n", info.word, info.yomi, info.kihon, info.hinshi);
}/*}}}*/
void minCostMatch(char *buf, int len, int dicsize, wordinfo* info[], char** c, int csize, unsigned char* louds, int *lnum, int * ansList, int* d){/* {{{*/
    double minCost[1000];
    int minWordNum[1000];
    char tmpWord[1000];
    double wordCost[1000][50];
    int lattice[1000][50];//[文字位置][単語数]
    int wordLen[1000][50];//lattice[n][i]の単語の長さ
    int latticeNum[1000];//lattice[n]に何個単語が入ってるか
    int i, k, hit, j=0, l;
    int place[1000];
    int p=0, kp=0;
    double tmpCost;
    int revans[1000];
    int ans[1000];
    int ansNum = 0;
    int owari[100];
    for(i=0;i<1000;i++)latticeNum[i] = 0;

    //初期化
    //ラティスを作る
    //printf("len =1 %d ", len);
    for(i=0; i<len; i++){
        if(checkBite(buf[i]) == 2)i++;

        strncpy(tmpWord, buf+i-1, len-i+1);
        tmpWord[len-i+1] = '\0';
        ansNum = searchLouds(c, csize, louds, lnum, tmpWord, ansList);
        l=0;
        for(k=0;k<ansNum;k++){
            lattice[p+k][latticeNum[p+k]] = -1;
            if(d[ansList[k]]==-1)continue;
            lattice[p+k][latticeNum[p+k]] = d[ansList[k]];
            wordCost[p+k][latticeNum[p+k]] = info[d[ansList[k]]]->cost;
            wordLen[p+k][latticeNum[p+k]] = k+1;
            latticeNum[p+k]++;
            
            
        }
        
        p++;

    }

    /*
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

            //ラティスとか初期化
            lattice[p][j] = -1;
            //wordLen[p][j] = 1;
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
*/
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
        //fprintf(stderr, "i==%d\n", i);
        revans[k] = lattice[i][minWordNum[i]];
        //fprintf(stderr, "revans[%d] == %d\n", k, revans[k]);
        if(revans[k] >= 0){
        //    printWordInfo(*info[revans[k]]);
        //printf("i = %d\n", i);
        //printf("k = %d\n", k);
        //printf("revans[k] = %d\n", revans[k]);
        //printf("len = %d\n", wordLen[i][minWordNum[i]]);
        //printf("minWordNum[i] = %d\n", minWordNum[i]);

        //printf("lattice[i][0] = %d\n", lattice[i][0]);
        //printf("lattice[i][1] = %d\n", lattice[i][1]);

        //printf("lattice[i+2][0] = %d\n", lattice[i-1][0]);
        //printf("lattice[i+2][1] = %d\n", lattice[i-1][0]);
        
        //fprintf(stderr,"i-wordLen[%d][minWordNum[%d]](%d)\n", i, i,wordLen[i][minWordNum[i]]);
        i -= wordLen[i][minWordNum[i]];
        
        }else{
            i -= 1;
        }

        if(i<0)break;
        k++;
    }

    for(i=0; i<=k; i++){
        ans[i] = revans[k-i];
        if(ans[i] >= 0){
            printWordInfo(*info[ans[i]]);
        }else{
            printf("undefined\n");
        }
    }
}/*}}}*/


int main(int argc, char* argv[]){
    wordinfo *info[LINE];
    int dicsize;
    int a;
    int b=0;
    int lnum[50];
    char *c[LINE*15];
    unsigned char louds[LINE*3];
    int d[LINE*3];
    //int ans[N];
    int i, len;
    int ansList[N];
    char buf[100];


    if((dicsize=readDic(FILENAME, info)) < 0){
        printf("cannot read dict");
        return -1;
    }

    for(i=0;i<LINE*3;i++)d[i]=-1;

    a = makeLouds(info, c, dicsize, louds, lnum, d); 
i=0;
    while(fgetline(buf, sizeof(buf), stdin) != -1){
        
        len = strlen(buf);
        minCostMatch(buf, len, dicsize, info, c, a, louds, lnum, ansList, d);
        i++;
    }
    //ansnum = searchLouds(c, a, louds, lnum, word, ans);
    

    //for (i=0; i<ansnum; i++){
    //    printf("%s\n", info[d[ans[i]]]->word);
    //}



    freeExtracted(c, a);
    freeDic(info, dicsize);
    
    return 0;
}


// vim: set foldmethod=marker :
