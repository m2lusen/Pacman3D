all:
	g++ -I inc/include -L inc/lib -o main src/main.cpp src/player.cpp src/map.cpp src/raycasting.cpp src/enemy.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
