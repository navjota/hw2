/*****************************************
 * ** YOU SHOULD NOT MODIFY THIS FILE **
 *****************************************/

/*
 * COMP1917: Computing 1, UNSW
 * Project 2: Image Processing using Quadtrees
 * File: quadtree.c
 * Written by: Alan Blair and Maurice Pagnucco
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "quadtree.h"

// Print command line prompt
void printPrompt(void) 
{
   if( isatty(fileno(stdin))) { // output goes to screen
     printf("Enter (A,I,P,F,B,D,L,R,M,NE,NW,SW,SE,O,U,Q, H for Help): ");
   }
   else {
     printf(">\n");
   }
}

// Print image buffer
static void printBuffer(char *buffer, int dim)
{
  int i,j;

  //printf("Printing %d x %d image\n", dim, dim);
  for( i=0; i < dim; i++ ) {
    for( j=0; j < dim; j++ ) {
      putchar(buffer[(i * dim) + j]);
    }
    putchar('\n');
  }
}

// Create image from quad tree and store it in buffer
static void createBuffer(
                         QTnode *qt,
                         char *buffer,
                         int rstart,
                         int cstart,
                         int qsize,
                         int dim
                        )
{
  int i,j;

  if( qt->type ) { // leaf node
    for( i = rstart; i < rstart + qsize; i++ ) {
      for( j = cstart; j < cstart + qsize; j++ ) {
        buffer[(i * dim) + j] = qt->type;
      }
    }
  }
  else { // recursively look at each subtree
    createBuffer(qt->ne,buffer,rstart, cstart+(qsize/2),qsize/2, dim);
    createBuffer(qt->nw,buffer,rstart, cstart, qsize/2, dim);
    createBuffer(qt->sw,buffer,rstart+(qsize/2),cstart, qsize/2, dim);
    createBuffer(qt->se,buffer,rstart+(qsize/2),cstart+(qsize/2),qsize/2,dim);
  }
}

// Print image stored in quad tree data structure
void printImage(QTnode *qt, int dim)
{
  char *buffer;
  int i;

  // reserve memory for file contents
  if((buffer = (char *) malloc(dim * dim)) == NULL) {
    printf("Error - out of memory.\n");
    exit(1);
  }
  // intialise buffer
  for( i = 0; i < dim * dim; buffer[i++] = ' ' )
    ;
  // decode quad tree into image buffer
  createBuffer(qt, buffer, 0, 0, dim, dim);
  printBuffer(buffer, dim); // print buffer
  free(buffer);
}

// Read image from file into buffer
static char *getBuffer(FILE *fp, int dim)
{
  char *buffer, c;
  int i=0,j=0;
  int isize; // dim of image and image size
             //(dim assumed to be n x n where n is a power of 2)

  isize = dim * dim; // size of image in pixels

  //printf("Reading image of size %d and dim %d\n", isize, dim);

  if((buffer = (char *) malloc(isize + 1)) == NULL) {
    // reserve memory for file contents
    printf("Error - out of memory\n");
    exit(1);
  }
  while((c = fgetc(fp)) != EOF && i < isize) {
    // store pixels in buffer
    if( c == '\n' ) {
      while( j < dim ) {
        buffer[i++] = ' ';
        j++;
      }
      j=0;
    }
    else {
      buffer[i++] = c;
      j++;
    }
  }
  while(i < isize) { // fill in additional elements as needed
    buffer[i++] = ' ';
  }
  buffer[i] = '\0'; // terminate string
                    // not required but we can print buffer for error checking
  return(buffer);
}

// Determine whether quadrant at rstart, cstart of qsize pixels is a leaf
// of the quad tree (i.e., all pixels are black or all pixels are white)
char checkQuadrant(char *buffer, int rstart, int cstart, int qsize, int dim)
{
    int i, j;

    for( i = rstart; i < (rstart + qsize); i++ ) {
      for( j = cstart; j < (cstart + qsize); j++ ) {
        if(buffer[(i * dim) + j] != buffer[(rstart * dim) + cstart]) {
          return(INTERNAL);
        }
      }
    }
    return(buffer[(rstart * dim) + cstart]);
}

// Create quad tree from image stored in buffer
QTnode *getqtree(char *buffer, int rstart, int cstart, int qsize, int dim)
{
    QTnode *qt = (QTnode *)malloc(sizeof(QTnode));

    if( qt == NULL ) {
      printf("Error: out of memory\n");
      exit(1);
    }
    // initialise sub-trees
    qt->ne = qt->nw = qt->sw = qt->se = NULL;
   
    if(!(qt->type = checkQuadrant(buffer, rstart, cstart, qsize, dim))) {
      qt->ne = getqtree(buffer,rstart, cstart + (qsize/2), qsize/2, dim);
      qt->nw = getqtree(buffer,rstart, cstart, qsize/2, dim);
      qt->sw = getqtree(buffer,rstart+(qsize/2),cstart, qsize/2, dim);
      qt->se = getqtree(buffer,rstart+(qsize/2),cstart+(qsize/2),qsize/2,dim);
      qt->ne->out = qt;
      qt->nw->out = qt;
      qt->se->out = qt;
      qt->sw->out = qt;
    }
    qt->out = NULL; // initialize parent

    return(qt);
}

// Read image from filename and return its dims
QTnode *getImage( char *filename, int *dim )
{
    FILE *fp;
    char *buffer;
    QTnode *qt;
    int ch;
    int d=1,i=0,j=0;

    if((fp = fopen(filename, "r")) == NULL) {
      printf("Error - file could not be opened: %s\n",filename);
      return(NULL);
    }

    while(( ch = getc(fp)) != EOF ) {
      if( ch == '\n' ) {
        j++;
        if( i > d ) {
          d *= 2;
        }
        i=0;
      }
      else {
        i++;
      }
    }
    if( j > d ) {
      d *= 2;
    }

    if((fp = fopen(filename, "r")) == NULL)
    {
        printf("Error - file could not be opened: %s\n",filename);
        return(NULL);
    }
    buffer = getBuffer(fp,d);
    fclose(fp);

    qt = getqtree( buffer, 0, 0, d, d );
    free( buffer );

    *dim = d;

    return( qt );
}

