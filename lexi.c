#include <stdio.h>
#include <string.h>

#define INPUTFILE "code.txt"

#define  norw      15         /* number of reserved words */
#define  imax   32767       /* maximum integer value */
#define  cmax      11         /* maximum number of chars for idents */
#define  nestmax    5         /* maximum depth of block nesting */                
#define  STR_MAX   256         /* maximum length of strings */


typedef enum { 
    nulsym = 1,         // NULL
    identsym = 2,       // Used before a varible
    numbersym = 3,      // Used before a number
    plussym = 4,        // +
    minussym = 5,       // -
    multsym = 6,        // *
    slashsym = 7,       // /
    oddsym = 8,         //
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

int loadCode(char *filename, char **result);
char ** leximizer(char * filedata, int size);

int main(int argc, char *argv[]) {
    char * input = INPUTFILE;
    char * filedata = NULL;
    int result;    

    char ** lexemes;

    if(argc == 2)
        input = argv[1];

    printf("Reading in input file: %s\n",input);

    result = loadCode(input, &filedata);
    
//    printf("\nFileload: %d\n%s", result, filedata);
    
    lexemes = leximizer(filedata, result);

    token = tokenizer(lexemes, ); 
    
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

    if (*result == NULL) {fputs ("Memory error",stderr); exit (2);}
    
    // copy the file into the buffer:
    size = fread (*result,1,size,file);

    (*result)[size] = '\0';

    //printf("%s\n",*result);
    /* the whole file is now loaded in the memory buffer. */

    fclose (file);

	return size;
}

char ** leximizer(char * filedata, int size) {
    char ** lexemes = NULL;
    int counter = 0;

    char temp[STR_MAX] = "";

    while(counter < size) {
        if(filedata[counter] != ' ' && filedata[counter] != '\t' && filedata[counter] != '\n')
            //printf("%c",filedata[counter]);//strcat(temp,filedata[counter]);
        printf("%c",filedata[counter]);
        counter++;
    }
    //printf(filedata);
}
