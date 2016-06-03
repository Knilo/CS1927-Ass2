// part 1-A of assignment 2 COMP1927 searchPagerank.c
// By : Dae Ro Lee (Justin) z5060887 and Aaron Schneider z5020001
// Date of completion 25/05/2016

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "DLList.h"
#include "url.h"

#define BUFSIZE 50

void pageRank(double d, double diffPR, int maxIterations);
char* stradd(const char* a, const char* b);
DLList getCollection();
Graph makeGraph(DLList list);

int main (int argc, const char * argv[]) {

    double dampingfactor = 0;
    double diffPR = 0;
    int maxIterations = 0;
    if (argc > 3) { 
        dampingfactor =  atof(argv[1]);
        diffPR = atof(argv[2]);
        maxIterations = atoi(argv[3]);
    } else {
        fprintf(stderr, "Usage: %s 'dampingfactor' 'diffPR' 'iterations'\n", argv[0]);
        exit(1);
    }
    pageRank(dampingfactor, diffPR, maxIterations);

    return EXIT_SUCCESS;
}

void pageRank(double d, double diffPR, int maxIterations) {

    DLList URLs = getCollection();
    Graph webPages = makeGraph(URLs);
    //showGraph(webPages, 0);

    int URLcount = DLListLength(URLs);
    int i;
    int j = 0;
    int iteration = 0;
    double diff = diffPR;
    char * checkingFrom;
    char * checkingTo;
    double sum = 0;

    double initPageRank2[URLcount];
    for (iteration = 0; iteration < maxIterations && diff >= diffPR; iteration ++) {

        //get initial pageranks for iteration
        for (i = 0; i < URLcount; i++) {
            DLListMoveTo(URLs, i + 1);
            initPageRank2[i] = getpagerank(URLs);

        }
        /* FOR TESTING
        printf("printing initPageRank2\n");
        for (i = 0; i < (sizeof (initPageRank2) / sizeof (initPageRank2[0])); i++) {
            printf("    %f\n", initPageRank2[i]);
        }
        printf("finished printing\n");
        */

        //set diff as 0
        diff = 0;

        //for every i check if anything connects to it
        for (i = 0; i < URLcount; i++) {

            DLListMoveTo(URLs, i + 1);
            checkingTo = DLListCurrent(URLs);
            printf("checkingTo: %s\n", checkingTo);

            sum = 0;
            for (j = 0; j < URLcount; j++) {
                DLListMoveTo(URLs, j + 1);
                checkingFrom = DLListCurrent(URLs);
                printf("    checkingFrom: %s\n", checkingFrom);

                if ((isConnected(webPages, checkingFrom, checkingTo)) && (strcmp(checkingTo, checkingFrom) != 0)) {
                    printf("        connected\n");
                    sum = (sum + (/*initPageRank2[j]*/getpagerank(URLs) / getDegree(URLs)));
                }
                else {
                    continue;
                }

            }
            DLListMoveTo(URLs, i + 1);
            //printf("checking: %d\n", i+1); // FOR TESTING
            alterpagerank(URLs, (((1 - d) / URLcount) + (d * (sum))));
            //printf("    old: %f\n", initPageRank2[i]); //FOR TESTING
            //printf("    new: %f\n", getPageRank(URLs)); //FOR TESTING
            diff = diff + fabs(initPageRank2[i] - getpagerank(URLs));
            //printf("    diff: %f\n", diff);
        }
        puts("");
        printf("iteration: %d, diff: %f\n", iteration, diff);
        URLs = orderByPagerank(URLs);
        showDLList(URLs);

    }

    //printf("%d\n", iteration); FOR TESTING
    //showDLList(URLs); FOR TESTING
    URLs = orderByPagerank(URLs);
    printToFileP(URLs);
    freeDLList(URLs);
    disposeGraph(webPages);
}

char* stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) , b);
}

DLList getCollection() {
    int i;
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
    float URLcount = DLListLength(URLs);
    double initPageRank = (1 / (float)URLcount);
    //printf("initPageRank: %f\n", initPageRank); //FOR TESTING

    for (i = 0; i <= URLcount; i++) {
        DLListMoveTo(URLs, i + 1);
        alterpagerank (URLs, initPageRank);

    }
    return URLs;
}

Graph makeGraph(DLList list) {
    int i;
    char buffer[BUFSIZE];
    int URLcount = DLListLength(list);
    Graph webPages = newGraph(URLcount);

    for (i = 0; i <= URLcount; i++) {
        int degree = 0;
        DLListMoveTo(list, i + 1);
        char * outgoingURL = DLListCurrent(list);
        char * outgoingURLtext = stradd(outgoingURL, ".txt");
        //printf("%s\n", outgoingURL); //FOR TESTING
        FILE *f = fopen(outgoingURLtext, "r");
        while ((fgets(buffer, sizeof(buffer), f) != NULL)) {
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) {
                getURL(cur, link, BUFSIZE - 1);
                //printf("%s\n", link);
                if (!isConnected(webPages, outgoingURL, link)) {
                    addEdge(webPages, outgoingURL, link);
                }
                cur += strlen(link);
                degree++;
//create linked list with outgoing URLs with it's pagerank divided by number of outgoing URLs
            }

        }
        //printf("degree: %d\n", degree); FOR TESTING
        alterDegree(list, degree);
        fclose(f);
    }
    return webPages;
}
