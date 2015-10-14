/******************************************************************************
 * ** YOU SHOULD NOT MODIFY THIS FILE **
 ******************************************************************************/

/*
 * COMP1917: Computing 1, UNSW
 * Project 2: Image Processing using Quadtrees
 * File: quadtree.h
 * Written by: Alan Blair and Maurice Pagnucco
 */

#define INTERNAL   0

// Quad tree data structure
typedef struct qtnode QTnode;

struct qtnode {
    char   type;// ' ','*' or INTERNAL
    QTnode *ne; // north east quadrant
    QTnode *nw; // north west quadrant
    QTnode *sw; // south west quadrant
    QTnode *se; // south east quadrant
    QTnode *out;// zoomed-out image
};

// Function prototypes

// Print prompt
extern void printPrompt(void);

// get image from file
QTnode *getImage( char *filename, int *dim );

// Print image to screen
void printImage( QTnode *qtree, int dim );
