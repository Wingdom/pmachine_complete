/*                                  Parser                                    */
/*                               Programmed by                                */
/*                     Brandon Couts and Cameron Yeager                       */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUGGER 1
#define INPUT_FILE "lexi.txt"
#define OUTPUTFILE "parser - completed.txt"

#define MAX_SYMBOL_TABLE_SIZE 256
/*
typedef enum { 
    nulsym = 1,         // NULL
    identsym = 2,       // Used before a varible
    numbersym = 3,      // Used before a number
    plussym = 4,        // +
    minussym = 5,       // -
    multsym = 6,        // *
    slashsym = 7,       // /
    oddsym = 8,         // odd
    eqsym = 9,          // =
    neqsym = 10,        // !=
    lessym = 11,        // <
    leqsym = 12,        // <=
    gtrsym = 13,        // >
    geqsym = 14,        // >=
    lparentsym = 15,    // (
    rparentsym = 16,    // )
    commasym = 17,      // ,
    semicolonsym = 18,  // ;
    periodsym = 19,     // .
    becomessym = 20,    // :=
    beginsym = 21,      // begin
    endsym = 22,        // end
    ifsym = 23,         // if 
    thensym = 24,       // then 
    whilesym = 25,      // while
    dosym = 26,         // do
    callsym = 27,       // call
    constsym = 28,      // const
    intsym = 29,        // int
    procsym = 30,       // procedure
    writesym = 31,      // write
    readsym = 32        // read
} token_type;
*/


/* For constants, you must store kind, name and value.
   For variables, you must store kind, name, L and M.
   For procedures, you must store kind, name, L and M. */

struct symbol{ 
	int kind; 		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value) 
	int level; 		// L level
	int addr; 		// M address
}; 

struct token {
    int id;
    int value;
};

FILE * file;
struct token marble;
struct symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

void getToken();
void printToken(struct token marble);

void error(int error_code);

int program();
int block();
int constDeclaration();
int varDeclaration();
int statement();
int condition();
int relOP();
int expression();
int term();
int factor();
int number();
int ident();
int digit();
int letter();

int main () {           
    getToken();
    program();
    
    system("PAUSE");
    return 0;
}

void error() {
/*  1.	Use = instead of :=.
    2.	= must be followed by a number.
    3.	Identifier must be followed by =.
    4.	const, var, procedure must be followed by identifier.
    5.	Semicolon or comma missing.
    6.	Incorrect symbol after procedure declaration.
    7.	Statement expected.
    8.	Incorrect symbol after statement part in block.
    9.	Period expected.
    10.	Semicolon between statements missing.
    11.	Undeclared identifier.
    12.	Assignment to constant or procedure is not allowed.
    13.	Assignment operator expected.
    14.	call must be followed by an identifier.
    15.	Call of a constant or variable is meaningless.
    16.	then expected.
    17.	Semicolon or } expected.
    18.	do expected.
    19.	Incorrect symbol following statement.
    20.	Relational operator expected.
    21.	Expression most not contain a procedure identifier.
    22.	Right parenthesis missing.
    23.	The preceding factor cannot begin with this symbol.
    24.	An expression cannot begin with this symbol.
    25.	This number is too large.
*/
}

void getToken() {
    char filename[250];
    if(file == NULL) {
        if(DEBUGGER == 1) {
            file = fopen ( "lexi.txt" , "r" );
        } else {
            printf("Enter a file name\n");
            scanf("%s",filename);
            file = fopen ( filename , "r" );
        }
    } 

    if(file == NULL) { 
        printf("File not found\n");
        system("PAUSE");
        exit(1);
    } else {
        if(!feof(file)) {
            fscanf(file, "%d", &marble.id);
            if(marble.id == 2 || marble.id == 3) {
                fscanf(file, "%d", &marble.value);
                printToken(marble);
            } else {
                marble.value = -1;
                printToken(marble);
            }
        }
    }
}

void printToken(struct token marble) {
    if(marble.id == 2 || marble.id == 3)
        printf("marble: %d  color: %d\n",marble.id,marble.value);
    else
        printf("marble: %d\n",marble.id);    
}

//<program> ::= block "." . 
int program() {
    return 0;
}

//<block> ::= <const-declaration> <var-declaration> <statement>	
int block() {
    return 0;
}

//<constdeclaration> ::= [ "const" <ident> "=" <number> {"," <ident> "=" <number>} "semicolomsym"]	
int constDeclaration() {
    return 0;
}

//<var-declaration> ::= [ "int" <ident> {"," <ident>} "semicolomsym"]
int varDeclaration() {
    return 0;
}

/*<statement > ::= [<ident> ":=" <expression>
	      	| "{" <statement> {"semicolomsym" <statement> } "}" 
	      	| "if" <condition> "then" <statement> 
	      	| e ] 
	      	*/
int statement() {
    return 0;
}
  
/*<condition> ::= "odd" <expression> 
	  	| <expression> <rel-op> <expression>
	  	*/
int condition() {
    return 0;
}
  
//<rel-op> ::= "="|"!="|"< >"|"<"|"<="|">"|">="
int relOP() {
    return 0;
}

//<expression> ::= [ "+"|"-"] <term> { ("+"|"-") <term>}
int expression() {
    return 0;
}

//<term> ::= <factor> {("*"|"/") <factor>} 
int term() {
    return 0;
}

//<factor> ::= <ident> | <number> | "(" <expression> ")"
int factor() {
    return 0;
}

//<number> ::= <digit> {<digit>}
int number() {
    return 0;
}

//<ident> ::= <letter> {<letter> | <digit>}
int ident() {
    return 0;
}

//<digit> ;;= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
int digit() {
    return 0;
}

//<letter> ::= "a" | "b" | … | "y" | "z" | "A" | "B" | ... | "Y" | "Z"
int letter() {
    return 0;
}

