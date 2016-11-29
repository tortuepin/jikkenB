#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


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

        printf("zero = %d", zeroNum);
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
int extractChar(wordinfo *dic[], int n, char** c, int dicsize, int head, int tail, unsigned char* louds, int* lo){ //{{{
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
    for(i=head; i<tail+1; i++){


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

printf("pre = %s  tmp = %s\n", pre, tmp);
        getN(dic[i]->word, n+2, tmpNext);
printf("preNext = %s tmpNext = %s\n", preNext, tmpNext);
        //もし重複していなかったらc[k]にコピー
        if(k==0 || strcmp(pre, tmp) != 0){

            //c[k]を初期化する
            c[k] = (char*)malloc(sizeof(char)*biteSize+1);
            for(q=0;q<sizeof(char)*biteSize+1;q++)c[k][q]=0;

            //c[k]に次の文字をコピー
            strncpy(c[k], tmp+start, sizeof(char)*biteSize+1);
            c[k][biteSize] = '\0';


            setBit(louds, j, 0);
            printf("set(%d, 0) ",j);
            j++;
            // もしn+1文字めがあったら1を立てる
            if(dic[i]->word[start+biteSize] != 0){
                if(strcmp(tmpNext, preNext)!=0){
                    setBit(louds, j, 1);
                    printf("set(%d, 1)",j);
                    j++;
                }
                strcpy(preNext, tmpNext);
            }
            k++;
            printf("\n");
        }else{
            // もしn+1文字目があったら1を立てる
            if(dic[i]->word[start+biteSize] != 0){
                // もし次の文字も一緒だったらだったら立てる
                if(strcmp(tmpNext, preNext)!=0){
                    setBit(louds, j, 1);
                    printf("tigaukara set(%d, 1)\n",j);
                    j++;
                }
            }
        }

        strcpy(preNext, tmpNext);
        printf("\n");


    }
    printf("j == %d\n", j);
    //printf("k == %d\n", k);
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
int makeLouds(wordinfo *dic[], char** c ,int dicsize, unsigned char* louds, int* lnum){ /* {{{ */
    int i, k;
    int a=0, b;
    int lo=0;
    char pre[3], tmp[3];


    for(i=0; i<50; i++){
        b = extractChar(dic, i, c+a, dicsize, 0, 100, louds, &lo);
        lnum[i] = b;
        a += b;

        if(b==0)break;
    }

    for(k=0; k<10; k++){

        printf("%d ",k);
        show_signed_char(louds[k]);
    }
    printf("\n");

    return a;
} /*}}}*/



// 子をたどる
void searchLouds(char ** c, int csize, unsigned char* louds, int* lnum, char *word){
    char first[3], Nchar[3];
    int i, index, node, numChild, k=0;
    int ansNum, oneNum;

    getN(word, 1, first);


    //一文字目をcから探す
    for(i=0; i<csize; i++){
        if(strcmp(c[i], first)==0){
            break;
        }
    }


    printf("\n\n");
    //この時点でc[i]が一文字め
    printf("hogehogec[%d] = %s\n", i, c[i]);

    ansNum = i;


    for(i=0; i<N; i++){
        index = getIndexNum(louds, ansNum+1, LINE*3);
        printf("index %d\n", index);
        
        printf("%d ", index/8);
        show_signed_char(louds[index/8]-1);
        show_signed_char(louds[index/8]);
        show_signed_char(louds[index/8]+1);

        // 子ノードの個数
        numChild = checkChildren(louds, index);
        printf("KODOMONOKAZU %d\n", numChild);

        oneNum = getNumofOne(louds, index);

        node = oneNum + lnum[0];
        getN(word, i+2, Nchar);
        printf("NODE %d\n", node);
        for(k=0; k<numChild; k++){
            printf("node%d+%d %s \n",node, k, c[node+k]);
            if(strcmp(c[node+k], Nchar)==0){
                break;
            }
        }
        if(k==numChild)break;
        ansNum = node + k;

        printf("yattane!!!! node=%d c[node]=%s\n", ansNum, c[ansNum]);
        printf("---------\nansNum=%d index=%d\n----------\n", ansNum, index);

        
        printf("\n");

    }


    printf("%d %d %d\n", lnum[0], lnum[1], lnum[2]);


}


int main(void){
    wordinfo *info[LINE];
    int dicsize;
    int a;
    int b=0;
    int lnum[50];
    char *c[LINE*15];
    unsigned char louds[LINE*3];
    int i;
    char k = 10;

    char word[100];

    fgets(word, 100, stdin);
    

    if((dicsize=readDic(FILENAME, info)) < 0){
        printf("cannot read dict");
        return -1;
    }

    a = makeLouds(info, c, dicsize, louds, lnum); 
    //printf("a = %d\n", a);
    //printf("lnum = %d\n", lnum[0]);
    for(i=0;i<a+1; i++){
       printf("%d ", i);
       printf("hoge = %s \n", c[i]);
    }


    searchLouds(c, a, louds, lnum, word);


    freeExtracted(c, a);



    freeDic(info, dicsize);
    
    return 0;
}


// vim: set foldmethod=marker :
