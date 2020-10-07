%{

#define TITLE 100
#define SPORT 101
#define YEARS 102
#define NAME 103
#define YEAR_NUM 104
#define COMMA 105
#define THROUGH 106
#define SINCE 107
#define ALL 108

union {
  int ival;
  char name [30];
  char str [80];
} yylval;

#include <string.h> 

extern int atoi (const char *);
%}

%option noyywrap
%option yylineno

/* exclusive start condition -- deals with C++ style comments */ 
%x COMMENT

%%
"Olympic Games" {strcpy (yylval.name, yytext); return TITLE;}
[0-9]+   {yylval.ival = atoi (yytext); if(yylval.ival == 2020) {yylval.ival = 2021;} if(yylval.ival>1895) return YEAR_NUM;}
("-"|"through")  {return THROUGH;}
"<sport>"        {return SPORT;}
"<years>"        {return YEARS;}
"since"          {return SINCE;}
"all"            {return ALL;}
","              {return COMMA;}

\"([^\\"\n]|\\.)*\" { strcpy (yylval.str, yytext); return NAME; }

[\n\t ]+  /* skip white space */

"//"       { BEGIN (COMMENT); }

<COMMENT>.+ /* skip comment */
<COMMENT>\n {  /* end of comment --> resume normal processing */
                BEGIN (0); }

.          { fprintf (stderr, "unrecognized token %c in line %d\n", yytext[0],yylineno); }

%%

int main (int argc, char **argv)
{
   int token;

   if (argc != 2) {
      fprintf(stderr, "Usage: mylex <input file name>\n", argv [0]);
      exit (1);
   }          

   yyin = fopen (argv[1], "r");
   printf("TOKEN          LEXEME               SEMANTIC VALUE\n");
   printf("--------------------------------------------------\n");
   while ((token = yylex ()) != 0) 
                                                  
     switch (token) {
       case TITLE:          
              printf("TITLE          %-20s %-20s\n", yytext,yylval.name);
	       break;
       case SPORT: 
              printf("SPORT          %s\n", yytext);
	       break;
       case YEARS: 
              printf("YEARS          %s\n", yytext);
	       break;
       case NAME: 
              printf("NAME           %-20s %-20s\n",yytext,yylval.str);            
	       break;
       case YEAR_NUM:     
              printf("YEAR_NUM       %-20s %-20d\n", yytext,yylval.ival);
	       break;
       case COMMA: 
              printf("COMMA          %s\n", yytext);
	       break;
       case THROUGH: 
              printf("THROUGH        %s\n", yytext);
	       break;
       case SINCE:     
              printf("SINCE          %s\n", yytext);
	       break;
       case ALL: 
              printf("ALL            %s\n", yytext);
	       break;
        default:     
              fprintf (stderr, "error ... \n"); exit (1);
     } 
   fclose (yyin);
   exit (0);
}

