.data
_t1: .space 4
_v2: .space 20
_t2: .space 4
_t3: .space 4
_t7: .space 4
_t8: .space 4
_t9: .space 4
_t10: .space 4
_t11: .space 4
_t12: .space 4
_t13: .space 4
_t16: .space 4
_t17: .space 4
_t18: .space 4
_t19: .space 4
_t23: .space 4
_t21: .space 4
_t22: .space 4
_t26: .space 4
_t24: .space 4
_t25: .space 4
_t27: .space 4
_t34: .space 4
_t35: .space 4
_t36: .space 4
_t37: .space 4
_t39: .space 4
_t40: .space 4
_t41: .space 4
_t42: .space 4
_t43: .space 4
_t44: .space 4
_t45: .space 4
_t46: .space 4
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

absolute:
	j label3
label2:
	addi $t0 $t0, -0
label3:
main:
	li $t0, 0
	li $t0, 0
	li $t0, 0
	li $t0, 0
label4:
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	li $t0, 0
	addi $t0 $t0, 1
	j label4
label6:
label7:
	li $t0, 4
	li $t1, 4
	mul $t0 $t0, $t1
	add $t0 $t0, $t0
	li $t0, 0
	li $t1, 4
	mul $t0 $t0, $t1
	add $t0 $t0, $t0
	li $t0, 0
	li $t1, 4
	mul $t0 $t0, $t1
	add $t0 $t0, $t0
	addi $t0 $t0, 1
	move $t0, $t0
	li $t0, 0
label10:
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	li $t0, 0
	addi $t0 $t0, 1
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	addi $t0 $t0, 1
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	addi $t0 $t0, 1
	move $t0, $t0
label14:
	addi $t0 $t0, 1
	j label10
label12:
	li $t0, 1
	li $t0, 0
	li $t0, 0
label15:
label18:
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	li $t0, 0
	j label25
label24:
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	li $t0, 4
	mul $t0 $t0, $t0
	add $t0 $t0, $t0
	sub $t0 $t0, $t0
	sub $t0 $t0, $t0
	li $t0, 0
label27:
label25:
label22:
	addi $t0 $t0, 1
	j label18
label20:
	li $t0, 0
	addi $t0 $t0, 1
	j label15
label17:
	add $t0 $t0, $t0
	j label7
label9:
