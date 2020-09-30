
CCC = g++
CCFLAGS= -O2 -std=gnu++17
LEX = flex
LFLAGS= -8     
YACC= bison 
YFLAGS= -d -t -y

RM = /bin/rm -f

my-c: y.tab.o lex.yy.o data.o my-c.o
	${CCC} ${CCFLAGS} lex.yy.o y.tab.o my-c.o data.o -o my-c

my-c.o: my-c.cpp my-c.h
	${CCC} -c -std=gnu++17 my-c.cpp
data.o: data.cpp data.h
	${CCC} -c -std=gnu++17 data.cpp
y.tab.o: my-c.yacc
	${YACC} ${YFLAGS} my-c.yacc
	${CCC} ${CCFLAGS} y.tab.c -c 

lex.yy.o: my-c.lex
	${LEX} $(LFLAGS) my-c.lex
	${CCC} ${CCFLAGS} lex.yy.c -c 

clean:
	${RM} lex.yy.* y.tab.* *.o my-c
