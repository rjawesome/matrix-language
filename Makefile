# Default rule to build the program
all: equations

# Rule to build the target executable
equations:
	g++ equations.cpp utils.cpp -o equations.out

# Rule to clean up build files
clean:
	rm -f equations.out

# Rule to build and run the program directly
run: equations
	./equations.out