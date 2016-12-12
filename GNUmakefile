
csv_lexer.o: csv_lexer.hpp csv_lexer.cpp
	g++ -g -c csv_lexer.cpp -o $@

csv_test.o: csv_test.cpp
	g++ -g -c csv_test.cpp -o $@


csv_test.exe: csv_test.o csv_lexer.o
	g++ -g csv_lexer.o csv_test.o -o $@


all: csv_test.exe

clean:
	del *.o
	del csv_test.exe
