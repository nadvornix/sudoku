/*
 * File:   main.c
 * Author: jirka
 *
 * Created on October 23, 2010, 9:39 AM
 */

#include <stdio.h>
#include <stdlib.h>

//#define FIELD_SIZE 9*9

//int  P[FIELD_SIZE];
int RESENI[9][9];
//int RESENI[FIELD_SIZE];

//int pole[9][9];

int allO = 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512; /*vsecky moznosti (= 0b0111111111) */

int P[9][9];

//typedef int[9][9] sudoku ;

//int fillArroundF(int x, int y) __attribute__((always_inline));

int fillArroundF(int x, int y);

int getF(int x, int y) __attribute__((always_inline));

int getF(int x, int y) {
    //return *(*P+9*y+x);
    return P[y][x];
}

void setF(int x, int y, int hodnota) __attribute__((always_inline));

void setF(int x, int y, int hodnota) { /*nastavi hodnotu policka*/
    P[y][x] = hodnota;
    //*(P + getOrd(x, y)) = hodnota;
}

int numBits(int i) __attribute__((always_inline));

int numBits(int i) { /* vrati pocet bitu nastavenych na 1 */
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return ((((i + (i >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24);
}

void vynulujPole() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            P[i][j] = allO;
        }
    }
}

int isEmpty(int x, int y) __attribute__((always_inline));

int isEmpty(int x, int y) {
    return (getF(x, y) == 0);
}

//dosazeni vysledku (v binarni forme) a checknuti jestli je vsecko OK
int setRes2(int x, int y, int hodnota) __attribute__((always_inline));

int setRes2(int x, int y, int hodnota) {
    setF(x, y, hodnota);

    return fillArroundF(x, y);
}


void remO2(int x, int y, int hodnota) __attribute__((always_inline));

void remO2(int x, int y, int hodnota) { /* odebere hodnotu z moznosti */
    int staraHodnota = getF(x, y);

    int novaHodnota = staraHodnota & (~hodnota);

    if (novaHodnota != staraHodnota) {
        setF(x, y, novaHodnota);
        //int pocetBitu = numBits(novaHodnota) ;
        if (numBits(novaHodnota) == 1) {
            fillArroundF(x, y);
            //setRes2(x, y, novaHodnota);
//            return 0;
        }
//        return 0;
    }
//    return (!novaHodnota);
}

int hotovoP() __attribute__((always_inline));

int hotovoP() { /* jestli je sudoku hotove? */
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (numBits(P[i][j]) != 1) {
                return 0;
            }
        }
    }

    return 1;
}

void pprintO(int hodnota) __attribute__((always_inline));

void pprintO(int hodnota) { /* vytiskne moznosti v citelne forme */
    int i;

    for (i = 1; i <= 9; i++) {
        if (hodnota & (1 << i)) {
            printf("%d", i);
        }
    }
}

void pprintF(int x, int y) __attribute__((always_inline));

void pprintF(int x, int y) {
    pprintO(getF(x, y));
}

int isNedokoncenyO(int hodnota) __attribute__((always_inline));

int isNedokoncenyO(int hodnota) { /* vrati True jestli policko ma oznacenou jen 1 moznost */
    return (numBits(hodnota) > 1);
}


int isNedokoncenyF(int x, int y) __attribute__((always_inline));

int isNedokoncenyF(int x, int y) {
    return isNedokoncenyO(getF(x, y));
}


int getNedokonceny(int *x, int *y) __attribute__((always_inline));

int getNedokonceny(int *x, int *y) {
    static int i = 0, j = 0;
    int obratek = 0;

    while (1) {
        for (; i < 9; i++) {
            for (; j < 9; j++) {
                /*
                                printf("%d,%d,%d\n", i,j,isNedokoncenyF( i, j));
                 */
                if (isNedokoncenyF(i, j)) {

                    *x = i;
                    *y = j;
                    j++; /* aby příště pokračovalo o kus vedle */
                    return 1;
                }
            }
            j = 0;
        }
        i = 0;
        //printf("padla\n");
        if (++obratek > 1) { /* zadny volny */
            *x = *y = 0;
            return 0;
        }

    }
}

int getNedokonceny2(int *x, int *y) __attribute__((always_inline));

int getNedokonceny2(int *x, int *y) {
    static int i = 0, j = 0;

    int obratek = 0;

    while (1) {
        for (; i < 9; i++) {
            for (; j < 9; j++) {
                /*
                                printf("%d,%d,%d\n", i,j,isNedokoncenyF( i, j));
                 */
                if (numBits(getF(i, j)) == 2) {
                    //                if (isNedokoncenyF(i, j)) {
                    *x = i;
                    *y = j;
                    j++; /* aby příště pokračovalo o kus vedle */
                    return 1;
                }
            }
            j = 0;
        }
        i = 0;
        //printf("padla\n");
        if (++obratek > 1) { /* zadny volny */
            *x = *y = 0;
            return 0;
        }

        return getNedokonceny(x, y);
    }
}

void vypisSudoku(int P[][9]) { /* vypise vyplnene sudoku (lidsky)*/
    int j;
    for (int i = 0; i < 9; i++) {
        for (j = 0; j < 8; j++) {
            /*if (isFilledF(i, j)) {
                printf("T");
            } else {
                printf("x");
            }*/
            pprintF(i, j);
            printf(" ");
        }
        pprintF(i, j);
        printf("\n");
    }
}

// always_inlude je uplne nahore ^^^

int fillArroundF(int x, int y) { /* vyskrta z okoli hodnotu x*/
    int hodnota = getF(x, y);

    int cordX, cordY;


    int zakladX = ((x / 3)*3);
    int zakladY = ((y / 3)*3);

    for (int i = 0; i < 9; i++) {

        if (y != i) {
                remO2(x, i, hodnota); /* odstraneni moznosti z radku */
                if (isEmpty(x, i)) {
                    return 0;
                }
            }
        if (x != i) {
            remO2(i, y, hodnota); /* odstraneni moznosti z radku */
            if (isEmpty(i, y)) {
                return 0;
            }
        }
        cordX = (i / 3 + zakladX);
        cordY = (i % 3 + zakladY);

        if (cordX != x && cordY != y)
                remO2(cordX, cordY, hodnota);
            if (isEmpty(cordX, cordY)) {
                return 0;
            }
    }
    //    return ((getF(x, 0)) && (getF(x, 1)) && (getF(x, 2)) && (getF(x, 3)) && (getF(x, 4)) &&
    //            (getF(x, 5)) && (getF(x, 6)) && (getF(x, 7)) && (getF(x, 8)) &&
    //
    //            (getF(0, y)) && (getF(1, y)) && (getF(2, y)) && (getF(3, y)) && (getF(4, y)) && (getF(5, y)) &&
    //            (getF(6, y)) && (getF(7, y)) && ( getF(8, y)) &&
    //
    //            (getF(zakladX+0, zakladY+0)) && ( getF(zakladX+0, zakladY+1)) &&
    //            (getF(zakladX+0, zakladY+2)) && ( getF(zakladX+1, zakladY+0)) &&
    //            (getF(zakladX+1, zakladY+1)) && ( getF(zakladX+1, zakladY+2)) &&
    //            (getF(zakladX+2, zakladY+0)) && (getF(zakladX+2, zakladY+1)) &&
    //            ( getF(zakladX+2, zakladY+2)));
    //    return getF(x, 0)*getF(x, 1)*getF(x, 2)*getF(x, 3)*getF(x, 4)*getF(x, 5)*getF(x, 6)*getF(x, 7)*getF(x, 8)*
    //        getF(0, y)*getF(1, y)*getF(2, y)*getF(3, y)*getF(4, y)*getF(5, y)*getF(6, y)*getF(7, y)* getF(8, y)*
    //        getF(zakladX+0, zakladY+0)* getF(zakladX+0, zakladY+1)* getF(zakladX+0, zakladY+2)* getF(zakladX+1, zakladY+0)* getF(zakladX+1, zakladY+1)* getF(zakladX+1, zakladY+2)* getF(zakladX+2, zakladY+0)* getF(zakladX+2, zakladY+1)* getF(zakladX+2, zakladY+2)
    //;


    //    for (int i = 0; i < 9; i++) {
    //
    //        if (y != i) {
    //            remO2(x, i, hodnota); /* odstraneni moznosti z radku */
    //            if (isEmpty(x, i)) {
    //                return 0;
    //            }
    //        }
    //        if (x != i) {
    //            remO2( i, y, hodnota); /* odstraneni moznosti z radku */
    //            if (isEmpty( i, y)) {
    //                return 0;
    //            }
    //        }
    //        cordX = (i / 3 + zakladX);
    //        cordY = (i % 3 + zakladY);
    //
    //        if (cordX != x && cordY != y)
    //            remO2(cordX, cordY, hodnota);
    //        if (isEmpty(cordX, cordY)) {
    //            return 0;
    //        }
    //    }
    return 1;
}

int nactiCislo() {
    int c = getchar();
    if (c == '.') {
        return 0;
    } else if (c > '0' && c <= '9') {
        return c - '0';
    }
    printf("Nespravny vstup.\n");
    exit(0);
}

void nactiZnak(int znak) {
    int c = getchar();
    if (c != znak) {
        printf("Nespravny vstup.\n");
        exit(0);
    }
}

int nactiSudoku() {
    int cislo, pocitadlo = 0;

    vynulujPole();

    printf("Zadejte sudoku:\n");

    for (int i = 0; i < 9; i++) {

        for (int j = 0; j < 9; j++) {
            cislo = nactiCislo();


            if (cislo) {
                pocitadlo++;
                if (!setRes2(i, j, (1 << cislo))) {
                    printf("Nespravny vstup.\n");
                    exit(0);
                }
            }


            if (j == 8) {
                nactiZnak('\n');
            } else {
                nactiZnak(' ');
            }

        }
    }
    return pocitadlo;
}

void copyPole(int Pa[][9], int Pb[][9]) __attribute__((always_inline));

void copyPole(int Pa[][9], int Pb[][9]) { /* Co kam */
    Pb[0][0] = Pa[0][0];
    Pb[0][1] = Pa[0][1];
    Pb[0][2] = Pa[0][2];
    Pb[0][3] = Pa[0][3];
    Pb[0][4] = Pa[0][4];
    Pb[0][5] = Pa[0][5];
    Pb[0][6] = Pa[0][6];
    Pb[0][7] = Pa[0][7];
    Pb[0][8] = Pa[0][8];
    Pb[1][0] = Pa[1][0];
    Pb[1][1] = Pa[1][1];
    Pb[1][2] = Pa[1][2];
    Pb[1][3] = Pa[1][3];
    Pb[1][4] = Pa[1][4];
    Pb[1][5] = Pa[1][5];
    Pb[1][6] = Pa[1][6];
    Pb[1][7] = Pa[1][7];
    Pb[1][8] = Pa[1][8];
    Pb[2][0] = Pa[2][0];
    Pb[2][1] = Pa[2][1];
    Pb[2][2] = Pa[2][2];
    Pb[2][3] = Pa[2][3];
    Pb[2][4] = Pa[2][4];
    Pb[2][5] = Pa[2][5];
    Pb[2][6] = Pa[2][6];
    Pb[2][7] = Pa[2][7];
    Pb[2][8] = Pa[2][8];
    Pb[3][0] = Pa[3][0];
    Pb[3][1] = Pa[3][1];
    Pb[3][2] = Pa[3][2];
    Pb[3][3] = Pa[3][3];
    Pb[3][4] = Pa[3][4];
    Pb[3][5] = Pa[3][5];
    Pb[3][6] = Pa[3][6];
    Pb[3][7] = Pa[3][7];
    Pb[3][8] = Pa[3][8];
    Pb[4][0] = Pa[4][0];
    Pb[4][1] = Pa[4][1];
    Pb[4][2] = Pa[4][2];
    Pb[4][3] = Pa[4][3];
    Pb[4][4] = Pa[4][4];
    Pb[4][5] = Pa[4][5];
    Pb[4][6] = Pa[4][6];
    Pb[4][7] = Pa[4][7];
    Pb[4][8] = Pa[4][8];
    Pb[5][0] = Pa[5][0];
    Pb[5][1] = Pa[5][1];
    Pb[5][2] = Pa[5][2];
    Pb[5][3] = Pa[5][3];
    Pb[5][4] = Pa[5][4];
    Pb[5][5] = Pa[5][5];
    Pb[5][6] = Pa[5][6];
    Pb[5][7] = Pa[5][7];
    Pb[5][8] = Pa[5][8];
    Pb[6][0] = Pa[6][0];
    Pb[6][1] = Pa[6][1];
    Pb[6][2] = Pa[6][2];
    Pb[6][3] = Pa[6][3];
    Pb[6][4] = Pa[6][4];
    Pb[6][5] = Pa[6][5];
    Pb[6][6] = Pa[6][6];
    Pb[6][7] = Pa[6][7];
    Pb[6][8] = Pa[6][8];
    Pb[7][0] = Pa[7][0];
    Pb[7][1] = Pa[7][1];
    Pb[7][2] = Pa[7][2];
    Pb[7][3] = Pa[7][3];
    Pb[7][4] = Pa[7][4];
    Pb[7][5] = Pa[7][5];
    Pb[7][6] = Pa[7][6];
    Pb[7][7] = Pa[7][7];
    Pb[7][8] = Pa[7][8];
    Pb[8][0] = Pa[8][0];
    Pb[8][1] = Pa[8][1];
    Pb[8][2] = Pa[8][2];
    Pb[8][3] = Pa[8][3];
    Pb[8][4] = Pa[8][4];
    Pb[8][5] = Pa[8][5];
    Pb[8][6] = Pa[8][6];
    Pb[8][7] = Pa[8][7];
    Pb[8][8] = Pa[8][8];

    //    for (int i = 0; i < 9; i++) {
    //        for (int j = 0; j < 9; j++) {
    //            Pb[i][j] = Pa[i][j];
    //            //*(Pb + i) = *(Pa + i);
    //        }
    //    }
}

int pocetNedokoncenych() {
    int pocitadlo = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (isNedokoncenyO(getF(j, i))) {
                pocitadlo++;
            }
        }
    }
    return pocitadlo;
}

//int ** allokujPole() {
//    int **Q, i;
//    Q = (int **) malloc(sizeof (int **) * 9);
//    for (i = 0; i < 9; i++) {
//        *(Q + i) = (int *) malloc(sizeof (int *) * 9);
//    }
//    return Q;
//}

//void uvolniPole (int **pole){
//
//    for (int i = 0; i < 9; i++) {
//        free(*(pole+i));
////        *(Q + i) = (int *) malloc(sizeof (int *) * 9);
//    }
//    free(pole);
//}

int tipuj() { /* zacne alternativni cestu */
    int x, y, o, cislice, reseni = 0;

    int oldPole[9][9];

    //    int **oldPole = allokujPole();
    //
    //    int **oldPole = (int **) alloca(sizeof (int **) * 9);
    //    for (int i = 0; i < 9; i++) {
    //        *(oldPole + i) = (int *) alloca(sizeof (int *) * 9);
    //    }

    if (!getNedokonceny2(&x, &y)) {
        //printf("hotovo\n");
        return 0;
    }

    o = getF(x, y);

    for (int i = 1; i <= 9; i++) {
        //cislice = o2nO(o & (1 << i)); /*mozna cislice*/
        cislice = o & (1 << i); /*mozna cislice*/
        if (cislice) {

            copyPole(P, oldPole);

            if (setRes2(x, y, cislice)) {
                if (hotovoP()) {
                    reseni += 1;
                    copyPole(P, RESENI);
                } else {
                    reseni += tipuj();
                    //copyPole(oldPole, P);
                }
            }
            copyPole(oldPole, P);
        }
    }
    //    uvolniPole(oldPole);
    return reseni;
}

int main(int argc, char** argv) {
    int nedokoncenych, pocetReseni = 0;
    //int **P = allokujPole();

    //    RESENI = allokujPole();

    //int oldPole[FIELD_SIZE];
    if (nactiSudoku() < 17) {
        printf("Nespravny vstup.\n");
        exit(1);
    }

    nedokoncenych = pocetNedokoncenych();

    if (nedokoncenych == 0) pocetReseni = 1;
    else {
        pocetReseni = tipuj();
        copyPole(RESENI, P);
    }

    if (pocetReseni == 0) printf("Sudoku nema zadne reseni.\n");
    if (pocetReseni > 1) printf("Reseni neni jednoznacne (%d variant).\n", pocetReseni);
    if (pocetReseni == 1) {
        printf("Reseni:\n");
        vypisSudoku(P);
    }

    return (EXIT_SUCCESS);
}

/*
 *  TODO:
 * */
