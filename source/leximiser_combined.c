/*                           A Lexigraphical reader                           */
/*                               Programmed by                                */
/*                     Brandon Couts and Cameron Yeagle                       */
/* -------------------------------------------------------------------------- */
//todo
// junk it into their structure

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEXER_INPUTFILE  "input2.txt"
#define LEXER_OUTPUTFILE "leximiser - compiled.txt"

#define LEXER_DEBUG 1
#define LEXER_ERROR 1

#define  LEXER_IMAX               99999       /* maximum integer value */
#define  LEXER_IMAX_SIZE          5
#define  LEXER_STR_MAX            256         /* maximum length of strings */
#define  LEXER_ALPHABET_SIZE      18+10+26+26
#define  LEXER_SPECIALSYM         15
#define  LEXER_RESERVED           35
#define  LEXER_RESERVED_LENGTH    10

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
    readsym = 32,       // read
    rBracketsym = 33,   // ]
    lBracketsym = 34,   // [
    elsesym = 35        // else
} tokenType;

struct token {
    char string[LEXER_STR_MAX];
    int size;
    int id;
    int varible_id;
};

struct varible {
    char string[LEXER_STR_MAX];
};

/* FRANKS SILLY STUFF */
struct lexemeTableEntry {
    char *lexeme;                  // lexeme
    int lexemeLen;                 // length of lexeme
    tokenType token;               // token type of the lexeme
    struct lexemeTableEntry *next; // link to the next lexeme in line
};

char alphabet[LEXER_ALPHABET_SIZE] = {'A','B','C','D','E','F','G','H','I','J','K','L',
                                'M','N','O','P','Q','R','S','T','U','V','W','X',
                                'Y','Z','a','b','c','d','e','f','g','h','i','j',
                                'k','l','m','n','o','p','q','r','s','t','u','v',
                                'w','x','y','z','0','1','2','3','4','5','6','7',
                                '8','9','+','-','*','/','(',')','=',',','.','<',
                                '>',';',':','{','}',' ','\t','\n'};
                                
char special[LEXER_SPECIALSYM] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';' , ':' , '{' , '}'};

char reservedWords[LEXER_RESERVED][LEXER_RESERVED_LENGTH] = {"NULL", "", "", "+", "-", "*", 
                                                "/", "odd", "=", "!=", "<", "<=", 
                                                ">", ">=", "(", ")", ",", ";", ".", 
                                                ":=", "{", "}", "if", "then", "while", 
                                                "do", "call", "const", "int", "procedure", 
                                                "write", "read", "]", "[","else"}; // added barkets for machine

struct varible * varibles;
int varible_size = 0;                                                
int token_num = 0;
int error = 0;


int loadCode( char *filename, char **result );
struct token * leximiser( char *file, int size );
struct token * tokeniser(struct token *tokens);
int isReserved( struct token * tokens );
int isSpecialSymbol( char dude );
int isAlphabet( char dude );
int isNum( struct token *tokens );
int isLetter( char dude );
void isVarible( struct token * tokens );
void printTokens( struct token * tokens );
void output( struct token * tokens );
struct lexemeTableEntry * lexer();
struct lexemeTableEntry * converter(struct token * lexemes);
struct lexemeTableEntry * insertBack(struct lexemeTableEntry *front, struct token lexeme);
void lexe_printTokens(struct lexemeTableEntry *table);

int main(){
    struct lexemeTableEntry * table = lexer();
    lexe_printTokens(table);
    system("PAUSE");
    return 0;
}

struct lexemeTableEntry * lexer () {
    char * input = LEXER_INPUTFILE;
    char * filedata = NULL;
    int result;    

    struct token * lexemes;

    if(LEXER_DEBUG == 1) printf("Reading in input file: %s\n",input);

    result = loadCode(input, &filedata);
    
    if(LEXER_DEBUG == 1) printf("\nFileload: %d characters\n%s\n", result, filedata);
    
    if(!error)
        lexemes = leximiser(filedata, result);
    
    if(!error)    
        lexemes = tokeniser(lexemes); 

    if(!error)
        printTokens(lexemes);
        
    if(!error)
        output(lexemes);

    if(LEXER_DEBUG == 1) system("PAUSE");
    
    struct lexemeTableEntry * tokens_to_the_max = converter(lexemes);
    
    return tokens_to_the_max;
}

int loadCode(char *filename, char **result) {
    FILE * file;
    int size;
    *result = NULL;

    file = fopen ( filename , "r" );
    if (file==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (file , 0 , SEEK_END);
    size = ftell (file);
    rewind (file);

    // allocate memory to contain the whole file:
    *result = (char *) malloc (size + 1);

    if (*result == NULL) {if(LEXER_ERROR == 1) printf ("LEXER_ERROR -- Memory unable to allocate",stderr); exit (2);}
    
    // copy the file into the buffer:
    size = fread (*result,1,size,file);

    (*result)[size] = '\0';

    /* the whole file is now loaded in the memory buffer. */

    printf("%s",*result);

    fclose (file);

	return size;
}

struct token * leximiser(char *file, int size) {
    struct token * tokens = NULL;
    
    int reading_in = 0;

    int counter = 0;

    while(counter < size) {
        if( file[counter] == '/' && file[counter + 1] == '*') {
                while(1) {
                    if( file[counter - 1] == '*' && file[counter] == '/') 
                        break;
                    counter++;
                }
        } else if( isSpecialSymbol( file[counter] ) ) {
            token_num++;
            tokens = (struct token *)realloc(tokens,sizeof(struct token) * token_num);
            tokens[token_num - 1].string[0] = file[counter];
            tokens[token_num - 1].id = 0;
            if(file[counter + 1] == '=') {
                tokens[token_num - 1].string[1] = '=';
                tokens[token_num - 1].string[2] = '\0';
                counter++;
            } else
                tokens[token_num - 1].string[1] = '\0';
            tokens[token_num - 1].size = 1;
            reading_in = 0;   
            if(LEXER_DEBUG == 1) printf("%c",file[counter]);
        } else if(file[counter] == ' ' || file[counter] == '\t' || file[counter] == '\n') {
            if(reading_in == 1) {
                reading_in = 0;
            }
            if(LEXER_DEBUG == 1) printf("%c",file[counter]);

        } else {
            if(reading_in == 0) {
                token_num++;
                tokens = (struct token *)realloc(tokens,sizeof(struct token) * token_num);
                tokens[token_num - 1].id = 0;
                strcpy(tokens[token_num - 1].string,"");
                tokens[token_num - 1].size = 0;
                reading_in = 1;
            }
            if(tokens[token_num - 1].size < LEXER_STR_MAX) {
                tokens[token_num - 1].string[tokens[token_num - 1].size] = file[counter];
                tokens[token_num - 1].size++;
                tokens[token_num - 1].string[tokens[token_num - 1].size] = '\0';
            } else {
                error = 1;
                if(LEXER_ERROR == 1) printf("LEXER_ERROR -- String too long.\n");
            }
            if(LEXER_DEBUG == 1) printf("%c",file[counter]);
        }
        counter++;
    }
    return tokens;
}

struct token * tokeniser(struct token *tokens) {
    int counter = 0;
    int check;
    int i;
    while(counter < token_num) {
        if(isReserved(&tokens[counter])) {
            tokens[counter].id = isReserved(&tokens[counter]);
        } else if (isLetter(tokens[counter].string[0])) {
            tokens[counter].id = 2;
            isVarible(&tokens[counter]);
        } else {
            if (tokens[counter].size <= 5){
                if(isNum(&tokens[counter])) {
                    check = atoi(tokens[counter].string);
                    if(check <= LEXER_IMAX) {
                        tokens[counter].id = 3;
                    } else {
                        if(LEXER_ERROR == 1) printf("LEXER_ERROR -- Integer too big: %d > %d\n",check,LEXER_IMAX);
                        error = 1;                    
                    }
                } else {
                    if(LEXER_ERROR == 1) printf("LEXER_ERROR -- %s is not a number.\n\n",tokens[counter].string);
                    error = 1;
                }
            } else {
                if(LEXER_ERROR == 1) printf("LEXER_ERROR -- Integer size too big: %s > %d\n",tokens[counter].string, LEXER_IMAX);
                error = 1;
            }
        }
        if(LEXER_DEBUG == 1) printf("[%d:%s]\n",tokens[counter].id,tokens[counter].string);
        counter++;
    }
    return tokens;
}

int isReserved(struct token *tokens) {
    int counter = 0;
    while(counter < LEXER_RESERVED) {
        if(strcmp(tokens->string, reservedWords[counter]) == 0) {
            tokens->id = counter + 1;
            return counter + 1;
        }
        counter++;
    }
    return 0;
}

int isSpecialSymbol( char dude ) {
    int counter = 0;
    while (counter < LEXER_SPECIALSYM) {
        if(special[counter] == dude)
            return 1;
        counter++;
    }
    return 0;
}

int isNum(struct token *tokens) {
    int counter = 0;
    int num;
    int isnum = 0;
    while( counter < tokens->size) {
        isnum = 0;
        for(num = 0; num < 10; num++)
            if(num == tokens->string[counter] - '0')
                isnum = 1;
        if(!isnum)
            return 0;
        counter++;
    }
    return 1;
}

int isLetter( char dude ) {
    int upper = 65;
    int lower = 97;
    
    for(upper = 65; upper <=90; upper++)
        if(dude == upper)
            return 1;
    
    for(lower = 97; lower <= 122; lower++)
        if(dude == lower)
            return 1;
    
    return 0;
}

int isAlphabet( char dude ) {
    int counter;
    for(counter = 0; counter < LEXER_ALPHABET_SIZE; counter++)
        if(dude == alphabet[counter])
            return 1;
    return 0;
}

void isVarible( struct token * tokens ) {
    int counter = 0;
    int isvarible = 0;
    //char * varibles[LEXER_STR_MAX];

    while(counter < varible_size) {
        if(strcmp(tokens->string,varibles[counter].string) == 0) {
            tokens->varible_id = counter + 1;
            isvarible = 1;
        }
        counter++;      
    }
    if(!isvarible) {
        varible_size++;
        varibles = (struct varible *)realloc(varibles,sizeof(struct varible)*varible_size);
        strcpy(varibles[varible_size - 1].string, tokens->string);
        tokens->varible_id = varible_size;
    }
}

void printTokens(struct token * tokens) {
    int counter = 0;
    while ( counter < token_num ){
        if(LEXER_DEBUG == 1) printf("[%d|%s]\n",tokens[counter].id,tokens[counter].string);
        counter++;
    }
    if(LEXER_DEBUG == 1) printf("\n");
}

void output(struct token * tokens) {
    int counter = 0;
    if(LEXER_DEBUG == 1) printf("Lexeme List, in varible identifition:\n");
    while ( counter < token_num ) {
        if(LEXER_DEBUG == 1) printf("%d ",tokens[counter].id);
        if(tokens[counter].id == 2) {
            if(LEXER_DEBUG == 1) printf("%s ",tokens[counter].string);
        }
        if(tokens[counter].id == 3) {
            if(LEXER_DEBUG == 1) printf("%s ",tokens[counter].string);
        }
        counter++;
    }
    if(LEXER_DEBUG == 1) printf("\n\n");
    counter = 0;
    if(LEXER_DEBUG == 1) printf("Lexeme List, in incremental varible identifition:\n");
    while ( counter < token_num ) {
        if(LEXER_DEBUG == 1) printf("%d ",tokens[counter].id);
        if(tokens[counter].id == 2) {
            if(LEXER_DEBUG == 1) printf("%d ",tokens[counter].varible_id);
        }
        if(tokens[counter].id == 3) {
            if(LEXER_DEBUG == 1) printf("%s ",tokens[counter].string);
        }
        counter++;
    }
    if(LEXER_DEBUG == 1) printf("\n");
    FILE *file;
    file = fopen(LEXER_OUTPUTFILE,"w");
    
    counter = 0;
    while (counter < token_num ) {
        fprintf(file,"%d ",tokens[counter].id);
        if(tokens[counter].id == 2)
            fprintf(file,"%d ",tokens[counter].varible_id);
        if(tokens[counter].id == 3)
            fprintf(file,"%s ",tokens[counter].string);
        counter++;
    }
    
    if(LEXER_DEBUG == 1) printf("\nLexeme List, in incremental varible identifition, stored in %s\n",LEXER_OUTPUTFILE);
    
    fclose(file);
}

struct lexemeTableEntry * converter(struct token * lexemes){
    int i = 0;
    struct lexemeTableEntry * there = NULL;

    for(i = 0; i < token_num; i++){
        there = insertBack(there,lexemes[i]);
    }   
    return there;
}

struct lexemeTableEntry * insertBack(struct lexemeTableEntry *front, struct token lexeme) {
    struct lexemeTableEntry *temp = NULL;
    struct lexemeTableEntry *begin = front;

    // Create the new node.
    temp = (struct lexemeTableEntry*)malloc(sizeof(struct lexemeTableEntry));

        // Copy everything thing in lexeme.string to temp ->     
        char * stringTemp = (char *)malloc(sizeof(char)*lexeme.size+1);
        char * j = &lexeme.string;
        temp -> lexeme = stringTemp;
        while(*stringTemp++ = *j++)
    
        temp -> lexemeLen = lexeme.size;
        temp -> token = lexeme.id;
        temp -> next = NULL;

    // Store front of the linked list
    begin = front;

    // Take care of case inserting into an empty list.
    if (begin == NULL)
        return temp;

    // Use front to iterate to the right spot to insert temp.
    while (begin->next != NULL)
        begin = begin->next;

    // Insert temp.
    begin->next = temp;

    // Return a pointer to the front of the list.
    return front;
}

void lexe_printTokens(struct lexemeTableEntry *table) {
     
    struct lexemeTableEntry *current = table;
    
    // Keep printing out token type for each lexeme until end of lexeme list
    while(current != NULL) {
        printf("ID%3d %2d %s \n",current->token,current->lexemeLen,current->lexeme);
        current = current -> next;
    }
    
    printf("\n");
}
