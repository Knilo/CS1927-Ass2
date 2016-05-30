#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "DLList.h"
#include "url.h"

#define BUFSIZE 50

DLList getCollection();

int main (int argc, const char * argv[]) {

    char term[] = "volcano";
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
        showDLList(URLwTerm);
    }
    fclose(inverted);
//calculate IDF for term
    int totalUrls = DLListLength(URLs);
    printf("totalUrls = %d\n", totalUrls);
    int termUrls = DLListLength(URLwTerm);
    printf("termUrls = %d\n", termUrls);
    double toBeIDF = (double)totalUrls/(double)termUrls;
    printf("toBeIDF = %f\n", toBeIDF);
    double IDF = log(toBeIDF);
    printf("IDF = %f\n", IDF);
    return EXIT_SUCCESS;
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