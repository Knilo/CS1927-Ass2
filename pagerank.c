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
#include "Graph.h"
#include "DLList.h"

int main (int argc, const char * argv[]) {



    return EXIT_SUCCESS;
}
//****** switch to linked list

int PageRank(int d, int diffPR, int maxIterations) {

    FILE * collection;
    int URLs[];
    int i;
    int URLcount = 0;

    collection = fopen ("collection.txt", "r");
    for (i = 0;; i++) {
        URL[i] = fgetc(collection);
        URLcount++;
        if (feof(collection)) {
            break;
        }
        //printf("%c", collection);
    }
    fclose(collection);
    Graph webPages = newGraph(URLcount);

    int pR[URLcount];
    for (i = 0; i <= URLcount; i++) {
        pR[i] = (1 / URLcount);
    }
    int iteration;

    for (iteration = 0; iteration < maxIterations && diff >= diffPR; iteration ++) {

        while (i = 0; i < URLcount; i++) {
            char outgoingURL[0x100];

            snprintf(buf, sizeof(outgoingURL), "%s.txt", URL[i]);
            FILE *f = fopen(outgoingURL, "r");
            int j;
            int oURLs[];
            for (j = 0;; j++) {
                oURL[j] = fgetc(outgoingURL);
                if (feof(collection)) {
                    break;
                }
            }
            
            /*       For each url pi in the collection */
            int PR_old = pR[i];
            sum = 0 ;
            for each url pj pointing to pi (ignore self - loops and parallel edges)
                sum = sum + PR(pj) / out - degree - of(pj);

            PR(pi) = (1 - d) / N + d * sum
                     diff = diff + Abs(PR_old - PR(pi);
        }

    }
    outGoingUrls = fopen ("%d.txt", "r", URL[i]);

}
