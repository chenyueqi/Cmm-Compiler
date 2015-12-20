.data
v3: .space 12
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra

separate:
	li $t8, 0
	li $t8, 0
	li $t0, 0
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 100
	div $t8, $t0
	mflo $t8
	move $t8, $t8
	li $t0, 1
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 10
	div $t8, $t0
	mflo $t8
	li $t0, 0
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 10
	mul $t8 $t0, $t8
	move $t8, $t8
	li $t0, 2
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 0
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 100
	mul $t8 $t0, $t8
	li $t0, 1
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 10
	mul $t8 $t0, $t8
	move $t8, $t8
	li $t0, 0
	li $t1, 4
	mul $t8 $t0, $t1
	li $t0, 2
	li $t1, 4
	mul $t8 $t0, $t1
label4:
	li $t8, 1
label8:
	addi $t8 $t8, 1
	j label4
label6:
	j label3
label2:
	li $t8, 0
label3:
	j label11
label10:
label11:
main:
	li $t8, 120
label12:
label16:
	addi $t8 $t8, 1
	j label12
label14:
