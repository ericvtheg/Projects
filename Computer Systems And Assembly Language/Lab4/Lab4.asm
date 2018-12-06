# Eric Ventor 
#eventor - 1492157
#01E-Justin Lee
#Lab4
#11/12/2017
#Finding primes by implementing a sieve of eratosthenes

.text
main:
	li	$v0, 4
	la	$a0, hello_msg
	syscall
	
	la 	$a0, prompt_msg
	syscall
	
	la 	$a0, input_msg
	syscall
	
	li	$v0, 5			#takes in integer input
	syscall
	
	move	$s0, $v0		#save user input into s0
	
	addi 	$s1, $zero, 1		#base reg
	addi	$s2, $zero, 1		#used to indicate whether prime or not	
	addi	$t4, $zero, 1		#bool used to check if first run through

prime_finder:	#using sieve of eratosthenes
	addi	$s1, $s1, 1		#increment base address by 1
	
	sgt	$t3, $s1, $s0		#if base reg becomes greater than input
	bnez	$t3, exit		#exit 
	
	la	$t0, array		#load in base address
	add	$s3, $s1, $t0		#add base reg with base address to increment
	lb	$t1, ($s3)		#load actual value where index reg is pointing
	bnez	$t1, prime_finder	#if the array elem does not equal zero, then it is prime, continue
	
	beq 	$t4, 1, flag		#skip comma and white space if first iteration
	
	#prints spaces and commas
	li	$v0, 4
	la	$a0, space
	syscall
	
flag:
	li	$t4, 0			#set flag to false after first iteration
	
	li	$v0, 1		
	add	$a0, $zero, $s1		#print base address i.e. prime number
	syscall

inner:
	add 	$s3, $s3, $s1		#add base reg with index to increment throughout whole array
	sb	$s2, 0($s3) 		#store 1 indiciating that it is not prime
	
	#debugg: used to check what is being stored into array elem
	#lb	$a0, ($s3)		#store in arg to print actual value of index pointer
	#syscall	
	
	sub	$t2, $s3, $t0		#get actual value of index instead of memory address index
	
	#debug: check non-memory index values
	#move	$a0, $t2
	#syscall
	
	li	$t3, 0
	sge  	$t3, $t2, $s0		#if index is greater than index
	bnez 	$t3, prime_finder	#continue in prime finder
	j 	inner			#else continue with inner loop

exit:
	li	 $v0, 10 		#exit
	syscall
	
# Data for the program:
.data
hello_msg: .asciiz 	"Prime Finder \n"
prompt_msg: .asciiz	"Enter an integer \n"
input_msg: .asciiz	"Input: "
space: .asciiz		", "
new_line: .asciiz	"\n"
array: .space 		10000	

