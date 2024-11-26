# Default rule to build the program
all: main

# Rule to build the target executable
main:
	g++ -std=c++17 -g -Iinc src/*.cpp -o main.out

# Rule to build the Windows executable
main-win:
	x86_64-w64-mingw32-g++ -std=c++17 -g -Iinc src/*.cpp -o main.exe

# Rule to clean up build files
clean:
	rm -f main.out

# Rule to build and run the program directly
run: main
	./main.out