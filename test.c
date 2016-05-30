/*
PageRank(d, diffPR, maxIterations)

            Read "web pages" from the collection in file "collection.txt"
            and build a graph structure using Adjacency List Representation

            N = number of urls in the collection
            diff = 0;
            For each url pi in the collection
                PR(pi) = 1/N  ;

            iteration = 0;
            While (iteration < maxIteration AND diff >= diffPR)
                iteration++;
                For each url pi in the collection
                    PR_old = PR(pi);
                    sum = 0 ;
                    for each url pj pointing to pi (ignore self-loops and parallel edges)
                        sum = sum + PR(pj) / out-degree-of(pj);

                    PR(pi) = (1-d)/N + d * sum
                    diff = diff + Abs(PR_old - PR(pi);
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "DLList.h"
#include "url.h"

#define MAXSTRING 50
#define BUFSIZE 50

char* stradd(const char* a, const char* b);
DLList getCollection();
Graph makeGraph(DLList list);

int main (int argc, const char * argv[]) {

    DLList URLs = getCollection();
    Graph webPages = makeGraph(URLs);

    int URLcount = DLListLength(URLs);
    int i;

    int iteration;
    int maxIterations = 100;
    double diffPR = 0.00001;
    double diff = 100;
    char * checkingFrom;
    char * checkingTo;
    //int connections = 0;
    //double connectionsFrom = 0;
    int j = 0;
    double d = 0.85;
    double initPageRank2[URLcount];
    for (iteration = 0; iteration < maxIterations && diff >= diffPR; iteration ++) {

        //get initial pageranks for iteration
        for (i = 0; i <= URLcount; i++) {
            initPageRank2[i] = getPageRank(URLs);
            DLListMoveTo(URLs, i + 1);
        }

        //set diff as 0
        diff = 0;

        //for every i check if anything connects to it
        for (i = 0; i <= URLcount; i++) {
            //connections = 0;
            DLListMoveTo(URLs, i + 1);
            checkingTo = DLListCurrent(URLs);
            //printf("checking to %s\n", checkingTo);
            j = 0;
            double sum = 0;
            for (j = 0; j <= URLcount; j++) {
                DLListMoveTo(URLs, j + 1);
                checkingFrom = DLListCurrent(URLs);
                //counting connections from j to i
                // connections = connections + isConnected(webPages, checkingFrom, checkingTo);
                //}


                //printf("isconnected?? %d\n", connections); //FOR TESTING
                //if (connections > 0) {
                if (isConnected(webPages, checkingFrom, checkingTo)) {
                    //double sum = 0;

                    //counting degree of urls pointing to i
                    //for (j = 0; j <= URLcount; j++) {
                    //  DLListMoveTo(URLs, j + 1);
                    //char * countingFrom = DLListCurrent(URLs);
                    //check all out going links from checkingFrom, sum them, then add that value to pr of the checking to

                    // if (isConnected(webPages, countingFrom, checkingTo)) {

                    sum = sum + (initPageRank2[j] / (double)getDegree(URLs));

                }

            }
            DLListMoveTo(URLs, i + 1);
            alterPageRank(URLs, sum);
            //printf("new prs\n");
            //showDLList(URLs);
        }

    }
    for (i = 0; i <= URLcount; i++) {
        DLListMoveTo(URLs, i + 1);
        alterPageRank(URLs, ((1 - d) / URLcount) + (d * (getPageRank(URLs))));
        diff = diff + fabs(initPageRank2[i] - getPageRank(URLs));


    }
    showDLList(URLs);
    printToFile(URLs);
    return EXIT_SUCCESS;

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
    int URLcount = DLListLength(URLs);
    double initPageRank = 1 / (float)URLcount;
    //printf("initPageRank: %f\n", initPageRank); //FOR TESTING
    DLListMoveTo(URLs, 1);
    for (i = 0; i <= URLcount; i++) {

        alterPageRank (URLs, initPageRank);
        DLListMove(URLs, 1);
    }
    return URLs;
}

Graph makeGraph(DLList list) {
    int i;
    char buffer[BUFSIZE];
    int URLcount = DLListLength(list);
    Graph webPages = newGraph(URLcount);

    for (i = 0; i < URLcount; i++) {
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
                addEdge(webPages, outgoingURL, link);
                cur += strlen(link);
                degree++;
//create linked list with outgoing URLs with it's pagerank divided by number of outgoing URLs
            }

        }
        printf("degree: %d\n", degree);
        alterDegree(list, degree);
        fclose(f);
    }
    return webPages;
}


