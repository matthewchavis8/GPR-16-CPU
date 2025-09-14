// Set R2 = 0
@R2
M=0

// Load R1 (multiplier) into D (loop cnt)
// Loading the value of the second number we want to multiply into the tmp register D
@R1
D=M

// If D == 0, kill the program
// If the number is 0 we know it will be 0 so just end the program
@END
D;JEQ

// Define label Loop
(LOOP)
// load value at Ram[0] and add to Ram[2]
// Add R0 to R2 product
@R0
D=M
@R2
M=M+D

// Decrement cntr
@R1
M=M-1

// Load cntr to D
@R1
D=M

@LOOP
D;JGT

(END)
// Infinite loop to end program
@END
0;JMP

