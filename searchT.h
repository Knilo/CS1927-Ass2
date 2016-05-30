//searchT.h


typedef struct Node {
	char *str;
	struct Node *left;
	struct Node *right;
} TNode;

typedef TNode *Tree;

Tree newNode(char *word); // creates new node 
//which can be attached onto a pre existing tree

Tree newTree(); // newTree function returns NULL

void freeTree(Tree t); // frees all malloced node of the tree, 
//the str as well as the struct union

int wordIsInT(Tree t, char *word); //returns 1 if word is in the Tree else 0

Tree insertWordT(Tree t, char *word); // inserts the word in the tree
 
Tree insetUrlT(Tree t, char *url, char *word); // inserts the url in the tree

