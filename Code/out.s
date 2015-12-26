.data
_v1: .space 40
_v2: .space 4
_t1: .space 4
_t2: .space 4
_t3: .space 4
_t4: .space 4
_t5: .space 4
_t6: .space 4
_v3: .space 4
_t12: .space 4
_t13: .space 4
_t14: .space 4
_t15: .space 4
_t16: .space 4
_t17: .space 4
_v4: .space 4
_t19: .space 4
_t20: .space 4
_t21: .space 4
_t22: .space 4
_t24: .space 4
_t25: .space 4
_t32: .space 4
_t33: .space 4
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
	li $t5, 0
label1:
	li $t0, 10
	bge $t5, $t0, label3
	li $t0, 4
	mul $t6, $t5, $t0
	la $t2, _v1
	add $t7, $t2, $t6
	mul $t8, $t5, $t5
	li $t0, 6
	mul $t9, $t0, $t5
	sub $s0, $t8, $t9
	addi $s1, $s0, 2
	sw $s1, 0($t7)
	addi $t5, $t5, 1
	j label1
label3:
	li $t5, 0
label4:
	li $t0, 10
	bge $t5, $t0, label6
	move $s2, $t5
label7:
	li $t0, 10
	bge $s2, $t0, label9
	li $t0, 4
	mul $s3, $t5, $t0
	la $t2, _v1
	add $s4, $t2, $s3
	li $t0, 4
	mul $s5, $s2, $t0
	la $t2, _v1
	add $s6, $t2, $s5
	lw $t3, 0($s4)
	lw $t4, 0($s6)
	ble $t3, $t4, label11
	li $t0, 4
	mul $s7, $s2, $t0
	la $t2, _v1
	add $s8, $t2, $s7
	la $t2, _v2
	sw $t5, 0($t2)
	lw $t5, 0($s8)
	la $t2, _t6
	sw $s1, 0($t2)
	li $t0, 4
	mul $s1, $s2, $t0
	la $t2, _t16
	sw $s7, 0($t2)
	la $t2, _v1
	add $s7, $t2, $s1
	la $t2, _t4
	sw $t9, 0($t2)
	la $t2, _t14
	sw $s5, 0($t2)
	li $t0, 4
	mul $t9, $s5, $t0
	la $t2, _t2
	sw $t7, 0($t2)
	la $t2, _v1
	add $t7, $t2, $t9
	lw $t3, 0($s7)
	lw $t3, 0($t7)
	la $t2, _t12
	sw $s3, 0($t2)
	li $t0, 4
	mul $s3, $s5, $t0
	la $t2, _v4
	sw $t5, 0($t2)
	la $t2, _v1
	add $t5, $t2, $s3
	la $t2, _t19
	sw $s1, 0($t2)
	sw $s1, 0($t5)
label11:
	addi $s2, $s2, 1
	j label7
label9:
	addi $s5, $s5, 1
	j label4
label6:
	li $s5, 0
label12:
	li $t0, 10
	bge $s5, $t0, label14
	la $t2, _t20
	sw $s7, 0($t2)
	li $t0, 4
	mul $s7, $s5, $t0
	la $t2, _t21
	sw $t9, 0($t2)
	la $t2, _v1
	add $t9, $t2, $s7
	lw $t4, 0($t9)
	move $a0, $t4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $s5, $s5, 1
	j label12
label14:
	li $t1, 0
	move $v0, $t1
	jr $ra
