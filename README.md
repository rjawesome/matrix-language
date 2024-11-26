# Matrix Language

## How to run
```
## From download
./matrix-lang
## From source
make run # compile and run
make main # compile
./main.out # run compiled
```

## Defining variables
- Data types are fraction and matrix (which consists of fractions)
- The get_matrix function requires user input after the function call
- Everything is passed by value (no pointers)
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

### `get_matrix`
- **Description**: Prompts the user to input a matrix with the specified number of rows and columns.
- **Parameters**:
  - `rows` (Fraction): The number of rows in the matrix.
  - `cols` (Fraction): The number of columns in the matrix.
- **Returns**: A matrix of the specified dimensions filled with user input.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> a
  1 2
  3 4
  ```

### `rref`
- **Description**: Converts a matrix to its reduced row echelon form (RREF).
- **Parameters**:
  - `matrix` (Matrix): The matrix to be converted.
- **Returns**: The matrix in RREF.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> rref(a)
  1 0
  0 1
  ```

### `solve_equations`
- **Description**: Solves a system of linear equations represented by a matrix.
- **Parameters**:
  - `matrix` (Matrix): The matrix representing the system of equations.
- **Returns**: None. Prints the solution to the console.
- **Examples**:
  - No solutions (`x+2y=3`, `4x+8y=6`)
    ```
    >> a = get_matrix(2, 3)
    1 2 3
    4 8 6
    >> solve_equations(a)
    No solutions!
    ```
  - One solution (`x+2y=3`, `2x+4y=6` with a solution of `x=1`, `y=1`):
    ```
    >> a = get_matrix(2, 3)
    1 2 3
    4 5 9
    >> solve_equations(a)
    One solution!
    -------------
    1 1 
    -------------
    ```
  - Many solutions (`x+2y=3`, `2x+4y=6` with a solution of `x=0+t`, `y=3/2-(1/2)t`):
    ```
    >> a = get_matrix(2, 3)
    1 2 3
    2 4 6
    >> solve_equations(a)
    Multiple solutions!
    x0
    -------------
    1 -1/2 
    -------------
    Offset
    -------------
    0 3/2 
    ----------------
    ```

### `orthonormalize`
- **Description**: Orthonormalizes the columns of a matrix using the Gram-Schmidt process.
- **Parameters**:
  - `matrix` (Matrix): The matrix to be orthonormalized.
- **Returns**: The orthonormalized matrix.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> ortho(a)
  √10/10 3√10/10 
  3√10/10 -1√10/10 
  ```

### `qr`
- **Description**: Computes the QR decomposition of a matrix (The orthogonal matrix Q can be retrieved from `ortho(matrix)`).
- **Parameters**:
  - `matrix` (Matrix): The matrix to be decomposed.
- **Returns**: The upper-triangular R matrix from the QR decomposition.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> qr(a)
  √10 7√10/5 
  0 √10/5
  >> ortho(a) * qr(a)
  1 2
  3 4
  ```

### `rows`
- **Description**: Returns the number of rows in a matrix.
- **Parameters**:
  - `matrix` (Matrix): The matrix whose rows are to be counted.
- **Returns**: The number of rows in the matrix.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> rows(a)
  2
  ```

### `cols`
- **Description**: Returns the number of columns in a matrix.
- **Parameters**:
  - `matrix` (Matrix): The matrix whose columns are to be counted.
- **Returns**: The number of columns in the matrix.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> cols(a)
  2
  ```

### `transpose`
- **Description**: Transposes a matrix.
- **Parameters**:
  - `matrix` (Matrix): The matrix to be transposed.
- **Returns**: The transposed matrix.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> transpose(a)
  1 3
  2 4
  ```

### `inverse`
- **Description**: Computes the inverse of a matrix or a fraction.
- **Parameters**:
  - `matrix` (Matrix) or `fraction` (Fraction): The matrix or fraction to be inverted.
- **Returns**: The inverse of the matrix or fraction.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> inverse(a)
  -2 1
  1.5 -0.5
  ```

### `determinant`
- **Description**: Computes the determinant of a matrix.
- **Parameters**:
  - `matrix` (Matrix): The matrix whose determinant is to be computed.
- **Returns**: The determinant of the matrix.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> determinant(a)
  -2
  ```

### `norm_sq`
- **Description**: Computes the squared norm of a matrix.
- **Parameters**:
  - `matrix` (Matrix): The matrix whose squared norm is to be computed.
- **Returns**: The squared norm of the matrix.
- **Example**:
  ```
  >> a = get_matrix(2, 2)
  1 2
  3 4
  >> norm_sq(a)
  30
  ```

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