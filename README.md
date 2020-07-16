# C++ - Linear Equations Solver

This program solves up to 100 linear equations.
At first the program reads the equations, then it can perform various operations on them

Rules and Constraints:
1. Variables can be entered in any order
    3x2+2x1+4x3=16 is valid

2. Any variable can appear multiple times
    4x1+3x2-2x1+4x3=16 is valid

3. Constant term can appear multiple times
    2x1+3x2+5+4x3-11=10 is valid

4. When a coefficient equals (1 or -1) the digit 1 is optional
    both 1x1+3x2-1x3=10 and x1+3x2-x3=10 are valid

5. Coefficients can be of any length and include float values

6. Variable names can be of any length

7. User can enter up to 100 equations

8. Any Equation mustn't contain any white spaces


Supported Operations:
1. num_vars
    Print the number of variables

2. equation i
    Print equation number i (i is integer)

3. column var
    Print the column of coefficients of variable (var)

4. add 1 3
    Add equation 1 and equation 3 and print the result equation

5. subtract 1 3
    Subtract equation 1 minus equation 3 and print the result equation

6. substitute var 1 3
    Substitutes the variable (var) in equation 1 by its equation in equation 3 and print the result equation

7. D
    Cramer’s Coefficient matrix

8. D var
    Cramer’s Coefficient matrix of variable (var)

9. D_value
    Cramer’s coefficient matrix determinant value

10. solve
    Print the solution of the equations

11. quit
    Exit the program
