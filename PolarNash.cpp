/*
AUTHOR: Evan Albers
DATE CREATED: July 15th 2022
SUMMARY: Implementation of the Polar Nash Algorithm
*/

#include "PolarNash.h"
#include <vector>

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


/* function to handle the case of the first node in the path 
   DEBUGGED? : N
*/
void firstNodeCase(node* node) {

    //generating values for each payoff possibility.
    float p_one_zero = node -> bestResponseFunc(1, 0, 0);
    float p_zero_one = node -> bestResponseFunc(0, 1, 0);
    float p_one_one = node -> bestResponseFunc(1, 1, 0);
    float p_zero_zero = node -> bestResponseFunc(0, 0, 0);

    //calculating vstar
    float v_star = (p_one_zero - p_zero_zero) / (p_zero_one - p_zero_zero - p_one_one + p_one_zero);

    //initializing two points for vertical part of s curve
    point2D breakpoint_top = {1, v_star};
    point2D breakpoint_bottom = {0, v_star};

    //declaring other points for message, and vector for message
    point2D v_lt_vstar; //for when v less than vstar
    point2D v_gt_vstar; //for when v greater than vstar
    int counter = 0;
    //determining which way the s is oriented; if p_one_one is greater, will be regular s, else will be a reversed s. Pushing points onto vector accordingly
    if (p_one_one > p_one_zero) {

        v_lt_vstar = {0, 0};
        v_gt_vstar = {1, 1};
        node -> right_msg[counter] = v_lt_vstar;
        node -> right_msg[++counter] = breakpoint_bottom;
        node -> right_msg[++counter] = breakpoint_top;
        node -> right_msg[++counter] = v_gt_vstar;

    } else {

        v_lt_vstar = {0, 1};
        v_gt_vstar = {1, 0};
        node -> right_msg[counter] = v_lt_vstar;
        node -> right_msg[++counter] = breakpoint_top;
        node -> right_msg[++counter] = breakpoint_bottom;
        node -> right_msg[++counter] = v_gt_vstar;

    }

    //"sending" message

    node -> right -> left_msg = node -> right_msg;
}

/* function to handle middle node cases 
    DEBUGGED? : N
*/
void middleNodeCase(node* node) {

    //constructing float coefficients as function of payoffs
    float a = node -> bestResponseFunc(1, 0, 0) + node -> bestResponseFunc(0, 1, 0)
             + node -> bestResponseFunc(0, 0, 0) - node -> bestResponseFunc(1, 1, 0);

    float b = node -> bestResponseFunc(0, 0, 0) - node -> bestResponseFunc(0, 1, 0);

    float c = node -> bestResponseFunc(1, 1, 1) - node -> bestResponseFunc(1, 0, 1) 
              - node -> bestResponseFunc(1, 1, 0) + node -> bestResponseFunc(1, 0, 0)
              - node -> bestResponseFunc(0, 1, 1) + node -> bestResponseFunc(0, 1, 1) 
              + node -> bestResponseFunc(0, 1, 0) - node -> bestResponseFunc(0, 0, 0);
    
    float d = node -> bestResponseFunc(0, 1, 0) - node -> bestResponseFunc(0, 0, 1) 
              - node -> bestResponseFunc(0, 1, 0) + node -> bestResponseFunc(0, 0, 0); 

    //finding where values of curves at lines that border the message box: w = 0, w = 1, u = 0, u = 1
    float u_one = (d - b) / (a - c);
    float u_not = (-b)/a;
    float w_not = b/d;
    float w_one = (a + b) / (c + d);

    int counter = 0;
    //determining which of five cases is present
    //if all four points in range, both curves must be present

    if ((0 <= u_one <= 1) && (0 <= u_not <= 1) && (0 <= w_not <= 1) && (0 <= w_one <= 1)) {



    
    } else if ((0 <= u_one <= 1) && (0 <= u_not <= 1)) //if this case, only top and bottom being intersected, one curve present
    {

        point2D a = {0, u_not};
        point2D b = {1, u_not};
        node -> right_msg[counter] = a;
        node -> right_msg[++counter] = b;

    } else if ((0 <= w_not <= 1) && (0 <= w_one <= 1)) //only l and r present, one curve present
    {


    } else //must be zero curves present, no intersection points, will never just have one? what about case where just L and top? or some combination? 
    {

        //if region is positive, v should play one always , else play zero
        if (((0.5 * d - b) / (0.5 * a - c) - 0.5) > 0) {

            node -> right_msg[counter] = (point2D) {0, 1};
            node -> right_msg[++counter] = (point2D) {1, 1};

        } else {

            node -> right_msg[counter] = (point2D) {0, 0};
            node -> right_msg[counter] = (point2D) {1, 0};

        }
    }

    //send the message to right node
    node -> right -> left_msg = node -> right_msg;

}



/* primative that returns 1 if some segment is vertical, 0 otherwise
   DEBUGGED? : N
 */
int isVert(point2D a, point2D b) {

    return (!(a.x - b.x) && (a.y - b.y));

}

void findBreakpoints(node* n, float* bps) {

    point2D* msg_ptr = n -> left_msg;

    while ((msg_ptr)) {

        if (isVert(*msg_ptr, *(++msg_ptr))) {

            *bps = (*msg_ptr).x;
            msg_ptr++;
            msg_ptr++;

        }

    }

}

void fourCurveCase(node* n, float a, float b, float c, float d) {

    float* breakpoints = (float*) malloc(sizeof(float) * n -> player_number);

    findBreakpoints(n, breakpoints);

    float p = 0;
    float q;
    int counter = 0;

    //handling v = 0 here
    if (d > 0) {

                n -> right_msg[counter] = (point2D) {0, 0};
                n -> right_msg[++counter] = (point2D) {(-b)/a, 0};

    } else {

                n -> right_msg[counter] = (point2D) {(-b)/a, 0};
                n -> right_msg[++counter] = (point2D) {1, 0};
    }

    while (breakpoints) {

        q = *breakpoints;

        float u_move = findValue(n, q);

        //handling the interval case
        if (u_move == (float) 1) {

            n -> right_msg[++counter] = (point2D) {(a+b)/(c+d), 0};
            n -> right_msg[++counter] = (point2D) {(a+b)/(c+d), q};

        } else {

            n -> right_msg[++counter] = (point2D) {(a+b)/(c+d), q};
            n -> right_msg[++counter] = (point2D) {(a+b)/(c+d), 1};

        }

        //handling v = breakpoint



    }

    //handling v = 1
    if ((c + d) > 0) {

        n -> right_msg[++counter] = (point2D) {(a+b)/(c+d), 1};
        n -> right_msg[++counter] = (point2D) {1, 1};

    } else {

        n -> right_msg[++counter] = (point2D) {0, 1};
        n -> right_msg[++counter] = (point2D) {(a+b)/(c+d), 1};

    }

}

int liesOn(point2D a, point2D b, float c) {

    return (a.x < c <= b.x);

}


float findValue(node* n, float f) {

    point2D* segs = n -> left_msg;
    
    while (segs) {

        if (liesOn(*segs, *++segs, f)) {

            return (*segs).y;

        }

    }

}

void oneCurveCase(node* n, float a, float b, float c, float d) {

    float u_one = (d - b) / (a - c);
    float u_not = (-b)/a;
    float w_not = b/d;
    float w_one = (a + b) / (c + d);
    int counter = 0;

    if ((0 <= u_one <= 1) && (0 <= u_not <= 1)) //if this case, only top and bottom being intersected, one curve present
    {

        point2D a = {0, u_not};
        point2D b = {1, u_not};
        n -> right_msg[counter] = a;
        n -> right_msg[++counter] = b;

    }

    else if ((0 <= w_not <= 1) && (0 <= w_one <= 1))  //horizontal curve case
    { 

        

    }
    else if ((0 <= u_one <= 1) && (0 <= w_one <= 1)) //top right curve case
    {

        //will want to determine which side is pos/neg, then fix response as one or zero accordingly. 

    }
    else if ((0 <= w_not <= 1) && (0 <= u_not <= 1)) //bottom left curve case
    {



    }


}



