# Matrix Language

## How to run
```
make run # compile and run
make main # compile
./main.out # run compiled
```

## Defining variables
- Data types are fraction and matrix (which consists of fractions)
- The get_matrix function requires user input after the function call
```
>> x = -5√2/7
-5√2/7
>> y = -√2/7
-1√2/7
>> z = 10
10
>> a = 5/10
1/2
>> a = get_matrix(2, 2)
1/2 √2 1
3
1/2 √2 
1 3
>> a
1/2 √2 
1 3
```

## Function calls
- Functions work similar to most other languages
```
function_name(parameter1, parameter2, ...)
```

## Default Functions

## Infix Operators
- The * operator between matrices uses matrix-matrix multiplciation
- The * operator between matrices and fractions uses scalar-matrix multiplication
- The only supported operators are + and * (to do division just inverse the second fraction and multiply)
- The * operator is prioritized over the + operator (unless parenthesis are used)
```
>> a = get_matrix(2, 2)
1 2 3 4
1 2
3 4
>> a * transpose(a)
5 11 
11 25 
>> (5/2 + 8/2) * a
13/2 13 
39/2 26 
>> 5/2 * inverse(8/2)
5/8
```