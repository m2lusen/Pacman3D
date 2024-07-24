all:
	g++ -I inc/include -L inc/lib -o main src/main.cpp -lmingw32 -lSDL2main -lSDL2