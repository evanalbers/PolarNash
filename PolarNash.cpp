/*
AUTHOR: Evan Albers
DATE CREATED: July 15th 2022
SUMMARY: Implementation of the Polar Nash Algorithm
*/

#include "PolarNash.h"

using namespace std;

/* function to initialize a node */
node* createNode(node* l, node* r, float (*brf)(float, float, float)) {

    node* n = (node*) malloc(sizeof(node));

    if (l) {

        n -> player_number = l -> player_number += 1;

    } else {

        n -> player_number = 0;

    }

    n -> left = l;
    n -> right = r;
    n -> bestResponseFunc = *brf;
    n -> bestResponse = 2;
    n -> right_msg = (point2D*) malloc(sizeof(point2D) * ((6 * (n -> player_number)) + 6));


}

/* function that generates segments for some node to be sent to following node 
   DEBUGGED? : N
*/
void generateSegments(node* n) {

    if (!(n -> left)) {

        firstNodeCase(n -> right);

    } else if (!(n -> right)) {
    
        lastNodeCase(n);
        
    } else {

        middleNodeCase(n);

    }
}












