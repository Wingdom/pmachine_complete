University of Central Florida
School of Electrical Engineering & Computer Science
COP 3402: System Software
Fall 2009

Homework #2 (Lexical Analyzer)
(Team project  Maximum of two people per team.)

Due Sunday Oct. 4th, 2009 by 11:59 p.m. 

Goal:
In this assignment you have to implement a lexical analyzer for the language nested C/0. Your program must be capable to read in a source program written in nested C/0, identify some errors, and produce, as output, the source program, the source program lexeme table, and a list of lexemes. 

For an example of input and output refer to Appendix A.

Lexical Conventions for PL/0:
A numerical value is assigned to each token as follows: 
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,  slashsym = 7, oddsym = 8,  eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12, 
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28, intsym = 29, procsym = 30, writesym = 31, readsym = 32.

Reserved Words: {, }, call, const, do, if, odd, procedure, then, int, while .
Special Symbols: ‘+’, ‘-‘, ‘*’, ‘/’, ‘(‘, ‘)’, ‘=’, ’,’ , ‘.’, ‘ <’, ‘>’,  ‘;’ , ’:’ .
Identifiers: identsym = letter (letter | digit)* 
Numbers: numbersym = (digit)+
Invisible Characters: tab, white spaces, newline
Comments denoted by: /* . . .   */

Refer to Appendix B for a declaration of the token symbols that may be useful.

Constraints:
Input:
1.	Identifiers can be a maximum of 11 characters in length.
2.	Numbers can be a maximum of 5 digits in length.
3.	Comments should be ignored and not tokenized.
4.	Invisible Characters should be ignored and not tokenized.

Output:
1.	The token separator in the output's Lexeme List (Refer to Appendix A) can be either a space or a bar ('|').
2.	In your output's Lexeme List, identifiers must show the token and the variable name separated by a space or bar.
3.	In your output's Lexeme List, numbers must show the token and the value separated by a space or bar. The value must be transformed into ASCII Representation (as discussed in class)
4.	Be consistent in output. Choose either bars or spaces and stick with them.
5.	The token representation of the Lexeme List will be used in the Parser (Project 3). So, PLAN FOR IT!

Detect the Following Lexical Errors:

Variable does not start with letter.
Number too long.
Name too long.
Invalid symbols.

Create a transition diagram (DFS) to recognize each lexeme on the source program and once accepted generate the token, otherwise emit an error message.
 
Submission Instructions:
Submit to Webcourse:
1.	 Source code.
2.	 Instructions to use the program in a readme document.
3.	 One run containing the input file (Source Program), and output in a file (Source, Lexeme Table(lexeme-token), Lexeme List)
Appendix A:

If the input is:
int x, y;
{
	y := 3;
	x := y + 56;
}.

The output will be:
Source Program:
int x, y;
{
	y := 3;
	x := y + 56;
}.

Lexeme Table:
lexeme		token type 	
int		29		
x		2
,		17		
y		2
;		18
{		21
y		2
:=		20
3		3
;		18
x		2
:= 	 	20		
y		2
+		4
56		3
;		18
} 		22
.		19

Lexeme List:
29  2 x  17  2 y  18  21 2 y 20 3 3 18  2 x  20  2  y  4  3  56  18  22  19

Appendix B:

Declaration of Token Types:
typedef enum { 
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
whilesym, dosym, callsym, constsym, intsym, procsym, writesym
} token_type;


Example of Token Representation:
“29  2  1  17  2  2  18  21  2  1  20  2	2  4  3  56  18  22  19”

Is Equivalent:
intsym	 identsym  x  commasym  identsym  y  semicolonsym  beginsym  identsym  x
becomessym  identsym  y  plussym  numbersym  56  semicolonsym  endsym  periodsym
Appendix C:

Example of a nested C/0 program: 
const m = 7, n = 85;  
int  i,x,y,z,q,r;  
procedure mult; 
   int a, b;  
  { 
  a := x;  b := y; z := 0;   
  while b > 0 do    
    {       
       if odd x then z := z+a;       
       a := 2*a; 
       b := b/2;     
    }   
};

{
  x := m;
  y := n;
  call mult;
}.

