//inverted.c new implementation
// find the max CHAR_LEN of the url text file, then use it to malloc a block of mem 
// with its size, as well as the str to compare with.
// array of words each with urls

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tree.h"

// use tree implementation, left is the different words, right is the urls 

void inverted(char **url, Tree t);

int main(int argc, char **argv) {
	char **url; // change depending on the implementation
	url = getUrl(); // get url and put it into the array
	Tree t = newTree();
	t = inverted(url, t);
	showTree(t); // alittle different to the orginal showTree
	freeTree(t);
}
Tree inverted(char **url, Tree t) {
	Tree t = newTree(); // tree.h
	int i, j, k;
	FILE *fp;
	char c;
	int maxStrLen = 1;
	int count;
	char *str;
	char *word;

	for (i = 0; url[i] != NULL; i ++) {
		fp = fopen(url[i], "r");
		c = fgetc(fp);
		count = 1;
		while (c != EOF) {
			for (count = 0; (c = fgetc(fp)) != '\0'; count ++);
			if (maxStrLen < count) {
				maxStrLen = count;
			}
		}
		str = malloc(sizeof(char) * maxStrLen); // make sure to copy and insert into nodes
		word = malloc(sizeof(char) * maxStrLen); // same as above
		fclose(fp);

		fp = fopen(url[i], "r");
		for (fgets(str, maxStrLen, fp); strchr(str, "#start Section-2") != NULL &&
			str != NULL; fgets(str, maxStrLen, fp));

		while (fgets(str,maxStrLen, fp) != NULL) {
			for (j = 0; str[j] != '\0'; j ++) {
				for (k = 0; !isspace(str[j]); k ++, j ++) {
					word[k] = tolower(str[j]);
				}
				word[k] = '\0';
				if (!wordIsInT(t, word)) { // make function
					insertWordT(t, word); //make function
					insetUrlT(t, url[i], word); // make function
				}
				
			}
		}
		free(str);
		free(word);
		fclose(fp);
	}

}
