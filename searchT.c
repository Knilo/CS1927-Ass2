// tree.c
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "searchT.h"


Tree newNodeTree(char *line)
{
	Tree new = malloc(sizeof(TNode));
	assert(new != NULL);
	new->str = malloc(sizeof(char) * strlen(line));
	assert(new->str != NULL);
	strcpy(new->str, line);
	new->left = new->right = NULL;
	return new;
}

Tree newTree() {
	return NULL;
}

void freeTree(Tree t) { // check if this frees all malloced memory
	if (t != NULL) {
		freeTree(t->left);
		freeTree(t->right);
		free(t->str);
		free(t);
	}
}
int wordIsInT(Tree t, char *word) {
	int retval = 0;
	while (t != NULL) {
		if (strcmp(t->str, word) == 0) {
			retval = 1;
			break;
		} else {
			t = t->left;
		}
	}
	return retval;
}

Tree insertWordT(Tree t, char *word) {
	Tree temp = t;
	if (t == NULL) {
		temp = newNodeTree(word);
	} else {
		while (temp->left != NULL) {
			temp = temp->left;
		}
		temp->left = newNodeTree(word);
	}
	return t;
}

Tree insetUrlT(Tree t, char *url, char *word) {
	Tree x = t;
	Tree y = t;
	while (x != NULL) {
		if (strcmp(x->str, word) == 0) {
			y = x;
			while (y->right != NULL) {
				y = y->right;
			}
			y->right = newNodeTree(url);
		}
		x = x->left;
	}
	return t;
}
