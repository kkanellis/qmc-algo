#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define DELIMITER ','
#define PARSE_DELIMETERS " ,"

#define EXPRESSION_MAX_LENGTH 1001

#define VARIABLES_MAX 32
#define MINTERMS_MAX (1<<32)

/** *******************************************************************/
/** SORTED LIST IMPLEMENTATION 
/** *******************************************************************/
struct lnode{
	unsigned int id;
	struct lnode * next;
	struct lnode * prev;
};
typedef struct lnode lnodeT;

void list_insert(lnodeT *root, unsigned int id){
	lnodeT *newElement = (lnodeT *)malloc( sizeof(lnodeT) );
	newElement->id = id;
	newElement->next = newElement->prev = NULL;
	
	/** List is empty **/
	/*if(root == NULL){
		root = newElement;
		return;
	}*/
	
	/** Find the proper position to insert the newElement **/
	lnodeT * curr = root;
	while(curr->next != NULL){
		if(curr->next->id > newElement->id)
			break;
		
		curr = curr->next;
	}
	
	/** Insert the newElement updating the next and previous nodes **/
	if( curr->next != NULL ){
		curr->next->prev = newElement;
		newElement->next = curr->next;
		newElement->prev = curr;
		curr->next = newElement;
	}
	else{ /** Last element on the list **/
		newElement->prev = curr;
		curr->next = newElement;
	}
}

void list_init(lnodeT **root, unsigned int id){
	lnodeT *newElement = (lnodeT *)malloc( sizeof(lnodeT) );
	newElement->id = id;
	newElement->next = newElement->prev = NULL;
	
	*root = newElement;
}
/** *******************************************************************/


/** *******************************************************************/
/** GROUP OF MINTERMS STRUCTURE 
/** *******************************************************************/
struct mintermGroup{
	char *repr;
	lnodeT* root;
	unsigned int cPosBits;
};
typedef struct mintermGroup mintermGroupT;
/** *******************************************************************/


int max(int a,int b){
	return a > b ? a : b;
}

void StrReverse(char *s){
	int low = 0;
	int high = strlen(s) - 1;
	while(low < high){
		char c = s[low];
		s[low] = s[high];
		s[high] = c;
		
		low++;high--;
	}
}
/** *******************************************************************/
void CountTerms(char *exp, int *cTerms){
	*cTerms = 1;
	
	char *ptr = strchr(exp,DELIMITER);
	while(ptr != NULL){
		(*cTerms)++;
		
		ptr = strchr(ptr + 1, DELIMITER);
	}
}

void ReadInput(int *cMinterms,char *exp_minterms){
	
	char printfFormat[15];
	sprintf(printfFormat,"%%%d[^\n]s",EXPRESSION_MAX_LENGTH - 1);
	
	/** TODO: Pretty print **/
	printf("Enter the minterms:\n\t>> ");
	scanf(printfFormat, exp_minterms);
	
	CountTerms(exp_minterms, cMinterms);
}


void ParseInput(char *exp, mintermGroupT *mt, int cTerms, int *cVariables){
	int i,j,binaryLen;
	int maxMinterm = 0;
	
	char * pch = strtok(exp, PARSE_DELIMETERS);
	for(i = 0; i < cTerms; i++){
		/* Initialize the number bits that are one as zero */ 
		mt[i].cPosBits = 0;
		
		/* Add the minterm to the list */
		mt[i].root = NULL;
		int id = atoi(pch);
		list_init(&mt[i].root, id);
		
		/* Find the minterm with the greatest number of bits */
		maxMinterm = max(maxMinterm, id );
		
		/* Construct the binary representation of the minterm */
		/* NOTE: It is constructed in the reverse order */
		char *binaryRepr = (char *)malloc( (VARIABLES_MAX + 1) * sizeof(char) );
		for(j = 0; j < VARIABLES_MAX; j++){
			if( (id & (1 << j) ) != 0){
				binaryRepr[j] = '1';
				mt[i].cPosBits++;
			}
			else{
				binaryRepr[j] = '0';
			}
		}
		binaryRepr[VARIABLES_MAX] = '\0';
		
		/* Copy the binary representation to the repr field */
		mt[i].repr = (char *)malloc( (VARIABLES_MAX + 1) * sizeof(char) );
		strcpy(mt[i].repr, binaryRepr);
		
		if(i < cTerms - 1){
			pch = strtok(NULL, PARSE_DELIMETERS);
		}
	}
	
	/* Calculate the number of Variables */
	(*cVariables) = (int)(ceil( log2( (double)(maxMinterm)) ));
	
	/* Realloc the repr fields to save some (hopefully enough?) memory */
	char * prev;
	for(i = 0; i < cTerms; i++){
		mt[i].repr = (char *)realloc( mt[i].repr , (*cVariables + 1) * sizeof(char) );
		mt[i].repr[ *cVariables ] = '\0';
		StrReverse( mt[i].repr );
	}
}
/** *******************************************************************/

int main(int argc, char *argv[]){
	mintermGroupT *m;
	int cVariables;
	int cMinterms;
	
	char exp_minterms[EXPRESSION_MAX_LENGTH];
	
	ReadInput( &cMinterms, exp_minterms);
	m = (mintermGroupT *)malloc( cMinterms * sizeof(mintermGroupT) );
	
	ParseInput(exp_minterms, m, cMinterms, &cVariables);
	
	int i;
	for(i = 0; i < cMinterms; i++){
		printf("[%d] [%d]\t%s\n",i,m[i].root->id,m[i].repr);
	}
	
	return 0;
}
