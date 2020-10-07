make:
	win_flex.exe tennis.lex
	win_bison.exe -d tennis.y
	gcc -o tennis.exe lex.yy.c tennis.tab.c
