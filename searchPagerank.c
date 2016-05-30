// searchPagerank.c
#include <stdio.h>
#include <stdlib.h>

//#include ".h" //use DDList.h DDList.c

int main(int argc, char **argv) {
	int i;
	if (argc == 1) {
		return 0;
	}
	List l = newList() // change depending on the .c .h file used
	FILE *indexFp = fopen("invertedIndex.txt", "r");
	FILE *pageRkFp = fopen("pagerank.txt", "r");

	//find argv[1] in invertedIndex.txt 
	//add the url's into a list

	//find argv[2] in invertedIndex.txt
	//using the urls of the corresponding argv[2],
	//if the url is in the List of urls made previously 
	//then make a new list and add it to the list 
	//else nothing, repeat till no more command line arguments 
	for (i = 2; i < argc; i ++) {

	} 

	//using pageRkFp compare the first url from pageRkFp with the urls in the list
	// if the url from pageRkFp is in the list print it to stdout
	// finish when after 10 urls are printed or when there is no more urls left

}
