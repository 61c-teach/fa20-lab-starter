.globl f

.data
neg3:   .asciiz "f(-3) should be 6, and it is: "
neg2:   .asciiz "f(-2) should be 61, and it is: "
neg1:   .asciiz "f(-1) should be 17, and it is: "
zero:   .asciiz "f(0) should be -38, and it is: "
pos1:   .asciiz "f(1) should be 19, and it is: "
pos2:   .asciiz "f(2) should be 42, and it is: "
pos3:   .asciiz "f(3) should be 5, and it is: "

output: .word   6, 61, 17, -38, 19, 42, 5
.text
main:
    la a0, neg3
    jal print_str
    li a0, -3
    la a1, output
    jal f               # evaluate f(-3); should be 6
    jal print_int
    jal print_newline

    la a0, neg2
    jal print_str
    li a0, -2
    la a1, output
    jal f               # evaluate f(-2); should be 61
    jal print_int
    jal print_newline

    la a0, neg1
    jal print_str
    li a0, -1
    la a1, output
    jal f               # evaluate f(-1); should be 17
    jal print_int
    jal print_newline

    la a0, zero
    jal print_str
    li a0, 0
    la a1, output
    jal f               # evaluate f(0); should be -38
    jal print_int
    jal print_newline

    la a0, pos1
    jal print_str
    li a0, 1
    la a1, output
    jal f               # evaluate f(1); should be 19
    jal print_int
    jal print_newline

    la a0, pos2
    jal print_str
    li a0, 2
    la a1, output
    jal f               # evaluate f(2); should be 42
    jal print_int
    jal print_newline

    la a0, pos3
    jal print_str
    li a0, 3
    la a1, output
    jal f               # evaluate f(3); should be 5
    jal print_int
    jal print_newline

    li a0, 10
    ecall

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (defined above).
# Think: why might having a1 be useful?
f:
    # YOUR CODE GOES HERE!

    jr ra               # Always remember to jr ra after your function!

print_int:
    mv a1, a0
    li a0, 1
    ecall
    jr    ra

print_str:
    mv a1, a0
    li a0, 4
    ecall
    jr    ra

print_newline:
    li a1, '\n'
    li a0, 11
    ecall
    jr    ra
