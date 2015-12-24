.data
_v1: .space 4
_v2: .space 4
_t1: .space 4
_t2: .space 4
_v3: .space 4
_t5: .space 4
_v4: .space 4
_t10: .space 4
_t11: .space 4
_t12: .space 4
_t13: .space 4
_t14: .space 4
_v5: .space 4
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

main:
	li $t3, 15
	li $t4, 28
	sub $t5, $t3, $t4
	li $t0, 2
	div $t5, $t0
	mflo $t6
	add $t7, $t6, $t3
	li $t0, 3
	mul $t8, $t7, $t0
	add $t3, $t8, $t3
	sub $t9, $t3, $t7
	move $a0, $t9
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	div $t3, $t4
	mflo $s0
	mul $s1, $s0, $t7
	div $t9, $t7
	mflo $s2
	li $t0, 4
	mul $s3, $s2, $t0
	add $s4, $s1, $s3
	sub $s5, $s4, $t4
	move $a0, $s5
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	li $t1, 0
	jr $ra
