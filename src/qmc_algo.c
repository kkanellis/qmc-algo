/**********************************************************************
 *		Quine - McCluskey Algorithm Implementation in C Language 
 **********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define DELIMITER ','
#define PARSE_DELIMETERS " ,"

#define EXPRESSION_MAX_LENGTH 1001

#define VARIABLES_MAX 20

typedef char bool;

/***********************************************************************
 * SORTED LIST IMPLEMENTATION 
 **********************************************************************/
struct lnode{
	unsigned int id;
	struct lnode * next;
	struct lnode * prev;
};
typedef struct lnode lnodeT;

void list_init(lnodeT **root, unsigned int id){
	/* Alocate and initialize the new node */
	lnodeT *newElement = (lnodeT *)malloc( sizeof(lnodeT) );
	newElement->id = id;
	newElement->next = newElement->prev = NULL;
	
	*root = newElement;
}

void list_insert(lnodeT *root, unsigned int id){
	/* Allocate and initialize the new node */
	lnodeT *newElement = (lnodeT *)malloc( sizeof(lnodeT) );
	newElement->id = id;
	newElement->next = newElement->prev = NULL;
	
	/* Find the proper position to insert the newElement */
	lnodeT * curr = root;
	while(curr->next != NULL){
		if(curr->next->id > newElement->id)
			break;
		
		curr = curr->next;
	}
	
	/* Insert the newElement updating the next and previous nodes */
	if( curr->next != NULL ){
		curr->next->prev = newElement;
		newElement->next = curr->next;
		newElement->prev = curr;
		curr->next = newElement;
	}
	else{ /* Last element on the list */
		newElement->prev = curr;
		curr->next = newElement;
	}
}

void list_merge(lnodeT **newRoot, lnodeT *firstRoot, lnodeT *secondRoot ){
	list_init(newRoot, firstRoot->id );
	
	/* Insert all the nodes of the first list */
	lnodeT *curr = firstRoot->next;
	while( curr != NULL ){
		list_insert( *newRoot, curr->id );
		curr = curr->next;
	}
	
	/* Insert all the nodes of the second list */
	curr = secondRoot;
	while( curr != NULL ){
		list_insert( *newRoot, curr->id );
		curr = curr->next;
	}
}

bool list_equal(lnodeT *firstRoot, lnodeT *secondRoot){
	lnodeT *fNode = firstRoot;
	lnodeT *sNode = secondRoot;
	while( fNode != NULL ){
		/* Not equal lists */
		if(fNode->id != sNode->id)
			return 0;
		
		fNode = fNode->next;
		sNode = sNode->next;
	}
	/* Lists are equal */
	return 1;
}

void list_print(lnodeT *root){
	lnodeT *curr = root;
	while(curr != NULL){
		printf("%2d ",curr->id);
		curr = curr->next;
	}
}
/** *******************************************************************/


/***********************************************************************
 * GROUP OF MINTERMS STRUCTURE 
 **********************************************************************/
struct mintermGroup{
	char *repr;
	lnodeT* root;
	unsigned int cPosBits;
};
typedef struct mintermGroup mintermGroupT;
/** *******************************************************************/

/***********************************************************************
 * Helper Functions
 **********************************************************************/ 
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
/**********************************************************************/

/***********************************************************************
 * Fundamental Functions
 **********************************************************************/
void CountTerms(char *exp, int *cTerms){
	*cTerms = 1;
	
	char *ptr = strchr(exp,DELIMITER);
	while(ptr != NULL){
		(*cTerms)++;
		
		ptr = strchr(ptr + 1, DELIMITER);
	}
}

void ReadInput(int *cMinterms,char *exp_minterms){
	printf("\n***********************************\n");
	printf("**** Quine-McCluskey Algorithm ****\n");
	printf("***********************************\n\n");
	
	char printfFormat[15];
	sprintf(printfFormat,"%%%d[^\n]s",EXPRESSION_MAX_LENGTH - 1);
	
	printf("Please enter the minterms:\n\t>> ");
	scanf(printfFormat, exp_minterms);
	
	/* Count the terms inserted by the user */
	CountTerms(exp_minterms, cMinterms);
}

void ParseInput(char *exp, mintermGroupT *mt, int cTerms, int *cVariables){
	int i,j;
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
		
		/* Find the next minterm */
		if(i < cTerms - 1){
			pch = strtok(NULL, PARSE_DELIMETERS);
		}
	}
	
	/* Calculate the number of variables */
	(*cVariables) = 0;
	int binary = 1;
	while( maxMinterm > (binary - 1) ){
		binary = binary * 2;
		(*cVariables)++;
	}
	
	/* Realloc the repr fields to save some (hopefully enough?) memory */
	for(i = 0; i < cTerms; i++){
		mt[i].repr = (char *)realloc( mt[i].repr , (*cVariables + 1) * sizeof(char) );
		mt[i].repr[ *cVariables ] = '\0';
		StrReverse( mt[i].repr );
	}
}

int CompareMinterms( const void *a , const void * b){
	mintermGroupT *first  = (mintermGroupT *)a;
	mintermGroupT *second = (mintermGroupT *)b;
	
	if( first->cPosBits == second->cPosBits ){
		return strcmp( first->repr, second->repr );
	}
	
	return (first->cPosBits - second->cPosBits);
}

bool CanFormGroup(mintermGroupT firstGroup, mintermGroupT secondGroup, int cVariables){
	int i, bitsDiffer = 0;
	for(i = 0; i < cVariables; i++){
		/* If some '-' cant match the corresponding '-' in the other group */ 
		if( (firstGroup.repr[i] == '-' && secondGroup.repr[i] != '-') ||
			(firstGroup.repr[i] != '-' && secondGroup.repr[i] == '-') ){
				return 0;
		}
		/* If the have different bit in the same position (excluding '-') */
		if( firstGroup.repr[i] != secondGroup.repr[i] ){
			bitsDiffer++;
		}
	}
	/* If the differ exactly ONE bit */ 
	return (bitsDiffer == 1) ? 1 : 0;
}

void CreateNewGroupRepr(char *newGroupRepr, char *firstGroupRepr, char *secondGroupRepr, int cVariables){
	int i;
	for(i = 0; i < cVariables; i++){
		if( firstGroupRepr[i] == secondGroupRepr[i] ){
			newGroupRepr[i] = firstGroupRepr[i];
		}
		else{
			newGroupRepr[i] = '-';
		}
	}
	newGroupRepr[cVariables] = '\0';
}
/** *******************************************************************/


int main(int argc, char *argv[]){
	mintermGroupT *table[VARIABLES_MAX + 1]; /* Array of columns */
	bool *termsUsed[VARIABLES_MAX + 1]; /* Boolean 2d array of terms used */
	
	int lenCol[VARIABLES_MAX]; /* Lenght of each column */
	int cVariables; /* Number of Variables */
	int cMinterms;	/* Number of Minterms */
	int cColums;	/* Number of Columns */ 
	
	/* Input sring */
	char exp_minterms[EXPRESSION_MAX_LENGTH];
	
	ReadInput( &cMinterms, exp_minterms);
	table[0] = (mintermGroupT *)malloc( cMinterms * sizeof(mintermGroupT) );
	lenCol[0] = cMinterms;
	
	ParseInput(exp_minterms, table[0], cMinterms, &cVariables);
	
	/* Sort the minterms by the number of ones in their binary represenation */
	qsort( table[0], cMinterms, sizeof(mintermGroupT) , CompareMinterms );
	
	int i,j;
	for( i = 0; i < cVariables; i++){
		
		/* Allocate space and initialize to zero for i-th column of the boolean table */
		termsUsed[i] = (bool *)calloc( lenCol[i], sizeof(bool) );
		
		/* Allocate space for the next column of the table */
		mintermGroupT *nextCol = (mintermGroupT *)malloc( (lenCol[i] * lenCol[i])  * sizeof(mintermGroupT) );
		
		int nextColPos = 0;
		for( j = 0; j < lenCol[i]; j++){
			int k = j+1;
			
			/* Skip the terms that have the same number of ones */
			while( k < lenCol[i] && table[i][k].cPosBits == table[i][j].cPosBits ){
				k++;
			}
			
			while( k < lenCol[i] && (table[i][k].cPosBits - table[i][j].cPosBits) == 1){
				/* Check if two groups can be merged into a larger group */
				if( CanFormGroup( table[i][k], table[i][j], cVariables ) ){
					
					/* Check if the new group has already been created */
					lnodeT *newGroupRoot;
					list_merge( &newGroupRoot, table[i][j].root, table[i][k].root );
					
					bool alreadyInColumn = 0;
					int c;
					for( c = 0; c < nextColPos; c++){
						if( list_equal( nextCol[c].root , newGroupRoot ) != 0 ){
							alreadyInColumn = 1;
							break;
						}
					}
					
					if( !alreadyInColumn ){
						/* Update the new group fields */
						nextCol[nextColPos].root = newGroupRoot;
						nextCol[nextColPos].cPosBits = table[i][j].cPosBits;
						
						/* Create the binary representation of the new group  */
						nextCol[nextColPos].repr = (char *)malloc( (cVariables + 1) * sizeof(char) );
						CreateNewGroupRepr( nextCol[nextColPos].repr, table[i][j].repr, table[i][k].repr, cVariables );
						
						nextColPos++;
					}
					
					/* Mark the these groups as used */
					termsUsed[i][j] = termsUsed[i][k] = 1;
				}
				k++;
			}
		}
		
		lenCol[i+1] = nextColPos;
		
		/* **************************** */
		/* TODO: Prime implicants chart */
		/* **************************** */
		
		/* Groups cannot be merged further into larger ones */
		if( lenCol[i+1] == 0 ){
			break;
		}
		
		/* Allocate the proper size for the next column and copy the terms to it */
		table[i+1] = (mintermGroupT *)malloc( lenCol[i+1] * sizeof(mintermGroupT) );
		for( j = 0; j < lenCol[i+1]; j++){
			table[i+1][j] = nextCol[j];
		}
		
		/* De-allocate the memory used in the termporary array  'nextCol' */
		free(nextCol);
	}
	
	cColums = i;
	
	/* Printing the columns (iterations) */
	printf("\n\n");
	for(j = 0; j <= cColums; j++){
		printf("############### COLUMN %d ###############\n",j);
		for(i = 0; i < lenCol[j]; i++){
			printf("[ ");
			list_print(table[j][i].root);
			printf("]  %s  %s\n", table[j][i].repr, (termsUsed[j][i] ? "OK" : " *") );
		}
		printf("\n\n");
	}
	
	return 0;
}
