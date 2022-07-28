/*
AUTHOR: Evan Albers
DATE CREATED: 14 July 2022
SUMMARY: Header file for PolarNash
*/

#include <vector>

using namespace std;

/* structure to represent a point in two dimensional space */
typedef struct _point2D {

    float x, y;

}point2D;

/* structure to represent an individual player. Records player best response function, their best response, and their neighbors */

typedef float (*brf_pointer) (float, float, float);

typedef struct _playerNode {

    int player_number;

    _playerNode *left, *right;

    brf_pointer bestResponseFunc;

    int bestResponse;

    point2D* left_msg;

    point2D* right_msg;

}node;

typedef struct _gamePath {

    node firstNode;

    int length;

}path;

/* used to construct a node */
node* createNode(node* l, node* r, float brf(float, float, float));

/* Generates the message to be sent to the right neighbor */
void generateSegments(node* n);

/* "Sends" a message to a node's right neighbor */
void setLeftMsg(vector<point2D> msg);

/* sets the right msg of a node */ 
void setRightMsg(vector<float> msg, node* n);

/* sets a node's right node */
void setRight(node* n, node new_node);

/* sets a node's left node */
void setLeft(node* n, node new_node);

/* sets a node's BRF */
void setBRF(node* n, float brf(float, float, float));

/* handles the first node's message it will send */
void firstNodeCase(node* n);

/* handles case for middle nodes */
void middleNodeCase(node* n);

/* determines the play of the last node */
void lastNodeCase(node* n);

/* handles graph case 1 */
void graphCaseOne(float u_top, float u_bottom, float w_left, float w_right);

/* handles graph case 2 */
void graphCaseTwo(float u_top, float u_bottom);

/* handles graph case 3 */
void graphCaseThree(float w_left, float w_right);

/* handles graph case 4 */
void graphCaseFour(node* n);




