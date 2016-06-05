// part 2 of assignment 2 COMP1927 searchPagerank.c
// By : Dae Ro Lee (Justin) z5060887 and Aaron Schneider z5020001
// Date of completion 29/05/2016


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
int termf(char *url, char *term);
//DLList getListOfurls(int argc, char **argv);
void getUrlStr(char url[], char *str, int start);
int maxLen(char *fileN);
DLList calcMultipleTerm(DLList a, DLList b);

int main (int argc, const char * argv[]) {
    int i = 0;
    DLList TfIdfFinal = newDLList(); // creates list to be returned
    if (argc > 1) { //checks the amount of inputs/terms
        if (argc == 2) { //if there is one term

            char * term = malloc(sizeof(argv[1])); //create empty string of correct size
            strcpy(term, argv[1]); //copy term from commandline to string

            TfIdfFinal = calcTfIdf(term); //function calculates TfIdf
            free(term);
        }
        if (argc == 3) { //if there are 2 terms

            char * term = malloc(sizeof(argv[1])); //create empty string of correct size for first term
            strcpy(term, argv[1]); //copy first term from commandline to string
            DLList TfIdf1 = newDLList(); //creates list for urls containg first term
            TfIdf1 = calcTfIdf(term); //calls function to claculate the TfIdf values for term 1

            char * term2 = malloc(sizeof(argv[2])); //create empty string of correct size for second term
            strcpy(term, argv[2]); //copy seconf term from commandline to string
            DLList TfIdf2 = newDLList(); //creates list for urls containg second term
            TfIdf2 = calcTfIdf(term); //calls function to claculate the TfIdf values for term 2
            TfIdfFinal = calcMultipleTerm(TfIdf1, TfIdf2); //uses the two lists to create one final lists of just urls with both terms
            free(term); //free term
            free(term2); //free term 2

        }
        if (argc > 3) { //if there are more than 2 terms
            //first do first term
            char * term = malloc(sizeof(argv[1]));  //create empty string of correct size for first term
            strcpy(term, argv[1]); //copy first term from commandline to string
            DLList TfIdf1 = newDLList(); //creates list for urls containg first term
            TfIdf1 = calcTfIdf(term); //calls function to claculate the TfIdf values for term 1

            for (i = 2; argv[i] != NULL; i++) { //for all other terms
                free(term); //free old term
                term = malloc(sizeof(argv[i])); //create empty string of correct size for first term
                strcpy(term, argv[i]); //copy corresponding term from commandline to string
                DLList TfIdf2 = newDLList(); //creates list for urls containg term
                TfIdf2 = calcTfIdf(term); //calls function to claculate the TfIdf values for term

                if (i == 2) { //if this is second term
                    TfIdfFinal = calcMultipleTerm(TfIdf1, TfIdf2);
                } else { //if this is any other term
                    TfIdfFinal = calcMultipleTerm(TfIdf2, TfIdfFinal);
                }

            }
            free(term);
        }
        TfIdfFinal = orderByPagerank (TfIdfFinal); //calls function to order by pagerank
        printToScreenTfIdf(TfIdfFinal); //calls function to print to screen
    } else { //if input is incorrect print error
        fprintf(stderr, "usage: %s term1 term2 term3...\n", argv[0]);
        exit(1);

    }
    free(TfIdfFinal); //free final list
    return EXIT_SUCCESS;
}
//function to calculate tfidf values for a term and put them in linked list
DLList calcTfIdf (char * term) {
    int i;
    char buffer[BUFSIZE];
    DLList URLs = getCollection(); //calls function to open collectio.txt and collect all urls in colection
    DLList URLwTerm = newDLList(); //creates empty list to contain urls with the term
    FILE * inverted = fopen ("invertedIndex.txt", "r"); //opens invertedIndex.txt
    while (!feof(inverted)) { //reads until end of file
        while ((fgets(buffer, sizeof(buffer), inverted) != NULL)) { //gets line
            char *cur, link[BUFSIZE];
            cur = buffer;
            if (strstr(cur, term) != NULL) { //checks if the line is the line for the term
                cur = strstr(cur, term); //sets cur to correct line
                while ((cur = nextURL(cur)) != NULL) { //gets all urls from the line
                    getURL(cur, link, BUFSIZE - 1);
                    DLListAfter(URLwTerm, link, 0, 0); //adds to list with terms
                    cur += strlen(link);
                }
            }
        }
    }
    fclose(inverted); //close the file
    int totalUrls = DLListLength(URLs); //counts total urls in system
    freeDLList(URLs); //frees the list with all urls
    int termUrls = DLListLength(URLwTerm); //counts urls containing term
    if (termUrls == 0) { //if there are none returns empty list
        return URLwTerm;
    }
    double toBeIDF = (double)totalUrls / (double)termUrls; //finds number to be logged for Idf
    double IDF = log10(toBeIDF); //logs the number
    for (i = 0; i < termUrls; i++) { //calculate tf for each URL in URLwTerm by calling function which counts term frequency
        DLListMoveTo(URLwTerm, i + 1);
        alterpagerank(URLwTerm, (IDF * termf(DLListCurrent(URLwTerm), term)));

    }
    return URLwTerm;
}
//this functions opens collection.txt to find the Urls in the system
DLList getCollection() {
    DLList URLs = newDLList(); //creates empty list
    char buffer[BUFSIZE];
    FILE * collection = fopen ("collection.txt", "r"); //opens collection.txt for reading
    while (!feof(collection)) { //reads until end of file
        while ((fgets(buffer, sizeof(buffer), collection) != NULL)) { //reads line
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) { //finds occurence of urlx in line
                getURL(cur, link, BUFSIZE - 1); //normalises url name
                DLListAfter(URLs, link, 0, 0); //adds to list
                cur += strlen(link);
            }
        }
    }
    fclose(collection); //closes file
    return URLs;
}

char* stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) , b);
}
//gets the term frequency in the url and ".txt" at the end of the url is added in the function
int termf(char *url, char *term) { // finds the term frequency in the url
    char urlTxt[54];
    strcpy(urlTxt, url);
    strcat(urlTxt, ".txt"); //adds in the .txt so that the file can be opened
    int termf = 0;
    int maxStrLen = maxLen(urlTxt); //finds max string length
    char *str = malloc(sizeof(char) * (maxStrLen + 1)); // make sure to copy and insert into nodes
    assert(str != NULL);
    char *word = malloc(sizeof(char) * (maxStrLen + 1)); // same as above
    assert(word != NULL);

    FILE *urlFile = fopen(urlTxt, "r"); //opens urlfile for reading
    assert(urlFile != NULL);
    int j, k;
    //jumps to section 2
    while (fgets(str, maxStrLen, urlFile) != NULL && strncmp(str, "#start Section-2", 16) != 0);
    while (fgets(str, maxStrLen, urlFile) != NULL) {
        if (strncmp(str, "#end Section-2", 14) == 0) {
            break;
        }
        //this loop makes everything lower case and counts frequency of term
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
// gets the url in the string provided, int start determines where to begin the search
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
/*
// finds the urls that that contain the following strings
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
}*/
// returns the largest length of the string in a given file. ".txt" is not added in the function, must be done prior
int maxLen(char *fileN) {

    FILE *fp = fopen(fileN, "r");
    assert(fp != NULL);
    int count = 1;
    int maxStrLen = 0;
    char c = fgetc(fp);
    while (c != EOF) {
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
//this function taske in multiple lists of tfidf values and combines them into one list with only urls that have both terms
DLList calcMultipleTerm(DLList a, DLList b) {
    DLList new = newDLList(); //creates empty list which will be returned

    //first checks for one of the lists being empty, if so returns empty list

    if (DLListIsEmpty(b) || DLListIsEmpty(b)) {
        freeDLList(a);
        freeDLList(b);
        return (new);
    }
    int i = 0;
    int j = 0;
    int used = 0; //variable used in case returning all values not just terms that conicide
    for (i = 0; i < DLListLength(a); i++) { //checks through first list
        DLListMoveTo(a, i + 1);
        used = 0;
        for (j = 0; j < DLListLength(b); j++) { //compares to all values in second list
            DLListMoveTo(b, j + 1);
            //if b is in a, sum tfidf and add to new
            if (strcmp(DLListCurrent(a), DLListCurrent(b)) == 0) {
                DLListAfter(new, DLListCurrent(a), 0, 0);
                alterpagerank(new, ((double)getpagerank(a) + (double)getpagerank(b)));
                used = 1; //url used
                break;
            }
        }
        //this code is to add all urls not just terms that coincide
        //DLListMove(b, 1);
        /*
        if (used == 0) {
            DLListAfter(new, DLListCurrent(a), 0, 0);
            alterpagerank(new, getpagerank(a));
            //printToScreenTfIdf(new);
        }

        }

        for (i = 0; i < DLListLength(b); i++) {

        DLListMoveTo(b, i + 1);
        used = 0;
        for (j = 0; j <= DLListLength(new); j++) {
            DLListMoveTo(new, j + 1);
            if (strcmp(DLListCurrent(b), DLListCurrent(new)) == 0) {
                used = 1;
            }
        }
        if (used == 0) {
            DLListAfter(new, DLListCurrent(b), 0, 0);
            alterpagerank(new, getpagerank(b));
        }
        */

    }
    freeDLList(a); //free original lists
    freeDLList(b);
    return new;
}