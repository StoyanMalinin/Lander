all:	build

build:	Lander

Lander:
	g++ -o Lander src/*.cpp -Isrc -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

clean:
	rm -f Lander
