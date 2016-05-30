//inverted.c

//get collection fuction
//stradd function 
// such that i dont have to use 3 libraries

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "searchT.h"

#define MAX_URL_SIZE 50

// use tree implementation, left is the different words, right is the urls 

Tree inverted(char **url, int);

void printTree(Tree t);

char **getUrl(int *); // gets url from collection.txt //use aarons code for this

void freeArray(char **, int size);

int main(void) {
	char **url;
	int count = 0;
	int *numOfUrl = &count;
	url = getUrl(numOfUrl); // get url and put it into the array
	
	Tree t = newTree();
	t = inverted(url, *numOfUrl);
	printTree(t); // alittle different to the orginal showTree
	freeTree(t);
	freeArray(url, *numOfUrl);
	return 0;
}
Tree inverted(char  **url, int urlCount) {
	Tree t = newTree();
	int i, j, k;
	FILE *fp;
	char c;
	int maxStrLen = 1;
	int count;
	char *str;
	char *word;
	char urlFile[MAX_URL_SIZE + 4];

	for (i = 0; i < urlCount ; i ++) {
		strcpy(urlFile, url[i]);
		strcat(urlFile, ".txt");
		fp = fopen(urlFile, "r");
		assert(fp != NULL);
		c = fgetc(fp);
		count = 1;
		while (c != EOF) {
			for (count = 0; c != EOF && c != '\n'; count ++) {
				c = fgetc(fp);
			}
			if (maxStrLen < count) {
				maxStrLen = count;
			}
			c = fgetc(fp);
		}
		str = malloc(sizeof(char) * maxStrLen); // make sure to copy and insert into nodes
		assert(str != NULL);
		word = malloc(sizeof(char) * maxStrLen); // same as above
		assert(word != NULL);
		fclose(fp);

		fp = fopen(urlFile, "r");
		assert(fp != NULL);
		for (fgets(str, maxStrLen, fp); strncmp(str, "#start Section-2", 16) != 0 &&
			str != NULL; fgets(str, maxStrLen, fp));

		while (fgets(str,maxStrLen, fp) != NULL) {
			if (strncmp(str, "#end Section-2", 14) == 0) {
				break;
			}
			for (j = 0; str[j] != '\0'; j ++) {
				if (isalnum(str[j])) {
					for (k = 0; isalnum(str[j]); k ++, j ++) {
						word[k] = tolower(str[j]);
					}
					word[k] = '\0';
					if (!wordIsInT(t, word)) {
						t = insertWordT(t, word);
						t = insertUrlT(t, url[i], word);
					} else {
						t = insertUrlT(t, url[i], word); 
					}
				}
				
			}
		}
		free(str);
		free(word);
		fclose(fp);
	}
	return t;
}
void printTree(Tree t) { //prints the 'tree' into the invertedIndex.txt
	Tree temp1 = t;
	Tree temp2 = t;
	FILE *fp = fopen("invertedIndex.txt", "w");
	assert(fp != NULL);
	assert(t != NULL);
	while (temp1 != NULL) {
		fprintf(fp, "%s", temp1->str);
		temp2 = temp1->right;
		while (temp2 != NULL) {
			fprintf(fp, " %s", temp2->str);
			temp2 = temp2->right;
		}
		fprintf(fp, "\n");
		temp1 = temp1->left;
	}
	fclose(fp);
}

char **getUrl(int *numOfUrl) { // gets url from collection.txt
	FILE *colFp = fopen("collection.txt", "r");
	int urlCount = *numOfUrl;
	assert(colFp != NULL);
	char c;
	char **url;
	int i, j;
	
	for (c = fgetc(colFp); c != EOF; c = fgetc(colFp)) {
		if (c == 'u') {
			c = fgetc(colFp);
			if (c =='r') {
				c = fgetc(colFp);
				if (c == 'l') {
					urlCount ++;
				}
			}
		}
	}
	url = malloc(sizeof(char *) * (urlCount)); // change to be specific
	assert(url != NULL);
	for (i = 0; i < urlCount; i ++) {
		url[i] = malloc(sizeof(char) * MAX_URL_SIZE);
		assert(url[i] != NULL);
	}
	fclose(colFp);
	colFp = fopen("collection.txt", "r");
	assert(colFp != NULL);
	i = 0;
	while ((c = fgetc(colFp)) != EOF) {
		if (isalnum(c)) {
			for (j = 0; (!isspace(c)) && c != EOF && c != '\0'; j ++, c = fgetc(colFp)) {
				url[i][j] = c;
			}
			url[i][j] = '\0';
			i ++;
		}
	}
	*numOfUrl = urlCount;
	return url;
} 
void freeArray(char **array, int size){
	int i;
	for (i = 0; i < size; i ++) {
		free(array[i]);
	}
	free(array);
}
