%code {
#include <stdio.h>
#include <string.h>

  /* yylex () and yyerror() need to be declared here */
extern int yylex (void);
void yyerror (char *s);
}

%code requires {
    struct Player {
         int wins;
         char name[30];
         char gender[5];
    };
}


/* note: no semicolon after the union */
%union{
   int wins;
   char name[30];
   char gender[5];
   struct Player player;
};

%token <wins> NUM
%token <gender> PLAYER_GENDER
%token <name> PLAYER_NAME
%token TITLE NAME GENDER WIMBLEDON AUSTRALIAN_OPEN 

%type <wins> year_spec list_of_years optional_wimbledon 
%type <player> list_of_players player

%%
input: TITLE list_of_players {
   if($2.wins == -2)
      printf("List is empty!\n");
   else if($2.wins == -1)
      printf("No Man players in the list.\n");
   else
      printf("Player with most wins at Wimbledon: %s (%d wins)",$2.name ,$2.wins);
   };

list_of_players: list_of_players player {
   if($1.wins > $2.wins)
      $$ = $1;
   else
      $$ = $2;
   };

list_of_players: /* empty */ {$$.wins == -2;} ;

player: NAME PLAYER_NAME GENDER PLAYER_GENDER optional_wimbledon optional_australian_open 
   {
      if(strcmp($4,"Man") == 0){
         strcpy($$.name,$2);
         $$.wins = $5;
      }else
         $$.wins = -1;
   };

optional_wimbledon: WIMBLEDON list_of_years {$$ = $2;} | /* empty */ { $$ = 0;} ;

optional_australian_open: AUSTRALIAN_OPEN list_of_years | /* empty */ ;

list_of_years: list_of_years ',' year_spec {$$ = $1 + $3;};

list_of_years: year_spec {$$ = $1;};

year_spec: NUM {$$ = 1;} | NUM '-' NUM {$$ = $3 - $1 + 1;};

%%

int main (int argc, char **argv)
{
  extern FILE *yyin;
  if (argc != 2) {
     fprintf (stderr, "Usage: %s <input-file-name>\n", argv[0]);
	 return 1;
  }
  yyin = fopen (argv [1], "r");
  if (yyin == NULL) {
       fprintf (stderr, "failed to open %s\n", argv[1]);
	   return 2;
  }
  
  yyparse ();
  
  fclose (yyin);
  return 0;
}

void yyerror (char *s)
{
  fprintf (stderr, "%s\n", s);
}





