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
    return;
}

void error(ifstream& inFile, token t, string expected) {
    inFile.close();
    cout << "Error: Line " << t.lineNumber << ": received " << tokenNames[t.ID];
    cout << " \"" << t.tokenInstance << "\", expected " << expected << ".\n";
    exit (EXIT_FAILURE);
}

// printToken is used for testing
void printToken() {
    cout << "Current token: " << tokenNames[t.ID] <<
            " , token instance " << t.tokenInstance << "\n";
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
    M(inFile);
    if (t.ID == DIVIDE_tk) {
        t = scanner(inFile);
        N(inFile);
        return;
    }
    else if (t.ID == MULTIPLY_tk) {
        t = scanner(inFile);
        N(inFile);
        return;
    }
    else {
        return; // predict end of <N>
    }
}

void M(ifstream& inFile) {
    if (t.ID == MINUS_tk) {
        t = scanner(inFile);
        M(inFile);
        return;
    }
    else {
        R(inFile);
        return;
    }
}

void R(ifstream& inFile) {
    if (t.ID == LEFTBRACKET_tk){
        t = scanner(inFile);
        expr(inFile);
        if (t.ID == RIGHTBRACKET_tk) {
            t = scanner(inFile);
            return;
        }
        else {
            error(inFile, t, tokenNames[RIGHTBRACKET_tk]);
        }
    }
    else if (t.ID == ID_tk) {
        t = scanner(inFile);
        return;
    }
    else if (t.ID == NUM_tk) {
        t = scanner(inFile);
        return;
    }
    else {
        error(inFile, t, "[<expr>], ID_tk, or NUM_tk");
    }
}

void stats(ifstream& inFile) {
    stat(inFile);
    if (t.ID == SEMICOLON_tk) {
        t = scanner(inFile);
        mStat(inFile);
        return;
    }
    else {
        error(inFile, t, tokenNames[SEMICOLON_tk]);
    }
}

void mStat(ifstream& inFile) {
    // check if the next token is a production of <stat>
    if (t.ID == IN_tk ||
            t.ID == OUT_tk ||
            t.ID == START_tk ||
            t.ID == COND_tk ||
            t.ID == ITERATE_tk ||
            t.ID == ID_tk) {
        // if the next token is a production of <stat>, 
        // predict <stat> ; <mStat> production for <mStat>
        stat(inFile);
        if (t.ID == SEMICOLON_tk) {
            t = scanner(inFile);
            mStat(inFile);
            return;
        }
        else {
            error(inFile, t, tokenNames[SEMICOLON_tk]);
        }
    }
    else {
        return; // predict empty production for <mStat>
    }
}

void stat(ifstream& inFile) {
    if (t.ID == IN_tk) {
        in(inFile);
        return;
    }
    else if (t.ID == OUT_tk) {
        out(inFile);
        return;
    }
    else if (t.ID == START_tk) {
        block(inFile);
        return;
    }
    else if (t.ID == COND_tk) {
        if_stat(inFile);
        return;
    }
    else if (t.ID == ITERATE_tk) {
        loop(inFile);
        return;
    }
    else if (t.ID == ID_tk) {
        assign(inFile);
        return;
    }
    else {
        error(inFile, t, " IN_tk, OUT_tk, START_tk, COND_tk,"
        " ITERATE_tk, or ID_tk");
    }
}

void in(ifstream& inFile) {
    if (t.ID == IN_tk) {
        t = scanner(inFile);
        if (t.ID == ID_tk) {
            t = scanner(inFile);
            return;
        }
        else {
            error(inFile, t, tokenNames[ID_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[IN_tk]);
    }
}

void out(ifstream& inFile) {
    if (t.ID == OUT_tk) {
        t = scanner(inFile);
        expr(inFile);
        return;
    }
    else {
        error(inFile, t, tokenNames[OUT_tk]);
    }
}

void if_stat(ifstream& inFile) {
    if (t.ID == COND_tk) {
        t = scanner(inFile);
        if (t.ID == LEFTPAR_tk) {
            t = scanner(inFile);
            if (t.ID == LEFTPAR_tk) {
                t = scanner(inFile);
                expr(inFile);
                RO(inFile);
                expr(inFile);
                if (t.ID == RIGHTPAR_tk) {
                    t = scanner(inFile);
                    if (t.ID == RIGHTPAR_tk) {
                        t = scanner(inFile);
                        stat(inFile);
                        return;
                    }
                    else {
                        error(inFile, t, tokenNames[RIGHTPAR_tk]);
                    }
                }
                else {
                    error(inFile, t, tokenNames[RIGHTPAR_tk]);
                }
            }
            else {
                error(inFile, t, tokenNames[LEFTPAR_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[LEFTPAR_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[COND_tk]);
    }
}

void loop(ifstream& inFile) {
     if (t.ID == ITERATE_tk) {
        t = scanner(inFile);
        if (t.ID == LEFTPAR_tk) {
            t = scanner(inFile);
            if (t.ID == LEFTPAR_tk) {
                t = scanner(inFile);
                expr(inFile);
                RO(inFile);
                expr(inFile);
                if (t.ID == RIGHTPAR_tk) {
                    t = scanner(inFile);
                    if (t.ID == RIGHTPAR_tk) {
                        t = scanner(inFile);
                        stat(inFile);
                        return;
                    }
                    else {
                        error(inFile, t, tokenNames[RIGHTPAR_tk]);
                    }
                }
                else {
                    error(inFile, t, tokenNames[RIGHTPAR_tk]);
                }
            }
            else {
                error(inFile, t, tokenNames[LEFTPAR_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[LEFTPAR_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[ITERATE_tk]);
    }
}

void assign(ifstream& inFile) {
    if (t.ID == ID_tk) {
        t = scanner(inFile);
        if (t.ID == LESSTHAN_tk) {
            t = scanner(inFile);
            if (t.ID == LESSTHAN_tk) {
                t = scanner(inFile);
                expr(inFile);
            }
            else {
                error(inFile, t, tokenNames[LESSTHAN_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[LESSTHAN_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[ID_tk]);
    }
}

void RO(ifstream& inFile) {
    if (t.ID == LESSTHAN_tk) {
        t = scanner(inFile);
        if (t.ID == LESSTHAN_tk) { // detected "< <"
            t = scanner(inFile);
            return;
        }
        else if (t.ID == GREATERTHAN_tk) { // detected "< >"
            t = scanner(inFile);
            return;
        }
        else { // detected "<"
            return;
        }
    }
    else if (t.ID == GREATERTHAN_tk) {
        t = scanner(inFile);
        if (t.ID == GREATERTHAN_tk) { // detected "> >"
            t = scanner(inFile);
            return;
        }
        else { // detected ">"
            return;
        }
    }
    else if (t.ID == EQUAL_tk) {
        t = scanner(inFile);
        return;
    }
    else {
        error(inFile, t, "LESSTHAN_tk, GREATERTHAN_tk, or EQUAL_tk");
    }

}
