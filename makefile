OBJS = Literal.o CNFGenerator.o PLResolution.o Source.o
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

assign2 : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -pthread -o hw3
	
Source.o : Source.cpp
	$(CC) $(CFLAGS) Source.cpp

PLResolution.o : PLResolution.h PLResolution.cpp
	$(CC) $(CFLAGS) PLResolution.cpp
	
CNFGenerator.o : CNFGenerator.h CNFGenerator.cpp
	$(CC) $(CFLAGS) CNFGenerator.cpp

Literal.o : Literal.h Literal.cpp
	$(CC) $(CFLAGS) Literal.cpp

clean:
	\rm *.o hw3
