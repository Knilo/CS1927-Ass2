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
#include "Graph.h"
#include "DLList.h"
#include "url.h"

#define MAXSTRING 50
#define BUFSIZE 50

DLList PageRank(int d, int diffPR, int maxIterations);
char* stradd(const char* a, const char* b);

int main (int argc, const char * argv[]) {

    if (argc > 3) {
        float dampingfactor =  atoi(argv[1]);
        float diffPR = atoi(argv[2]);
        int iterations = atoi(argv[3]);
    } else {
        fprintf(stderr, "Usage: %s dampingfactor diffPR iterations\n", argv[0]);
        exit(1);
    }
    DLList PageRanks = PageRank(dampingfactor, diffPR, iterations);
    showDLList(PageRanks);

    return EXIT_SUCCESS;
}

DLList PageRank(int d, int diffPR, int maxIterations) {

    int i;
    DLList URLs = newDLList();
    char buffer[BUFSIZE];
    int URLcount = 0;
    FILE * collection = fopen ("collection.txt", "r");
    while (!feof(collection)) {
        while ((fgets(buffer, sizeof(buffer), collection) != NULL)) {
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) {

                getURL(cur, link, BUFSIZE - 1);
                DLListAfter(URLs, link, 0);
                //showDLList(URLs);
                URLcount++;
                printf("%s, %d\n", link, URLcount);
                cur += strlen(link);
            }
        }
    }
    fclose(collection);
    int p = DLListLength(URLs);
    /* FOR TESTING
    printf("length of list: %d\n", p);
    showDLList(URLs);
    DLListMoveTo(URLs, 1);
    //char *curr = DLListCurrent(URLs);
    //printf("%s\n", curr);
    */
    double initPageRank = 1 / (float)p;
    //printf("initPageRank: %f\n", initPageRank); //FOR TESTING
    for (i = 0; i <= URLcount; i++) {
        alterPageRank (URLs, initPageRank);
        DLListMove(URLs, 1);
    }
    // showDLList(URLs); //FOR TESTING


    Graph webPages = newGraph(URLcount);

    int iteration;
    for (iteration = 0; iteration < maxIterations && diff >= diffPR; iteration ++) {
        diff = 0;
        for (i = 0; i < URLcount; i++) {
            DLListMoveTo(URLs, i + 1);
            char * outgoingURL = DLListCurrent(URLs);
            outgoingURL = stradd(outgoingURL, ".txt");
            //printf("%s\n", outgoingURL); //FOR TESTING
            FILE *f = fopen(outgoingURL, "r");
//create linked list with outgoing URLs with it's pagerank divided by number of outgoing URLs
        }
    }
}


    char* stradd(const char* a, const char* b) {
        size_t len = strlen(a) + strlen(b);
        char *ret = (char*)malloc(len * sizeof(char) + 1);
        *ret = '\0';
        return strcat(strcat(ret, a) , b);
    }



