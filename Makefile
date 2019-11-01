all: main

main: main.cpp
	g++ main.cpp scanner.cpp parser.cpp -o frontEnd
	
clean:
	rm frontEnd input.fs19