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
int getN(DLList a, DLList b);
DLList makeMergedList(DLList a, DLList b);
int strIsIn(DLList, char *);
void getPos(int n, int j, int *visited, int *p, double *minScaleFDist, DLList l, int numOfFiles);
void fileSize(DLList l, int *s);
int factorial(int n);
double scalFootDist(DLList l, int n, int *p, int numOfFiles);
double minScaleFDist(DLList l, int n, int numOfFiles);
void printUrls(DLList l, int n);

int main (int argc, const char * argv[]) {
    int i = 0;
    int n = 0;
    DLList mergedList = newDLList(); //create list to print
    if (argc > 1) { //check amount of inputs
        if (argc == 2) { //if one input
            char * file = malloc(sizeof(argv[1])); //set string with length of file name
            strcpy(file, argv[1]); //copy filename to string
            mergedList = getListWithPosition(file); //call function to create list of urls in file with position
            n = (DLListLength(mergedList) - 1); //sets n for list
            free(file);
        }
        if (argc == 3) { //if two input files
            char file1[50]; //create array of charcters to store file name
            strcpy(file1, argv[1]); //copy filename to array
            DLList listForFile1 = newDLList(); //create empty list for file 1
            listForFile1 = getListWithPosition(file1); //call function to create list of urls in file 1 with position
            char file2[50]; //create array of charcters to store file name
            strcpy(file2, argv[2]); //copy filename to array
            DLList listForFile2 = newDLList(); //create empty list for file 2
            listForFile2 = getListWithPosition(file2); //call function to create list of urls in file 2 with position
            n = getN(listForFile1, listForFile2); //call function to calculate n
            mergedList = makeMergedList(listForFile1, listForFile2); //call function to merge lists for final calculation
        }
        if (argc > 3) { //if there are more than 2 inputs
            char file1[50]; //create array of charcters to store file name
            strcpy(file1, argv[1]); //copy filename to array
            DLList listForFile1 = newDLList(); //create empty list for file 1
            listForFile1 = getListWithPosition(file1); //call function to create list of urls in file 1 with position
            for (i = 2; argv[i] != NULL; i++) {

                char nextFile[50]; //create array of characters to store next filename
                strcpy(nextFile, argv[i]); //copy filename to array
                DLList listForNextFile = newDLList(); //create empty list for next file
                listForNextFile = getListWithPosition(nextFile);//call function to create list of urls in next file with position
                if (i == 2) { //if second file
                    n = getN(listForFile1, listForNextFile); //call function to calculate n
                    mergedList = makeMergedList(listForFile1, listForNextFile); //call function to merge lists for final calculation
                } else {
                    n = getN(listForNextFile, mergedList); //call function to calculate n
                    mergedList = makeMergedList(listForNextFile, mergedList); //call function to merge lists for final calculation
                }
            }
        }
        double wcp = minScaleFDist(mergedList, n, argc - 1); //call function to calculate wcp
        printf("%f\n", wcp); //print wcp
        printUrls(mergedList, n); //call function to order and print urls
        free(mergedList); //free final list
    } else { //if incorrect inputs print error message
        fprintf(stderr, "usage: %s file1 file2 file3...\n", argv[0]);
        exit(1);

    }

    return EXIT_SUCCESS;
}
//function to create list from file and calculate their positions
DLList getListWithPosition(char * file) {
    int i;
    DLList URLs = newDLList(); //create empty list
    char buffer[BUFSIZE];
    FILE * openfile = fopen (file, "r"); //open the supllied file
    while (!feof(openfile)) { //read until end of file
        while ((fgets(buffer, sizeof(buffer), openfile) != NULL)) { //get line
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) { //find next "url..."
                getURL(cur, link, BUFSIZE - 1); //normalise url name
                DLListAfter(URLs, link, 0, 0); //add to list
                cur += strlen(link);
            }
        }
    }
    fclose(openfile); //close file
    int URLcount = DLListLength(URLs); //calculate amounf of urls
    DLListMoveTo(URLs, 1);
    for (i = 0; i <= URLcount; i++) { //loop to add postions to nodes
        alterDegree (URLs, i + 1);
        DLListMove(URLs, 1);
    }
    alterDegree(URLs, (getDegree(URLs) - 1));
    char length[50];
    i = sprintf(length, "%d", DLListLength(URLs)); //convert length as integer to string
    DLListAfter(URLs, length, 0, 0); //put length of list as last node
    return URLs;
}
//function to merge to url lists with postion
DLList makeMergedList(DLList a, DLList b) {
    //if one of the lists is empty, return the other
    if (DLListIsEmpty(b)) {
        freeDLList(b);
        return a;
    } else if (DLListIsEmpty(a)) {
        freeDLList(a);
        return b;
    }
    DLList new = newDLList(); //create new empty list
    int i = 0;
    for (i = 0; i < DLListLength(a); i++) { //add all nodes from a to new
        DLListMoveTo(a, i + 1);
        DLListAfter(new, DLListCurrent(a), 0, getDegree(a));
    }
    for (i = 0; i < DLListLength(b); i++) { //add all nodes from b to new
        DLListMoveTo(b, i + 1);
        DLListAfter(new, DLListCurrent(b), 0, getDegree(b));
    }
    //free both lists
    freeDLList(a);
    freeDLList(b);
    return new;
}
//function to calculate n (amount of urls)
int getN(DLList a, DLList b) {
    //if one of the supllied lists is empty return the other
    if (DLListIsEmpty(b)) {
        return DLListLength(a);
    } else if (DLListIsEmpty(a)) {
        return DLListLength(b);
    }
    int i = 0;
    int j = 0;
    int n = DLListLength(a) + DLListLength(b); //set n to be total of both lists
    for (i = 0; i < DLListLength(a); i++) { //cycle through a
        DLListMoveTo(a, i + 1);
        for (j = 0; j <= DLListLength(b); j++) { //search through b for any nodes that coincide
            DLListMoveTo(b, j + 1);
            if (strcmp(DLListCurrent(a), DLListCurrent(b)) == 0) { //if b is in subract 1 from n
                n--;
                break;
            }
        }
    }
    for (i = 0; i < DLListLength(a); i++) { //search through a for any nodes not of type url... subract from n if one is found
        DLListMoveTo(a, i + 1);
        if (strstr(DLListCurrent(a), "url") == NULL) {
            n--;
        }
    }
        for (i = 0; i < DLListLength(b); i++) { //search through b for any nodes not of type url... subract from n if one is found
        DLListMoveTo(b, i + 1);
        if (strstr(DLListCurrent(b), "url") == NULL) {
            n--;
        }
    }
    return n;
}

double scalFootDist(DLList l, int n, int *p, int numOfFiles) {
    double sum = 0;
    char str[50]; // the string array that contained the url string
    DLList visited = newDLList(); // list of the visited urls
    int *sizeT = malloc(sizeof(int) * numOfFiles); // array of the size of the various files, e.g sizeT[0] = the size of the first file.
    fileSize(l, sizeT);
    int i;
    //printf("n = %d\n", n);
    int fI = 0; // file Itterator
    if (!DLListIsEmpty(l)) {
        for (i = 0; i < n; i ++) {
            DLListMoveTo(l, i + 1); // moves the current node in l back
            fI = 0;
            // if the current string has alreday been visited skip
            while (strIsIn(visited, DLListCurrent(l)) || strncmp(DLListCurrent(l), "url", 3) != 0) {
                //printf("t %d\n", strncmp(DLListCurrent(l), "url", 3));
                if (strncmp(DLListCurrent(l), "url", 3) != 0) {
                    fI ++;
                }
                if (DLListMove(l, 1) == 1) {
                    free(sizeT);
                    return sum;
                }
            }

            strcpy(str, DLListCurrent(l));
            DLListAfter(visited, str, 0, 0);

            //printf("degree = %d, sizet = %d, p[i] = %d, sum = %f\n", getDegree(l), sizeT[fI], p[i], sum);

            sum = fabs((double)getDegree(l) / (double)sizeT[fI] - (double)p[i] / (double)n) + sum;
            //printf("after sum = %lf\n", sum);
            // finds the same url in the list
            while (DLListMove(l, 1) == 0) {
                if (strncmp(DLListCurrent(l), "url", 3) != 0) {
                    fI ++;
                } else {
                    if (strcmp(DLListCurrent(l), str) == 0) {
                        //printf("degree = %d, sizet = %d, p[i] = %d, sum = %f\n", getDegree(l), sizeT[fI], p[i], sum);
                        sum += fabs((double)getDegree(l) / (double)sizeT[fI] - (double)p[i] / (double)n);
                        //printf("after sum = %lf\n", sum);
                    }
                }
            }
        }
    }
    free(sizeT);
    //printf("final sum = %lf\n", sum);
    return sum;
}
//finds the different sets of p and uses the output to find the Scale of Distance
void getPos(int n, int j, int *visited, int *p, double *minScaleFDist, DLList l, int numOfFiles) {
    int i;
    double num;
    if (j == n) {
        if (*minScaleFDist < 0) {
            for (i = 0; i < n; i ++) { // for testing
                //printf("p[%d] %d\n", i, p[i]);
            }
            *minScaleFDist = scalFootDist(l, n, p, numOfFiles);
        } else {
            num = scalFootDist(l, n, p, numOfFiles);
            if (*minScaleFDist > num) {
                *minScaleFDist = num;
            }
        }
    }
    for (i = 0; i < n; i ++) {
        if (visited[i] == 0) {
            p[j] = i + 1;
            visited[i] = 1;
            getPos(n, j + 1, visited, p, minScaleFDist, l, numOfFiles);
            visited[i] = 0;
        }
    }
} // does the string belong in the List, 1 if true, 0 otherwise
int strIsIn(DLList l, char *str) {
    if (DLListIsEmpty(l)) {
        return 0;
    }
    if (strcmp(DLListCurrent(l), str) == 0) {
        return 1;
    }
    while (DLListMove(l, 1) == 0) {
        if (strcmp(DLListCurrent(l), str) == 0) {
            return 1;
        }
    }
    return 0;
}
// wrapper function
double minScaleFDist(DLList l, int n, int numOfFiles) {
    //int factN = factorial(n);
    //printf("%d\n", factN); // delete after testing

    double *minScaleFDist = malloc(sizeof(double));
    *minScaleFDist = -1;
    int i;
    int *visited = malloc(sizeof(int) * n);
    assert(visited != NULL);
    for (i = 0; i < n; i ++) {
        visited[i] = 0;
    }
    int *p = malloc(sizeof(int) * n);
    getPos(n, 0, visited, p, minScaleFDist, l, numOfFiles);
    free(visited);
    double min = *minScaleFDist;
    free(minScaleFDist);
    free(p);
    return min;
}

void fileSize(DLList l, int *s) {
    int i = 0;
    if (!DLListIsEmpty(l)) {
        DLListMoveTo(l, 1);
        if (strncmp(DLListCurrent(l), "url", 3) != 0) {
            sscanf(DLListCurrent(l), "%d", &s[i]);
            i ++;
        }
        while (isNext(l)) {
            DLListMove(l, 1);
            if (strncmp(DLListCurrent(l), "url", 3) != 0) {
                sscanf(DLListCurrent(l), "%d", &s[i]);
                i ++;
            }
        }
        DLListMoveTo(l, 1);
    }
}
int factorial(int n) {
    if (n > 1) {
        return n * factorial(n - 1);
    }
    return 1;
}

void printUrls(DLList l, int n) {
    DLList visited = newDLList();
    DLListMoveTo(l, 1);
    DLList temp = l;
    while (DLListLength(visited) < n) {
        if (strncmp(DLListCurrent(temp), "url", 3) == 0) {
            if (!strIsIn(visited, DLListCurrent(temp))) {
                DLListAfter(visited, DLListCurrent(temp), 0, 0);
                printf("%s\n", DLListCurrent(temp));
            }
        }
        if (DLListMove(temp, 1) == 1) {
            break;
        }
    }
}