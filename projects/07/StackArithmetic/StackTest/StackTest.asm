@17
D=A
@SP
A=M
M=D
@SP
AM=M+1
@17
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF0
D;JEQ
@SP
A=M-1
M=0
@END0
0;JMP
(IF0)
@SP
A=M-1
M=-1
(END0)
@17
D=A
@SP
A=M
M=D
@SP
AM=M+1
@16
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF1
D;JEQ
@SP
A=M-1
M=0
@END1
0;JMP
(IF1)
@SP
A=M-1
M=-1
(END1)
@16
D=A
@SP
A=M
M=D
@SP
AM=M+1
@17
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF2
D;JEQ
@SP
A=M-1
M=0
@END2
0;JMP
(IF2)
@SP
A=M-1
M=-1
(END2)
@892
D=A
@SP
A=M
M=D
@SP
AM=M+1
@891
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF3
D;JLT
@SP
A=M-1
M=0
@END3
0;JMP
(IF3)
@SP
A=M-1
M=-1
(END3)
@891
D=A
@SP
A=M
M=D
@SP
AM=M+1
@892
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF4
D;JLT
@SP
A=M-1
M=0
@END4
0;JMP
(IF4)
@SP
A=M-1
M=-1
(END4)
@891
D=A
@SP
A=M
M=D
@SP
AM=M+1
@891
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF5
D;JLT
@SP
A=M-1
M=0
@END5
0;JMP
(IF5)
@SP
A=M-1
M=-1
(END5)
@32767
D=A
@SP
A=M
M=D
@SP
AM=M+1
@32766
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF6
D;JGT
@SP
A=M-1
M=0
@END6
0;JMP
(IF6)
@SP
A=M-1
M=-1
(END6)
@32766
D=A
@SP
A=M
M=D
@SP
AM=M+1
@32767
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF7
D;JGT
@SP
A=M-1
M=0
@END7
0;JMP
(IF7)
@SP
A=M-1
M=-1
(END7)
@32766
D=A
@SP
A=M
M=D
@SP
AM=M+1
@32766
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
D=M-D
@IF8
D;JGT
@SP
A=M-1
M=0
@END8
0;JMP
(IF8)
@SP
A=M-1
M=-1
(END8)
@57
D=A
@SP
A=M
M=D
@SP
AM=M+1
@31
D=A
@SP
A=M
M=D
@SP
AM=M+1
@53
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
M=D+M
@112
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
M=M-D
@SP
A=M-1
M=-M
@SP
AM=M-1
D=M
A=A-1
M=D&M
@82
D=A
@SP
A=M
M=D
@SP
AM=M+1
@SP
AM=M-1
D=M
A=A-1
M=D|M
@SP
A=M-1
M=!M