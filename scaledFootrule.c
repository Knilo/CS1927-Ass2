#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "DLList.h"
#include "url.h"

#define BUFSIZE 50

DLList getListWithPosition(char * file);
//char* stradd(const char* a, const char* b);
int getN(DLList a, DLList b);
DLList makeMergedList(DLList a, DLList b);


int main (int argc, const char * argv[]) {
    int i = 0;
    int n = 0;
    DLList mergedList = newDLList();
    if (argc > 1) {
        //DLList mergedList = newDLList();
        if (argc == 2) {

            char * file = malloc(sizeof(argv[1]));
            strcpy(file, argv[1]);

            mergedList = getListWithPosition(file);
            n = DLListLength(mergedList);
        }
        if (argc == 3) {
            puts("1");
            char file1[50];// = malloc(sizeof(argv[1]));
            puts("2");
            strcpy(file1, argv[1]);
            puts("3");
            DLList listForFile1 = newDLList();
            listForFile1 = getListWithPosition(file1);
            showDLList(listForFile1);
            puts("");

            char file2[50];// = malloc(sizeof(argv[2]));
            strcpy(file2, argv[2]);
            DLList listForFile2 = newDLList();
            listForFile2 = getListWithPosition(file2);
            showDLList(listForFile2);
            puts("");

            n = getN(listForFile1, listForFile2);
            mergedList = makeMergedList(listForFile1, listForFile2);
            showDLList(mergedList);

        }
        if (argc > 3) {
            char file1[50];// = malloc(sizeof(argv[1]));

            strcpy(file1, argv[1]);

            DLList listForFile1 = newDLList();
            listForFile1 = getListWithPosition(file1);
            //DLList TfIdfFinal = newDLList();

            for (i = 2; argv[i] != NULL; i++) {

                //printf("i = %d", i);
                char nextFile[50];// = malloc(sizeof(argv[i]));

                strcpy(nextFile, argv[i]);

                DLList listForNextFile = newDLList();

                listForNextFile = getListWithPosition(nextFile);

                if (i == 2) {
                    n = getN(listForFile1, listForNextFile);
                    mergedList = makeMergedList(listForFile1, listForNextFile);
                } else {
                    n = getN(listForNextFile, mergedList);
                    mergedList = makeMergedList(listForNextFile, mergedList);
                }

            }
// now need to order final list

        }
        //int n = getN(mergedList);
        puts("");
        printf("n = %d\n", n);
        //double wcp = wcp(mergedList, n, p);

    } else {
        fprintf(stderr, "usage: %s file1 file2 file3...\n", argv[0]);
        exit(1);

    }
    free(mergedList);
    return EXIT_SUCCESS;
    /*
    }
        DLList T1 = newDLList();
        T1 = getListWithPosition("rankA.txt");



        DLList T2 = newDLList();
        T2 = getListWithPosition("rankB.txt");


        DLList merged = newDLList();
        merged = mergeLists(T1, T2);
        int n = DLListLength(merged);
        int sizeT1 = DLListLength(T1);
        int sizeT2 = DLListLength(T2);
        for (i = 0; i <= DLListLength(T1); i++) {
            getdegree(T1);
        }
    */







}

DLList getListWithPosition(char * file) {
    int i;
    DLList URLs = newDLList();
    //char* filename = stradd(file, ".txt");
    char buffer[BUFSIZE];
    //int URLcount = 0;
    FILE * openfile = fopen (file, "r");
    while (!feof(openfile)) {
        while ((fgets(buffer, sizeof(buffer), openfile) != NULL)) {
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) {

                getURL(cur, link, BUFSIZE - 1);
                DLListAfter(URLs, link, 0, 0);
                //showDLList(URLs);
                //URLcount++;
                //printf("%s, %d\n", link, URLcount);
                cur += strlen(link);
            }
        }
    }
    fclose(openfile);

    int URLcount = DLListLength(URLs);
//add postions to nodes
    DLListMoveTo(URLs, 1);
    for (i = 0; i < URLcount; i++) {

        alterDegree (URLs, i + 1);
        DLListMove(URLs, 1);
    }
    alterDegree(URLs, (getDegree(URLs) - 1));
    char length[50];
    //put length of list as last node
    //itoa(DLListLength(URLs), length, 10);
    i = sprintf(length, "%d", DLListLength(URLs));
    DLListAfter(URLs, length, 0, 0);
    return URLs;
}
/*
char* stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) , b);
}*/

DLList makeMergedList(DLList a, DLList b) {

    if (DLListIsEmpty(b)) {
        return a;
    } else if (DLListIsEmpty(a)) {
        return b;
    }

    DLList new = newDLList();
    int i = 0;

    for (i = 0; i < DLListLength(a); i++) {

        DLListMoveTo(a, i + 1);
        DLListAfter(new, DLListCurrent(a), 0, getDegree(a));
    }
    for (i = 0; i < DLListLength(b); i++) {

        DLListMoveTo(b, i + 1);
        DLListAfter(new, DLListCurrent(b), 0, getDegree(b));
    }
    freeDLList(a);
    freeDLList(b);
    return new;
}

int getN(DLList a, DLList b) {


    if (DLListIsEmpty(b)) {
        return DLListLength(a);
    } else if (DLListIsEmpty(a)) {
        return DLListLength(b);
    }
    //DLList new = newDLList();

    int i = 0;
    int j = 0;
    //int used = 0;
    int n = DLListLength(a) + DLListLength(b);
    //printf("n= %d", n);

    for (i = 0; i < DLListLength(a); i++) {

        DLListMoveTo(a, i + 1);
        //used = 0;
        for (j = 0; j <= DLListLength(b); j++) {
            DLListMoveTo(b, j + 1);
            //if b is in a, sum tfidf and add to new
            if (strcmp(DLListCurrent(a), DLListCurrent(b)) == 0) {
                //DLListAfter(new, DLListCurrent(a), 0, 0);
                n--;
                break;
            }
        }

    }
    for (i = 0; i < DLListLength(a); i++) {
        DLListMoveTo(a, i + 1);
        if (strstr(DLListCurrent(a), "url") == NULL) {
            n--;
        }
    }
    return n;
}
