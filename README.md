# my_c
A small toy language for exploring compilers

+Requirements:
1) Linux Operating System
2) Bison
3) Flex
4) g++ supporting the C++17 Standard

+Instructions:
1) Ensure Flex and Bison are installed and in the path.
2) Run the makefile in the src directory with the command "make" to compile the interpreter.
3) To evaluate programs, pipe output from a file into my-c, e.g. cat ../examples/prog5 | ./my-c

+Example Programs:
1) prog1 is a blanket test of the my_c language.
2) prog2 finds the range of an array, i.e. the difference between its maximum value and its minimum value.
3) prog3 is a Fizzbuzz.
4) prog4 calculates the sum of numbers 1 through 50 using a while loop and compares it to a known formula. 
5) prog5 calculates the factorial of 10.
