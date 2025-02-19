# Default rule to build the program
all: main

# Rule to build the target executable
main:
	g++ -DMAIN -std=c++17 -g -O3 -mavx2 -Iinc -Wno-ignored-attributes src/*.cpp -o main.out

# Rule to build the Windows executable
main-win:
	x86_64-w64-mingw32-g++ -DMAIN -std=c++17 -g -O3 -mavx2 -Iinc src/*.cpp -o main.exe

# Rule to clean up build files
clean:
	rm -f main.out

# Rule to build and run the program directly
run: main
	./main.out