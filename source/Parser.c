/*                                  Parser                                    */
/*                               Programmed by                                */
/*                     Brandon Couts and Cameron Yeagle                       */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define DEBUGGER 0
#define INPUTFILE "leximiser - compiled.txt"
#define OUTPUTFILE "parser - completed.txt"

#define STATE(x) if(DEBUGGER == 1) printf( x );


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

struct symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
/*For constants, you must store kind, name and value.
  For variables, you must store kind, name, L and M.
  For procedures, you must store kind, name, L and M.*/

FILE * file;
struct token marble;

int rel_op();
int ident();
int number();
int factor();
int term();
int expression();
int condition();
int statement();
int var_declaration();
int constdeclaration();
int block();
int program();
void error(int error_code);
void printToken(struct token marble);
void getToken();

int main ()
{
    program();
    
    system("PAUSE");
    return 0;
}
void getToken() {
    char filename[250];
    if(file == NULL) {
        if(DEBUGGER == 1) {
            file = fopen ( "leximiser - compiled.txt" , "r" );
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

void error(int error_code)
{
    //1.	Use = instead of :=.
    if(error_code == 1)
    {
        printf("Use = instead of :=");
    }
    //2.	= must be followed by a number.
    if(error_code == 2)
    {
        printf("= must be followed by a number");
        
    }
    //3.	Identifier must be followed by =.
    if(error_code == 3)
    {
        printf("Identifier must be followed by =");
    }
    //4.	const, var, procedure must be followed by identifier.
    if(error_code == 4)
    {
        printf("Const, var, procedure must be followed by identifier");
    }
    //5.	Semicolon or comma missing.
    if(error_code == 5)
    {
        printf("Semicolin or comma missing");
    }
    //6.	Incorrect symbol after procedure declaration.
    if(error_code == 6)
    {
        printf("Incorrect symbol after procedure declaration");
    }
    //7.	Statement expected.
    if(error_code == 7)
    {
        printf("Statement expected");
    }
    //8.	Incorrect symbol after statement part in block.
    if(error_code == 8)
    {
        printf("Incorrect symbol after statement part in block");
    }
    //9.	Period expected.
    if(error_code == 9)
    {
        printf("Period expeected");
    }
    //10.	Semicolon between statements missing.
    if(error_code == 10)
    {
        printf("Semicolon between statements missing");
    }
    //11.	Undeclared identifier.
    if(error_code == 11)
    {
        printf("Undeclared identifier");
    }
    //12.	Assignment to constant or procedure is not allowed.
    if(error_code == 12)
    {
        printf("Assignment to constant or procedure is not allowed");
    }
    //13.	Assignment operator expected.
    if(error_code == 13)
    {
        printf("Assignment operator expected");
    }
    //14.	call must be followed by an identifier.
    if(error_code == 14)
    {
        printf("Call must be followed by an identifier");
    }
    //15.	Call of a constant or variable is meaningless.
    if(error_code == 15)
    {
        printf("Call of a constant or variable is meaningless");
    }
    //16.	then expected.
    if(error_code == 16)
    {
        printf("Then expected");
    }
    //17.	Semicolon or } expected.
    if(error_code == 17)
    {
        printf("Semicolon or } expected");
    }
    //18.	do expected.
    if(error_code == 18)
    {
        printf("Do expected");
    }
    //19.	Incorrect symbol following statement.
    if(error_code == 19)
    {
        printf("Incorrect symbol following statement");
    }
    //20.	Relational operator expected.
    if(error_code == 20)
    {
        printf("Relation operator expected");
    }
    //21.	Expression most not contain a procedure identifier.
    if(error_code == 21)
    {
        printf("Expression must not contain a procedure identifier");
    }
    //22.	Right parenthesis missing.
    if(error_code == 22)
    {
        printf("Right parenthesis missing");
    }
    //23.	The preceding factor cannot begin with this symbol.
    if(error_code == 23)
    {
        printf("The preceding factor cannot begin with this symbol");
    }
    //24.	An expression cannot begin with this symbol.
    if(error_code == 24)
    {
        printf("An expression cannot begin with this symbol");
    }
    //25.	This number is too large.
    if(error_code == 25)
    {
        printf("This number is too large");
    }
    system("PAUSE");
    exit(1);
}

//<program> ::= block "." . 
int program()
{
    getToken();
    block();
    if(marble.id != periodsym)
        error(9);
    return 0;
}

//<block> ::= <const-declaration> <var-declaration> <statement>	
int block()
{
    constdeclaration();
    var_declaration();
    statement(); //STATEMENT
    
    return 0;
}//end

//<constdeclaration> ::= [ "const" <ident> "=" <number> {"," <ident> "=" <number>} "semicolomsym"]	
int constdeclaration()
{
    if(marble.id == constsym){
        STATE("DEFINING CONST\n");
        do{ //repeat
            getToken(); //GET(TOKEN)
            if(marble.id != identsym) //if TOKEN != "identsym" then ERROR;
                error(4);
            getToken(); //GET(TOKEN)
            if(marble.id != eqsym) //if TOKEN != "eqsym" then ERROR;
                error(3);
            getToken(); //GET(TOKEN)
            if(marble.id != numbersym) //if TOKEN != NUMBER then ERROR;
                error(2);
            getToken();
        }while(marble.id == commasym); //until TOKEN != "commasym";
        
        if(marble.id != semicolonsym) //if TOKEN != "semicolonsym" then ERROR;
            error(5);
        getToken(); //GET(TOKEN)
    } //end
    return 0;
}

//<var-declaration> ::= [ "int" <ident> {"," <ident>} "semicolomsym"]
int var_declaration()
{
    if(marble.id == intsym){ //if TOKEN = "int" then begin
    STATE("DEFINING INTS\n");
        do{ //repeat
            getToken(); //GET(TOKEN);
            if(marble.id != identsym) //if TOKEN != "identsym" then ERROR;
                error(4);
            getToken(); //GET(TOKEN);
        }while(marble.id == commasym); //until TOKEN != "commasym";
        if(marble.id != semicolonsym) //if TOKEN != "semicolonsym" then ERROR;
            error(17);
        getToken(); //GET(TOKEN)
    }//end
    while(marble.id == procsym){ //while TOKEN = "procsym" do begin
        getToken(); //GET(TOKEN);
        if(marble.id != identsym) //if TOKEN != "identsym" then ERROR;
            error(4);
        getToken(); //GET(TOKEN);
        if(marble.id != semicolonsym)//if TOKEN != "semicolomsym" then ERROR;
            error(5);
        getToken(); //GET(TOKEN);
        block(); //BLOCK;
        if(marble.id != semicolonsym) //if TOKEN != "semicolomsym" then ERROR;
            error(5);
        getToken(); //GET(TOKEN)
    }//end
    return 0;
}

/*<statement > ::= [<ident> ":=" <expression>
	      	| "{" <statement> {"semicolomsym" <statement> } "}" 
	      	| "if" <condition> "then" <statement> 
	      	| e ] 
	      	*/
int statement()
{//begin
    if(marble.id == identsym){ //if TOKEN = "identsym" then begin
        getToken(); //GET(TOKEN);
        if(marble.id != becomessym) //if TOKEN != "becomessym" then ERROR;
            error(19);
        getToken(); //GET(TOKEN);
        expression(); //EXPRESSION
    } //end
    else if(marble.id == callsym){ //else if TOKEN = "callsym" then begin
        getToken(); //GET(TOKEN);
        if(marble.id != identsym) //if TOKEN != "identsym" then ERROR;
            error(14);
        getToken(); //GET(TOKEN)
    }//end
    else if(marble.id == beginsym){ //else if TOKEN = "beginsym" then begin	("beginsym"  = "{" )
        getToken(); //GET TOKEN;
        statement(); //STATEMENT;
        while(marble.id == semicolonsym){ //while TOKEN = "semicolomsym" do begin
            getToken(); //GET(TOKEN);
            statement(); //STATEMENT
        }//end;
        if(marble.id != endsym) //if TOKEN != "endsym" then ERROR; 	("endsym"  = "}" )
            error(17);
        getToken(); //GET(TOKEN)
    } //end
    else if(marble.id == ifsym){//else if TOKEN = "ifsym" then begin
        getToken(); //GET(TOKEN);
        condition(); //CONDITION;
        if(marble.id != thensym) //if TOKEN != "thensym" then ERROR;
            error(16);
        getToken(); //GET(TOKEN);
        statement();//STATEMENT
    } //end
/* While is not implemented in this grammar    
    else if(marble.ic == whilesym){ //else if TOKEN = "whilesym" then begin
        getToken(); //GET(TOKEN);
        condition(); //CONDITION;
        if(marble.id != dosym) //if TOKEN != "dosym" then ERROR;
            error(18);
        getToken(); //GET(TOKEN);
        statement(); //STATEMENT 
    } //end */
    return 0;
} //end;
  
/*<condition> ::= "odd" <expression> 
	  	| <expression> <rel_op> <expression>
	  	*/
int condition()
{//begin
    if(marble.id == oddsym){ //if TOKEN = "oddsym" then begin
        getToken(); //GET(TOKEN);
        expression(); //EXPRESSION
    }
    else{ //else begin
        expression(); //EXPRESSION;
        if(!rel_op()) //if TOKEN != RELATION then ERROR;
            error(20);
        getToken(); //GET(TOKEN);
        expression(); //EXPRESSION
    } //end
    return 0;
}//end;

//<expression> ::= [ "+"|"-"] <term> { ("+"|"-") <term>}
int expression()
{ //begin
    if(marble.id == plussym || marble.id == minussym) //if TOKEN = "plussym"or "minussym" then GET(TOKEN);
        getToken();
    term(); //TERM;
    while(marble.id == plussym || marble.id == slashsym){ //while TOKEN = "plussym" or "slashsym" do begin
        getToken(); //GET(TOKEN);
        term(); //TERM
    }//end
    return 0;
}//end;

//<term> ::= <factor> {("*"|"/") <factor>} 
int term()
{//begin
    factor();//FACTOR;
    while(marble.id == multsym || marble.id == slashsym){//while TOKEN = "multsym" or "slashsym" do begin
        getToken(); //GET(TOKEN);
        factor(); //FACTOR
    } //end
    return 0;
} //end;

//<factor> ::= <ident> | <number> | "(" <expression> ")"
int factor()
{//begin
    if(marble.id == identsym) //if TOKEN = "identsym then
        getToken(); //GET(TOKEN)
    else if(marble.id == number()) //else if TOKEN = NUMBER then
        getToken(); //GET(TOKEN)
    else if(marble.id == lparentsym){//else if TOKEN = "(" then begin
        getToken(); //GET(TOKEN);
        expression(); //EXPRESSION;
        if(marble.id != rparentsym) //if TOKEN != ")" then ERROR;
            error(22);
        getToken(); //GET(TOKEN)
    }//end
    else
        error(4);
    return 0;
} //end;

//<number> ::= <digit> {<digit>}
int number()
{
    if(marble.id == numbersym)
        return 3;
    return -1;
}

//<ident> ::= <letter> {<letter> | <digit>}
int ident()
{
    if(marble.id == identsym)
        return 2;
    return -1;
}

//<rel-op> ::= "="|"!="|"< >"|"<"|"<="|">"|">="
int rel_op()
{
    if(marble.id == becomessym || marble.id == eqsym || 
       marble.id == neqsym ||
       marble.id == neqsym /*< > needs to be tokenised as neqsym too*/ || 
       marble.id == lessym || marble.id == leqsym ||
       marble.id == gtrsym || marble.id == geqsym)
        return 1;
}

