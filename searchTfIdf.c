#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "DLList.h"
#include "url.h"

#define BUFSIZE 50

DLList calcTfIdf (char * term);
DLList getCollection();
char* stradd(const char* a, const char* b);
int termf(char *url, char *term); //gets the term frequency in the url
// ".txt" at the end of the url is added in the function
DLList getListOfurls(int argc, char **argv);
// finds the urls that that contain the following strings

// the functions below are used within the functions that are mentioned above
// dont worry about the functions below unless you feel the need to use it
void getUrlStr(char url[], char *str, int start);
// gets the url in the string provided, int start determines where to begin the search
int maxLen(char *fileN); // returns the largest length of the string in a given file
// ".txt" is not added in the function, must be done prior
DLList calcMultipleTerm(DLList a, DLList b);

int main (int argc, const char * argv[]) {
    int i = 0;
    if (argc > 1) {
        DLList TfIdfFinal = newDLList();
        if (argc == 2) {

            char * term = malloc(sizeof(argv[1]));
            strcpy(term, argv[1]);

            TfIdfFinal = calcTfIdf(term);
        }
        if (argc == 3) {

            char * term = malloc(sizeof(argv[1]));
            strcpy(term, argv[1]);

            DLList TfIdf1 = newDLList();
            TfIdf1 = calcTfIdf(term);

            term = malloc(sizeof(argv[2]));
            strcpy(term, argv[2]);

            DLList TfIdf2 = newDLList();
            TfIdf2 = calcTfIdf(term);


            TfIdfFinal = calcMultipleTerm(TfIdf1, TfIdf2);

        }
        if (argc > 3) {
            char * term = malloc(sizeof(argv[1]));

            strcpy(term, argv[1]);

            DLList TfIdf1 = newDLList();
            TfIdf1 = calcTfIdf(term);
            //DLList TfIdfFinal = newDLList();

            for (i = 2; argv[i] != NULL; i++) {

                //printf("i = %d", i);
                term = malloc(sizeof(argv[i]));

                strcpy(term, argv[i]);

                DLList TfIdf2 = newDLList();

                TfIdf2 = calcTfIdf(term);

                if (i == 2) {
                    TfIdfFinal = calcMultipleTerm(TfIdf1, TfIdf2);
                } else {
                    TfIdfFinal = calcMultipleTerm(TfIdf2, TfIdfFinal);
                }

            }
// now need to order final list

        }
        //printToScreenTfIdf(TfIdfFinal);
        TfIdfFinal = orderByPagerank (TfIdfFinal);
        //puts("");
        printToScreenTfIdf(TfIdfFinal);

    } else {
        fprintf(stderr, "usage: %s term1 term2 term3...\n", argv[0]);
        exit(1);

    }

    return EXIT_SUCCESS;

}

DLList calcTfIdf (char * term) {
    int i;
    char buffer[BUFSIZE];
    int URLcount = 0;
    DLList URLs = getCollection();
    DLList URLwTerm = newDLList();
    FILE * inverted = fopen ("invertedIndex.txt", "r");
//doesn't work for moon
    while (!feof(inverted)) {
        while ((fgets(buffer, sizeof(buffer), inverted) != NULL)) {
            char *cur, link[BUFSIZE];
            cur = buffer;
            if (strstr(cur, term) != NULL) {
                cur = strstr(cur, term);
                while ((cur = nextURL(cur)) != NULL) {
                    getURL(cur, link, BUFSIZE - 1);
                    //getURL(cur, link, BUFSIZE - 1);
                    DLListAfter(URLwTerm, link, 0, 0);

                    URLcount++;
                    //printf("%s, %d\n", link, URLcount);
                    cur += strlen(link);
                }
            }
        }

        //}
        //showDLList(URLwTerm);
    }
    fclose(inverted);
//calculate IDF for term
    int totalUrls = DLListLength(URLs);
    freeDLList(URLs);
    //printf("totalUrls = %d\n", totalUrls);
    int termUrls = DLListLength(URLwTerm);
        if (termUrls == 0) {
        return URLwTerm;
    }

    //printf("termUrls = %d\n", termUrls);
    double toBeIDF = (double)totalUrls / (double)termUrls;
    //printf("toBeIDF = %f\n", toBeIDF);
    double IDF = log(toBeIDF);
    //printf("IDF = %f\n", IDF);
    //calculate tf for each URL in URLwTerm
    for (i = 0; i <= termUrls; i++) {
        DLListMoveTo(URLwTerm, i + 1);
        alterpagerank(URLwTerm, (IDF * termf(DLListCurrent(URLwTerm), term)));

    }
    //printToScreenTfIdf(URLwTerm);

    return URLwTerm;
}



DLList getCollection() {

    DLList URLs = newDLList();
    char buffer[BUFSIZE];
    //int URLcount = 0;
    FILE * collection = fopen ("collection.txt", "r");
    while (!feof(collection)) {
        while ((fgets(buffer, sizeof(buffer), collection) != NULL)) {
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
    fclose(collection);
    return URLs;
}

char* stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) , b);
}

int termf(char *url, char *term) { // finds the term frequency in the url
    char urlTxt[54];
    //printf("url, \"%s\"\n", url);
    strcpy(urlTxt, url);
    strcat(urlTxt, ".txt"); //adds in the .txt so that the file can be opened
    int termf = 0;

    int maxStrLen = maxLen(urlTxt);
    //printf("maxStrLen = %d\n", maxStrLen);
    char *str = malloc(sizeof(char) * (maxStrLen + 1)); // make sure to copy and insert into nodes
    assert(str != NULL);
    char *word = malloc(sizeof(char) * (maxStrLen + 1)); // same as above
    assert(word != NULL);

    FILE *urlFile = fopen(urlTxt, "r");
    assert(urlFile != NULL);
    int j, k;
    while (fgets(str, maxStrLen, urlFile) != NULL && strncmp(str, "#start Section-2", 16) != 0);
    while (fgets(str, maxStrLen, urlFile) != NULL) {
        if (strncmp(str, "#end Section-2", 14) == 0) {
            break;
        }
        for (j = 0; str[j] != '\0'; j ++) {
            if (isalnum(str[j])) {
                for (k = 0; isalnum(str[j]); k ++, j ++) {
                    word[k] = tolower(str[j]);
                }
                word[k] = '\0';
                if (strcmp(term, word) == 0) {
                    termf ++;
                }
            }

        }
    }
    free(str);
    free(word);
    fclose(urlFile);
    return termf;
}
void getUrlStr(char url[], char *str, int start) {
    int i, j;
    for (i = start; str[i] != '\0' && str[i] != '\n'; i ++) {
        if (str[i] == 'u') {
            for (j = 0; isalnum(str[i]); j ++, i ++) { //would url contain just words and numbers??
                url[j] = str[i];
            }
            url[j] = '\0';
            break;
        }
    }
}

DLList getListOfurls(int argc, char **argv) {
    char url[50];
    int maxStrLen = maxLen("invertedIndex.txt");
    char *str = malloc(sizeof(char) * (maxStrLen + 1));
    DLList l = newDLList();
    DLList l2 = newDLList();
    int i, j;
    FILE *indexFp = fopen("invertedIndex.txt", "r");
    assert(indexFp != NULL);
    for (i = 1; i < argc; i ++) {
        while (fgets(str, maxStrLen, indexFp) != NULL) {
            if (strncmp(str, argv[i], strlen(argv[i])) == 0) {
                l = newDLList();
                for (j = 0; str[j] != '\0' && str[j] != '\n'; j ++) {
                    if (str[j] == 'u') {
                        getUrlStr(url, str, j);
                        DLListAfter(l, url, 0, 0);
                    }
                }
                if (DLListIsEmpty(l)) { // The word does not exist in the index
                    return 0; // Hence there will be no output and the program exits early
                }
                l2 = cmpLists(l, l2);
                break;
            }
        }
        fclose(indexFp);
        indexFp = fopen("invertedIndex.txt", "r");
        assert(indexFp != NULL);
    }
    fclose(indexFp);

    return l2;
}
int maxLen(char *fileN) {
    //printf("fileN, \"%s\"\n", fileN);
    FILE *fp = fopen(fileN, "r");
    assert(fp != NULL);
    int count = 1;
    int maxStrLen = 0;
    char c = fgetc(fp);
    while (c != EOF) { // check if this really works to get the maxStrLen;
        for (count = 1; c != EOF && c != '\n' && c != '\0'; count ++) {
            c = fgetc(fp);
        }
        if (maxStrLen < count) {
            maxStrLen = count;
        }
        c = fgetc(fp);
    }
    fclose(fp);
    return maxStrLen;
}

DLList calcMultipleTerm(DLList a, DLList b) {

    if (DLListIsEmpty(b)) {
        return a;
    } else if (DLListIsEmpty(a)) {
        return b;
    }
    //puts("1");
    DLList new = newDLList();
    //DLListMoveTo(a, 1);

//    DLListMoveTo(b, 1);
    int i = 0;
    int j = 0;
    int used = 0;
    /*
    puts("a:");
    printToScreenTfIdf(a);
    puts("b:");
    printToScreenTfIdf(b);
    */
    for (i = 0; i < DLListLength(a); i++) {

        DLListMoveTo(a, i + 1);
        used = 0;
        for (j = 0; j <= DLListLength(b); j++) {
            DLListMoveTo(b, j + 1);
            //if b is in a, sum tfidf and add to new
            if (strcmp(DLListCurrent(a), DLListCurrent(b)) == 0) {
                DLListAfter(new, DLListCurrent(a), 0, 0);
                alterpagerank(new, ((double)getpagerank(a) + (double)getpagerank(b)));
                //puts("new:");
                //printToScreenTfIdf(new);
                used = 1;
                break;
            }
        }
        //DLListMove(b, 1);
        if (used == 0) {
            DLListAfter(new, DLListCurrent(a), 0, 0);
            alterpagerank(new, getpagerank(a));
            //puts("newhiiiii:");
            //printToScreenTfIdf(new);
        }
    }

    for (i = 0; i <= DLListLength(b); i++) {

        DLListMoveTo(b, i + 1);
        used = 0;
        for (j = 0; j <= DLListLength(new); j++) {
            //puts("4");
            DLListMoveTo(new, j + 1);
            if (strcmp(DLListCurrent(b), DLListCurrent(new)) == 0) {
                used = 1;
            }
        }
        if (used == 0) {
            DLListAfter(new, DLListCurrent(b), 0, 0);
            alterpagerank(new, getpagerank(b));
        }


    }

    //puts("new:");
    //printToScreenTfIdf(new);
    freeDLList(a);
    freeDLList(b);

    return new;
}