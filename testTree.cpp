/* 
 * File:   testTree.cpp
 * Author: Michael Beckering
 * Project 1
 * CS-4280-001-FS2019
 * 
 * Created on November 10, 2019, 1:54 PM
 */

#include "testTree.h"
#include "node.h"
#include "token.h"
#include <string>
#include <iostream>

using namespace std;

void printPreorder(node *root, int depth){
    if (root == NULL) {
        return;
    }
    //const char* nodeData = root->data.c_str(); // convert node data to const char*
    //if (root->t0 )
    //string data = root->t0.ID + " " + root->t0.tokenInstance + "\n";
    //const char* tokenData = data.c_str();
    //printf("%*c%c:%-9s ", depth*2, ' ' , root->key, nodeData);
    //printf("\n");
    cout << "printPreorder depth = " << depth << "\n";
    printPreorder(root->c0, depth+1);
    printPreorder(root->c1, depth+1);
    printPreorder(root->c2, depth+1);
    printPreorder(root->c3, depth+1);
}