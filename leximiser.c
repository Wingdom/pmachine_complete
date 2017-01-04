/*                           Lexigraphical reader                             */
/*                               Programmed by                                */
/*                     Brandon Couts and Cameron Yeager                       */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUTFILE  "code.txt"
#define OUTPUTFILE "leximiser - compiled.txt"

#define  IMAX               32767       /* maximum integer value */
#define  IMAX_SIZE          5
#define  STR_MAX            11         /* maximum length of strings */
#define  ALPHABET_SIZE      18+10+26+26
#define  SPECIALSYM         15
#define  RESERVED           32
#define  RESERVED_LENGTH    10

struct token {
    char string[STR_MAX];
    int size;
    int id;
    int varible_id;
};
struct varible {
    char string[STR_MAX];
};

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

char alphabet[ALPHABET_SIZE] = {'A','B','C','D','E','F','G','H','I','J','K','L',
                                'M','N','O','P','Q','R','S','T','U','V','W','X',
                                'Y','Z','a','b','c','d','e','f','g','h','i','j',
                                'k','l','m','n','o','p','q','r','s','t','u','v',
                                'w','x','y','z','0','1','2','3','4','5','6','7',
                                '8','9','+','-','*','/','(',')','=',',','.','<',
                                '>',';',':','{','}',' ','\t','\n'};
char special[SPECIALSYM] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';' , ':' , '{' , '}'};
char reservedWords[RESERVED][RESERVED_LENGTH] = {"NULL", "", "", "+", "-", "*", 
                                                "/", "odd", "=", "!=", "<", "<=", 
                                                ">", ">=", "(", ")", ",", ";", ".", 
                                                ":=", "{", "}", "if", "then", "while", 
                                                "do", "call", "const", "int", "procedure", 
                                                "write", "read"};

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

int main (int argc, char *argv[]) {
    char * input = INPUTFILE;
    char * filedata = NULL;
    int result;    

    struct token * lexemes;

    if(argc == 2)
        input = argv[1];

    printf("Reading in input file: %s\n",input);

    result = loadCode(input, &filedata);
    
    printf("\nFileload: %d characters\n%s\n", result, filedata);
    
    if(!error)
        lexemes = leximiser(filedata, result);
        
    if(!error)    
        lexemes = tokeniser(lexemes); 

    if(!error)
        printTokens(lexemes);
        
    if(!error)
        output(lexemes);

    system("PAUSE");
    
    return 0;
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

    if (*result == NULL) {fputs ("ERROR -- Memory unable to allocate",stderr); exit (2);}
    
    // copy the file into the buffer:
    size = fread (*result,1,size,file);

    (*result)[size] = '\0';

    /* the whole file is now loaded in the memory buffer. */

    fclose (file);

	return size;
}

struct token * leximiser(char *file, int size) {
    struct token * tokens = NULL;
    
    int reading_in = 0;

    int counter = 0;

    while(counter < size) {
        if(isAlphabet(file[counter]))
            if( file[counter] == '//' && file[counter + 1] == '*') {
                while(counter < size) {
                    if( file[counter] == '*' && file[counter + 1] == '//') 
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
            } else if(file[counter] == ' ' || file[counter] == '\t' || file[counter] == '\n') {
                if(reading_in == 1) {
                    reading_in = 0;
                }
            } else {
                if(reading_in == 0) {
                    token_num++;
                    tokens = (struct token *)realloc(tokens,sizeof(struct token) * token_num);
                    tokens[token_num - 1].id = 0;
                    strcpy(tokens[token_num - 1].string,"");
                    tokens[token_num - 1].size = 0;
                    reading_in = 1;
                }
                if(tokens[token_num - 1].size < STR_MAX) {
                    tokens[token_num - 1].string[tokens[token_num - 1].size] = file[counter];
                    tokens[token_num - 1].size++;
                    tokens[token_num - 1].string[tokens[token_num - 1].size] = '\0';
                } else {
                    error = 1;
                    printf("ERROR -- String too long.\n");
                }
            }
        else {
            error = 1;
            printf("ERROR -- Not a valid character position: %c ,%d\n",file[counter],counter);
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
                    if(check <= IMAX) {
                        tokens[counter].id = 3;
                    } else {
                        printf("ERROR -- Integer too big: %d > %d\n",check,IMAX);
                        error = 1;                    
                    }
                } else {
                    printf("ERROR -- %s is not a number.\n\n",tokens[counter].string);
                    error = 1;
                }
            } else {
                printf("ERROR -- Integer size too big: %s > %d\n",tokens[counter].string, IMAX);
                error = 1;
            }
        }
        //printf("[%d:%s]\n",tokens[counter].id,tokens[counter].string);
        counter++;
    }
    return tokens;
}

int isReserved(struct token *tokens) {
    int counter = 0;
    while(counter < RESERVED) {
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
    while (counter < SPECIALSYM) {
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
    for(counter = 0; counter < ALPHABET_SIZE; counter++)
        if(dude == alphabet[counter])
            return 1;
    return 0;
}

void isVarible( struct token * tokens ) {
    int counter = 0;
    int isvarible = 0;
    //char * varibles[STR_MAX];

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
        printf("[%d|%s]\n",tokens[counter].id,tokens[counter].string);
        counter++;
    }
    printf("\n");
}

void output(struct token * tokens) {
    int counter = 0;
    printf("Lexeme List, in varible identifition:\n");
    while ( counter < token_num ) {
        printf("%d ",tokens[counter].id);
        if(tokens[counter].id == 2)
            printf("%s ",tokens[counter].string);
        if(tokens[counter].id == 3)
            printf("%s ",tokens[counter].string);
        counter++;
    }
    printf("\n\n");
    counter = 0;
    printf("Lexeme List, in incremental varible identifition:\n");
    while ( counter < token_num ) {
        printf("%d ",tokens[counter].id);
        if(tokens[counter].id == 2)
            printf("%d ",tokens[counter].varible_id);
        if(tokens[counter].id == 3)
            printf("%s ",tokens[counter].string);
        counter++;
    }
    printf("\n");
    FILE *file;
    file = fopen(OUTPUTFILE,"w");
    
    counter = 0;
    while (counter < token_num ) {
        fprintf(file,"%d ",tokens[counter].id);
        if(tokens[counter].id == 2)
            fprintf(file,"%d ",tokens[counter].varible_id);
        if(tokens[counter].id == 3)
            fprintf(file,"%s ",tokens[counter].string);
        counter++;
    }
    
    printf("\nLexeme List, in incremental varible identifition, stored in %s\n",OUTPUTFILE);
    
    fclose(file);
}
