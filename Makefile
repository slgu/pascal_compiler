LEX=flex
YACC=yacc
CC=g++ -std=c++11
a.out:common.o labelmanager.o lex.yy.o y.tab.o expr.o stmt.o enviroment.o var_record.o routine.o
	$(CC) labelmanager.o routine.o stmt.o common.o expr.o  var_record.o enviroment.o lex.yy.o y.tab.o -o a.out
labelmanager.o:labelmanager.cpp
	$(CC) -c labelmanager.cpp
common.o: common.cpp
	$(CC) -c common.cpp
lex.yy.o:lex.yy.c y.tab.h
	$(CC) -c lex.yy.c
routine.o: routine.cpp routine.h
	$(CC) -c routine.cpp
expr.o: expr.cpp expr.h
	$(CC) -c expr.cpp
stmt.o: expr.cpp stmt.h
	$(CC) -c stmt.cpp
enviroment.o: enviroment.cpp enviroment.h
	$(CC) -c enviroment.cpp
var_record.o: var_record.cpp var_record.h
	$(CC) -c var_record.cpp
y.tab.o:y.tab.c
	$(CC) -c y.tab.c
y.tab.c y.tab.h:parse.y
	$(YACC) -d parse.y
lex.yy.c:token.l
	$(LEX) token.l
clean:
	rm -f *.o lex.yy.c y.tab.h y.tab.c a.out *.gch
	