/*
 * COMP1917: Computing 1
 * Assignment 2: Image Processing using Quadtrees
 * File: hw2.c
 * Written by: Alan Blair
 */

// System include files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Local include files
#include "quadtree.h"

// Constants
#define MAXLENGTH    200


// Main function
int main( void )
{
  char command[MAXLENGTH];
  char c;

 /********************************************************************
  * YOU WILL NEED TO COMPLETE THE FOLLOWING SECTION FOR STAGES 2 - 5 *
  *******************************************************************/

  printPrompt();

  while( fgets( command, MAXLENGTH, stdin ) != NULL ) {

    int  imgNum;
    char *p;

    if(( p=strrchr( command, '\n')) != NULL ) {
      *p = '\0'; // remove '\n' at end of line
    }
    // find the first non-space character in the command
    p = command;
    while(isspace(*p)) {
      p++;
    }
    c = tolower(*p);

    if( isdigit(c)) {
      if( sscanf( command, "%d", &imgNum ) == 1 ) {

        // INSERT CODE FOR <k> COMMAND
      }
    }
    else switch( c ) {

    case 'h': // help

      printf(" A - Add image\n" );
      printf(" I - Index\n" );
      printf(" P - Print image\n" );
      printf(" F - Forward\n" );
      printf(" B - Back\n" );
      printf("<k>- make image number k the current image\n");
      printf(" D - Delete image\n" );
      printf(" L - Look for image\n" );
      printf(" R - Rotate image counterclockwise\n" );
      printf(" M - Mirror image (reflect vertically)\n" );
      printf("NE - zoom into North East corner\n" );
      printf("NW - zoom into North West corner\n" );
      printf("SW - zoom into South West corner\n" );
      printf("SE - zoom into South East corner\n" );
      printf(" O - zoom Out\n" );
      printf(" U - Undo\n" );
      printf(" H - Help\n" );
      printf(" Q - Quit\n" );
      break;

      // INSERT CODE FOR OTHER COMMANDS

    case 'q': // quit program
      printf("Bye!\n");
      return 0;
      break;

    default:
      printf("Unrecognized command: %s\n", command );
      break;
    }

    printPrompt();
  }

  return 0;
}
