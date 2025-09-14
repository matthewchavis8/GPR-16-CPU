(LOOP)
    @KBD
    D=M
    @BLACKEN
    D;JNE      // If key is pressed
    @CLEAR
    0;JMP      // Else go clear screen

// Blackens the screen
(BLACKEN)
    @SCREEN
    D=A
    @addr
    M=D         // addr = SCREEN base
    @8192
    D=A
    @CNT
    M=D         // CNT = 8192 words

(BLACKEN_LOOP)
    @addr
    A=M
    M=-1        // Write black pixels
    @addr
    M=M+1       // addr++
    @CNT
    M=M-1       // CNT--
    D=M
    @BLACKEN_LOOP
    D;JGT       // Keep looping if CNT > 0
    @LOOP
    0;JMP

// Clears the screen
(CLEAR)
    @SCREEN
    D=A
    @addr
    M=D         // addr = SCREEN base
    @8192
    D=A
    @CNT
    M=D         // CNT = 8192 words

(CLEAR_LOOP)
    @addr
    A=M
    M=0         // Clear pixel word
    @addr
    M=M+1       // addr++
    @CNT
    M=M-1       // CNT--
    D=M
    @CLEAR_LOOP
    D;JGT       // Keep looping if CNT > 0
    @LOOP
    0;JMP
