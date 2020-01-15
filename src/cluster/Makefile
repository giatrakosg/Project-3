OBJS = CMDParser.o main.o Parser.o Database.o Item.o Vector.o Curve.o ConfParser.o Clustering.o Hashtable.o
TESTOBJS = CMDParser.o  Parser.o Database.o Item.o Vector.o test.o Curve.o ConfParser.o Clustering.o Hashtable.o
SOURCE = CMDParser.cpp main.cpp Parser.cpp Database.cpp Item.cpp Vector.cpp test.cpp Curve.cpp ConfParser.cpp Clustering.cpp Hashtable.cpp
HEADER = CMDParser.hpp main.hpp Parser.hpp Database.hpp Item.hpp Vector.hpp Curve.hpp ConfParser.hpp Clustering.hpp Hashtable.h
OUT = cluster test
CC = g++
FLAGS = -g -c -std=c++11 -Wall

cluster : $(OBJS)
		$(CC) -g $(OBJS) -lpthread -o cluster
test : $(TESTOBJS)
		$(CC) -g $(TESTOBJS) -o ../test/test
test.o : ../test/test.cpp
		$(CC) $(FLAGS) ../test/test.cpp
main.o: main.cpp
		$(CC) $(FLAGS) -lpthread main.cpp
CMDParser.o: CMDParser.cpp
		$(CC) $(FLAGS) CMDParser.cpp
Parser.o : Parser.cpp
		$(CC) $(FLAGS) Parser.cpp
Database.o : Database.cpp
		$(CC) $(FLAGS) Database.cpp
Item.o : Item.cpp
		$(CC) $(FLAGS) Item.cpp
Vector.o : Vector.cpp
		$(CC) $(FLAGS) Vector.cpp
Curve.o : Curve.cpp
		$(CC) $(FLAGS) Curve.cpp
ConfParser.o : ConfParser.cpp
		$(CC) $(FLAGS) ConfParser.cpp
Clustering.o : Clustering.cpp
		$(CC) $(FLAGS) Clustering.cpp
Hashtable.o : Hashtable.cpp
		$(CC) $(FLAGS) Hashtable.cpp


clean:
		rm -f $(OBJS) $(TESTOBJS) $(OUT) out
