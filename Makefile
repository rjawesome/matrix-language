# Default rule to build the program
all: main

# Rule to build the target executable
main:
	g++ -Iinc src/*.cpp -o main.out

# Rule to clean up build files
clean:
	rm -f main.out

# Rule to build and run the program directly
run: main
	./main.out