main: main.cpp
	g++ main.cpp -o main.app

clean:
	rm -f main.app

run: main.app
	./main.app
