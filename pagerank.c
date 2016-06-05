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
    if (argc > 3) { //check for correct amount of inputs and assign to variables
        dampingfactor =  atof(argv[1]);
        diffPR = atof(argv[2]);
        maxIterations = atoi(argv[3]);
    } else { //if incorrect number of inputs prints wrror message
        fprintf(stderr, "Usage: %s 'dampingfactor' 'diffPR' 'iterations'\n", argv[0]);
        exit(1);
    }
    pageRank(dampingfactor, diffPR, maxIterations); //call function to determinf pagerank

    return EXIT_SUCCESS;
}

void pageRank(double d, double diffPR, int maxIterations) {

    DLList URLs = getCollection(); //calls function to read collection.txt and create a linked list of all the URLs in the system
    Graph webPages = makeGraph(URLs); //creates a graph based on the linked list of URLs

    int URLcount = DLListLength(URLs); //determines the amount of URLs
    int i;
    int j = 0;
    int iteration = 0; //sets iteration to 0
    double diff = diffPR; //assigns diffPR to diff
    char * checkingFrom;
    char * checkingTo;
    double sum = 0; //sum set intially to 0

    double initPageRank2[URLcount];
    for (iteration = 0; iteration < maxIterations && diff >= diffPR; iteration ++) {

        diff = 0; //set diff as 0

        //for every i check if anything connects to it
        for (i = 0; i < URLcount; i++) { //first for checks for connections towards the URL
            DLListMoveTo(URLs, i + 1);
            checkingTo = DLListCurrent(URLs);
            sum = 0;
            for (j = 0; j < URLcount; j++) { //second for cycles through rest of the URLs to see what connects to original
                DLListMoveTo(URLs, j + 1);
                checkingFrom = DLListCurrent(URLs);
                if ((isConnected(webPages, checkingFrom, checkingTo)) && (strcmp(checkingTo, checkingFrom) != 0)) { //is the URL connected to it?

                    sum = (sum + (getpagerank(URLs) / getDegree(URLs))); //adds the pagerank of the connecting URL divided by the degree of the connecting URL to the sum
                }
                else {
                    continue;
                }

            }
            DLListMoveTo(URLs, i + 1); //returns to the URL we were checking to
            alterpagerank(URLs, (((1 - d) / URLcount) + (d * (sum)))); //updates the pagerank according to the formula
            diff = diff + fabs(initPageRank2[i] - getpagerank(URLs)); //updates diff
        }
    }
    URLs = orderByPagerank(URLs); //calls function to order URLs by pagerank
    printToFileP(URLs);  //calls function to print the URLs and pagerank to the file
    freeDLList(URLs); //frees the linked list
    disposeGraph(webPages); //frees the graph
}
//This function is used to append ".txt" to the url name - helper function
char* stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) , b);
}
//this functions opens collection.txt to find the Urls in the system
DLList getCollection() {
    int i;
    DLList URLs = newDLList(); //creates an empty list
    char buffer[BUFSIZE];
    FILE * collection = fopen ("collection.txt", "r"); //open file for reading
    while (!feof(collection)) { //continues reading until end of file
        while ((fgets(buffer, sizeof(buffer), collection) != NULL)) { //adds line to buffer
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) { //jumps to characters "url" in line
                getURL(cur, link, BUFSIZE - 1); //normalises url
                DLListAfter(URLs, link, 0, 0); //add to list
                cur += strlen(link);
            }
        }
    }
    fclose(collection); //close collection.txt
    float URLcount = DLListLength(URLs); //gets amount of URLs
    double initPageRank = (1 / (float)URLcount); //determines initial pagerank
    for (i = 0; i <= URLcount; i++) { //this for loop sets all the pageranks to inital value
        DLListMoveTo(URLs, i + 1);
        alterpagerank (URLs, initPageRank);
    }
    return URLs;
}
//this function turn a linked list of URLs into a properly connected graph and calculates outward degree
Graph makeGraph(DLList list) {
    int i;
    char buffer[BUFSIZE];
    int URLcount = DLListLength(list); //gets amount of URLs
    Graph webPages = newGraph(URLcount); //creates empty graph

    for (i = 0; i <= URLcount; i++) { //this for loop opens all the corresponding urlx.txt files and reads the URLs it connects to and adds edges in the graph for those links
        int degree = 0; //initally degree set as 0
        DLListMoveTo(list, i + 1);
        char * outgoingURL = DLListCurrent(list); //reads in url to open
        char * outgoingURLtext = stradd(outgoingURL, ".txt"); //add .txt to urlx
        FILE *f = fopen(outgoingURLtext, "r"); //opens url for reading
        while ((fgets(buffer, sizeof(buffer), f) != NULL)) { //reads in line
            char *cur, link[BUFSIZE];
            cur = buffer;
            while ((cur = nextURL(cur)) != NULL) { //finds occurance if "url..."
                getURL(cur, link, BUFSIZE - 1); //normalises url names
                if (!isConnected(webPages, outgoingURL, link)) { //if it isn't already connected in the graph adds an edge
                    addEdge(webPages, outgoingURL, link); //adds edges
                    degree++; //increases degree
                }
                cur += strlen(link);
            }
        }
        alterDegree(list, degree); //sets degree as counted in loop
        fclose(f); //close the file
    }
    return webPages;
}
