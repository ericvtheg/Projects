# Eric Ventor 
#eventor - 1492157
#01E-Justin Lee
#Lab3
#11/1/2017
# BaseFile.asm-- Starter File to begin coding lab3
# this file will not work correctly unless you have arguments to the program
# be sure to add them before you try to run it.
.text
main:
	move	 $s0,$a1
	lw	 $s3, ($s0)		#used for counting # of chars
	lw       $s1, ($s0) 		#loads base array from program argument
	lw	 $s0,($s0)
	la	 $a0, hello_msg 	# load the addr of hello_msg into $a0.
	li	 $v0, 4 		# 4 is the print_string syscall.
	syscall 			# do the syscall.
	
	#prints input:
	la 	$a0, input_msg
	syscall
	
	#prints program argument value as a string
	la 	$a0, ($s0)
	syscall
	
	#prints new line
	la 	$a0,new_line
	syscall
	
	li 	$t2, 0			# number of chars
	li	$t7 ,'-'			
count_chars: #helper function: counts number of chars in string until null character
	lb 	$t3,0($s3)
	beq 	$t7,$t3,flag
post_flag: 
	beqz 	$t3,done
	addi 	$t2,$t2,1
	addi 	$s3,$s3,1
	j count_chars	

done: #converts string to int based on Darrel's method found on piazza

	#positioning pointer
	add	$s1, $s1, $t2 		#points to null character of array
	#start with 0
	add 	$s2, $zero, $zero  	#setting sum equal to 0
	
	lb 	$t1, 0($s0)		#load char currently being pointed by $s0
	
	addi 	$t1, $t1, -48 		#subtracts ascii value of zero to convert to dec value
	add 	$s2, $s2, $t1		#adds to sum
	add 	$s0,$s0, 1		#increment string array address
	
StI: #string to int

	#multiply it by 10
	li 	$t0, 10 		#used to mult
	beq 	$s1, $s0, bitmask 
	
	mul 	$s2, $s2, $t0  

	
	#take a byte of the string from the biggest (most significant end)
	lb 	$t1, ($s0)	 	#load char from array into t1
	
	#debug: print chars
	#li 	$v0, 11
	#la 	$a0, ($t1)
	#syscall
		
	#Subtract the ASCII value of “0” from it. 
	addi 	$t1,$t1, -48 		#turns ascii value to dec value
		
	#add dec value to sum
	add 	$s2, $s2, $t1 
	addi 	$s0, $s0, 1
		
	#repeat until you have done this for every byte
	beq 	$s1, $s0, bitmask
	j 	StI

#intitializes bit mask
bitmask: #prints text, & intitializes bit mask

	#print "Output: "
	li 	$v0, 4
	la 	$a0, output_msg
	syscall
	
	#debug: print new line for readability
	#li 	$v0, 4
	#la 	$a0, new_line
	#syscall
	
	#declaring/initializing
	add 	$t4, $zero, 1 		#add 1 to the first bit of the binary number
	sll 	$t4,$t4, 31 		#now shift that 1 over 31 for a total of 32. For a 32 bit system
	addi 	$t5, $zero, 32 		#to keep track of # of comparisons

	
bitmask_loop:
	beq 	$s4, 1, flag_true	#jumps to flag_true if flag is true then sets flag to false
	and 	$t6, $s2, $t4		#comparing bitmask with our integer sum	
	beq 	$t6, $zero, print	#if == 0, print
	addi 	$t6, $zero, 1		#otherwise print 1

print: #prints binary outputs
	li 	$v0, 1		
	move 	$a0, $t6 		
	syscall
	
	srl 	$t4, $t4, 1	
	addi 	$t5, $t5, -1		#decrement, # of comparisons left
	
	bne 	$t5, $zero, bitmask_loop
	j	exit

exit:
	#debug: used to check string->int value
	#li 	$v0, 1
	#la 	$a0, ($s2)
	#syscall
	
	#debug: used for readability in comparing binary ouputs
	#li 	$v0, 4
	#la 	$a0, new_line
	#syscall
	
	#debug: used to check value
	#li 	$v0, 35 		#prints binary equivalent of arg
	#la 	$a0, ($s2)
	#syscall
	
	# your code goes here above the exit syscall
	li	 $v0, 10 		# 10 is the exit syscall.
	syscall 			# do the syscall.
	
flag: #sets our flag equal to 1 for future reference
	li	$s4, 1
	addi 	$s0,$s0, 1
	j	post_flag
	
flag_true: #inverts our sum and adds 1 for the 2's complement operation. Then sets flag back to 0
	not 	$s2,$s2
	addi 	$s2,$s2, 1
	li 	$s4, 0
	j 	bitmask_loop
	
# Data for the program:
.data
hello_msg: .asciiz 	"Integer to Binary Converter \n"
input_msg: .asciiz	"Input: "
output_msg: .asciiz	"Output: "
new_line: .asciiz	"\n"

