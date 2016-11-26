#include <stdio.h>
#include <stdlib.h>


#define MASK0 255
#define MASK8 128
#define MASK7 64
#define MASK6 36
#define MASK5 16
#define MASK4 8
#define MASK3 4
#define MASK2 2
#define MASK1 1

void show_signed_char(signed char x)
{
  int i;

  printf("%4d  %02x  ", x, (unsigned char)x);
  for (i = 7; i >= 0; i--) {
    printf("%d", (x>>i) & 1);
  }
  printf("\n");
}


// cのnビット目が何か
int checkMask(unsigned char c, int n){
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

}

int main(void){
    unsigned char c = MASK7;
    printf("%c", c);

    show_signed_char(c);

    printf("%d", checkMask(c, 7));
    printf("%d", checkMask(c, 1));

}
