/*

This is the case of the first node, handles relavant duties. 
*/

#include "PolarNash.h"

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

