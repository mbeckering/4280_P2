/* 
 * File:   parser.cpp
 * Author: Michael Beckering
 * Project 2
 * CS-4280-001-FS2019
 *
 * Created on November 1, 2019, 9:55 AM
 */

#include "parser.h"

using namespace std;

void parser(ifstream& inFile) {
    string tokenNames[] = {
        "IdentifierTk",   // 0
        "NumberTk",       // 1
        "KeywordTk",      // 2
        "OperatorTk",     // 3
        "EndOfFileTk"     // 4
    };
    token t = scanner(inFile);
    while (t.ID != EOF_tk) {
        cout << tokenNames[t.ID] << " ";
        cout << t.tokenInstance << " ";
        cout << "Line=" << t.lineNumber << "\n";
        t = scanner(inFile);
    }
    if (t.ID == EOF_tk) {
        cout << tokenNames[t.ID] << " ";
        cout << t.tokenInstance << " ";
        cout << "Line=" << t.lineNumber << "\n";
    }

}