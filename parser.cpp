/* 
 * File:   parser.cpp
 * Author: Michael Beckering
 * Project 2
 * CS-4280-001-FS2019
 *
 * Created on November 1, 2019, 9:55 AM
 */

#include "parser.h"
#include <cstdlib>

using namespace std;

// global string array associated to the token.ID enumeration (for printing)
string tokenNames[] = {
        "ID_tk",      // 0
        "NUM_tk",     // 1
        "START_tk",   // 2
        "STOP_tk",    // 3
        "ITERATE_tk", // 4
        "VOID_tk",    // 5
        "VAR_tk",     // 6
        "RETURN_tk",  // 7
        "IN_tk",      // 8
        "OUT_tk",     // 9
        "PROGRAM_tk", // 10
        "COND_tk",    // 11
        "THEN_tk",    // 12
        "LET_tk",     // 13
        "EQUAL_tk",   // 14
        "LESSTHAN_tk",    // 15
        "GREATERTHAN_tk", // 16
        "LESSEQUAL_tk",   // 17
        "GREATEREQUAL_tk",// 18
        "EQUALEQUAL_tk",  // 19
        "COLON_tk",   // 20
        "PLUS_tk",    // 21
        "MINUS_tk",   // 22
        "MULTIPLY_tk",// 23
        "DIVIDE_tk",  // 24
        "MODULUS_tk", // 25
        "PERIOD_tk",  // 26
        "LEFTPAR_tk", // 27
        "RIGHTPAR_tk",// 28
        "COMMA_tk",   // 29
        "LEFTCURLY_tk",   // 30
        "RIGHTCURLY_tk",  // 31
        "SEMICOLON_tk",   // 32
        "LEFTBRACKET_tk", // 33
        "RIGHTBRACKET_tk",// 34
        "EOF_tk"      // 35
    };

// global token for use in recursive descent parse functions
token t;

void parser(ifstream& inFile) {

    // get the first token from the scanner
    t = scanner(inFile);
    // call the first nonterminal's function
    program(inFile);
    
    /* this code prints all tokens after retrieving the first.. save for testing
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
    */

}

void error(ifstream& inFile, token t, string expected) {
    inFile.close();
    cout << "Error: Line " << t.lineNumber << ": received " << tokenNames[t.ID];
    cout << " \"" << t.tokenInstance << "\", expected " << expected << ".\n";
    exit (EXIT_FAILURE);
}

// below is the structure of our recursive descent parser
// each function represents a nonterminal of the same name
// each function accepts an UNCONSUMED token, so it must call
// scanner() to see the token it should expect

void program(ifstream& inFile) {
    vars(inFile);
    block(inFile);
    if (t.ID == EOF_tk) {
        cout << "Parse OK\n";
    }
    else {
        error(inFile, t, tokenNames[EOF_tk]);
    }
    return;
}

void block(ifstream& inFile) {
    if (t.ID == START_tk) {
        t = scanner(inFile);
        vars(inFile);
        stats(inFile);
        if (t.ID == STOP_tk) {
            t = scanner(inFile); // got expected STOP_tk token, consume it
            return; // <block> grammar correct
        }
        else {
            // didn't see expected STOP token
            error(inFile, t, tokenNames[STOP_tk]);
        }
    }
    else {
        // didnt see expected START token
        error(inFile, t, tokenNames[START_tk]);
    }
}

void vars(ifstream& inFile) {
    if (t.ID == VAR_tk) {
        t = scanner(inFile);
        if (t.ID == ID_tk) {
            t = scanner(inFile);
            if (t.ID == COLON_tk) {
                t = scanner(inFile);
                if (t.ID == NUM_tk) {
                    t = scanner(inFile);
                    vars(inFile);
                    return; // grammer for this <vars> production is correct
                }
                else {
                    error(inFile, t, tokenNames[NUM_tk]);
                }
            }
            else {
                error(inFile, t, tokenNames[COLON_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[ID_tk]);
        }
    }
    else {
        // predict empty production for <vars>
        return;
    }
}

void expr(ifstream& inFile) {
    A(inFile);
    if (t.ID == PLUS_tk) {
        t = scanner(inFile);
        expr(inFile);
        return;
    }
    else {
        return; // predict end of <expr>
    }
}

void A(ifstream& inFile) {
    N(inFile);
    if (t.ID == MINUS_tk) {
        t = scanner(inFile);
        A(inFile);
        return;
    }
    else {
        return; // predict end of <A>
    }
}

void N(ifstream& inFile) {
    
}

void M(ifstream& inFile) {
    
}

void R(ifstream& inFile) {
    
}

void stats(ifstream& inFile) {
    
}

void mStat(ifstream& inFile) {
    
}

void stat(ifstream& inFile) {
    
}

void in(ifstream& inFile) {
    
}

void out(ifstream& inFile) {
    
}

void if_stat(ifstream& inFile) {
    
}

void loop(ifstream& inFile) {
    
}

void assign(ifstream& inFile) {
    
}

void RO(ifstream& inFile) {
    
}
