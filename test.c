
#include <stdio.h>
#include <stdlib.h>

void addO(int *P, int x, int y, int hodnota) { /* prida hodnotu do moznosti */
    setF(P, x, y, getF(P, x, y) | (1 << hodnota));
}

void fillO(int *P, int x, int y) { /* vyplni moznosti pro jedno policko */
    /* TODO: zacachovat getF(x,y) */
    int i, j;
    setF(P, x, y, allO);
    for (i = 0; i < 9; i++) { /*radek*/
        if (isFilledF(P, x, i)) {
            remO(P, x, y, o2nF(P, x, i)); /* odstraneni moznosti */
        }
    }
    for (i = 0; i < 9; i++) { /*sloulec*/
        if (isFilledF(P, i, y)) {
            remO(P, x, y, o2nF(P, i, y)); /* odstraneni moznosti */
        }
    }
    for (i = (x / 3 * 3); i < (x / 3 * 3) + 3; i++) { /*ctverec*/
        for (j = (y / 3 * 3); j < (y / 3 * 3) + 3; j++) {
            if (isFilledF(P, i, j)) {

                remO(P, x, y, o2nF(P, i, j));
            }
        }
    }
}


void copyPole(int *Pa, int *Pb){    /* Co kam */
    int i;
    for (i=0; i<4; i++){
        *(Pb+i) = *(Pa+i);
    }
}

void test (int *P){
  *(P+1)=4;
}

void pprint(int *P){
  int i;
  for (i=0; i<4; i++){
    printf("%d",*(P+i));
  }
  printf("\n");
}

void main (void) {
  int P[4]={0,0,0,0};
  int Q[4]={1,1,1,1};

  copyPole(P,Q);
  test(Q);
  pprint(P);
  pprint(Q);


}





