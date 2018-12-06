# Eric Ventor 
#eventor - 1492157
#01E-Justin Lee
#Lab5
#11/26/2017
#Encrypting/decrypting using a cipher
.text
main:
	move	$s0, $a1 	#saves key and clear text into s0
	lw	$s1, 0($s0)	#load in key of prog arg
	
	lw	$s3, 4($s0)	#load in cleartext part of prog arg
	add	$t2, $zero, 0	#intialize var that holds length of cleartext
	
	li	$v0, 4
	la	$a0, key_prompt	#prints key prompt
	syscall
	la	$a0, ($s1)	#prints key
	syscall
	la	$a0, new_line	
	syscall
	la	$a0, text_prompt	
	syscall
	la	$a0, ($s3)	#prints cleartext
	syscall
	la	$a0, new_line
	syscall
	la	$a0, en_prompt
	syscall
	
	#encode func call preparation
	la	$a0, ($s1)	#key
	la	$a1, ($s3)	#cleartext
	la	$a2, encoded	#ciphertext
	jal	encode
	
	la	$a0, new_line
	syscall
	la	$a0, de_prompt
	syscall
	
	#decode func call preparation
	la	$a0, ($s1)	#key
	la	$a1, encoded	#notcleartext
	la	$a2, decoded	#ciphertext
	jal	decode
	
	j 	exit		#exit after decoding

encode:	#a0 = key, #a1 - cleartext, #a2 = ciphertext 
	move	$s2, $a0	#base reg for key
	move	$t3, $s2	#key pointer to be incremented
	move	$s4, $a1	#base reg for clear text
	move	$s5, $a2	#pointer to encode array
	
loop:
	lb	$t4, 0($t3)	#get first letter of key
	lb	$t5, 0($s4)	#get letter of clear text
	
	#debugging: see what key is pointing at
	#li	$v0, 11
	#lb	$a0, ($t4)
	#syscall
	
	add	$t6, $t5, $t4	#half cipher'd letter
	remu	$t6, $t6, 128	#done ciphered letter in t6
	
	beqz	$t5, encode_done
	addi	$s4, $s4, 1	#increment cleartext pointer
	
	sb   	$t6, 0($s5)	#store into declared encoded array
	
	addi	$s5, $s5, 1	#increment array pointer
	
	la	$a0, -1($s5)
	syscall
	
	addi	$t3, $t3, 1	#increment key pointer
	lb	$t4, 0($t3)	#get first letter of key
	beqz 	$t4, key_reset	#needs to set keypointer to base
	j	loop
	
key_reset:
	move	$t3, $s2	#set key pointer to base
	j	loop
	
encode_done:
	jr	$ra	
	
decode: #a0 = key, #a1 - notclearText, #a2 = decodedText 
	move	$s2, $a0	#base key reg
	move	$t3, $s2	#key pointer to be incremented
	move	$s4, $a1	#base encoded array
	move	$s5, $a2	#base decoded array to be incremented
	
decode_loop:
	lb	$t4, 0($s4)	#load in encrpyted letter
	lb	$t5, 0($t3)	#load in key letter
	
	#debugging: prints out values for encrypted letter array pointer and key array pointer
	#li	$v0, 11
	#add	$a0, $t4, $zero
	#syscall
	#add	$a0, $t5, $zero
	#syscall
	
	subu  	$t6, $t4, $t5	#cipher operations
	remu 	$t6, $t6, 128
	
	#debugging: prints out letter that has been decrypted
	#li	$v0, 11
	#add	$a0, $t6, $zero
	#syscall
	
	addi	$s4, $s4, 1	#increment encoded array
	
	#debugging: check what encoded array is pointing at after incrementing
	#lb	$a0, 0($s4)
	#syscall
	
	beqz	$t4, decode_done
	
	sb	$t6, ($s5)	#storing byte into decoded array
	
	li	$v0, 11
	lb	$a0, 0($s5)
	syscall
	
	addi	$s5, $s5, 1	#increment decoded array
	
	addi	$t3, $t3, 1	#increment key pointer
	lb	$t5, 0($t3)	#get first letter of key
	beqz	$t5, d_key_reset
	j 	decode_loop
d_key_reset:
	move	$t3, $s2
	j decode_loop
	
decode_done:
	#need to null terminate decode array
	jr	$ra
	
done:	#have to store a 0 to null terminate the strings -- is this already being done?? --or is
	#or is it only for the declared arrays


exit:
	#debugging:
	#li	$v0, 1
	#la	$a0, ($t0)	#prints length of key
	#syscall
	#la	$a0, ($t2)	#prints length of cleartext
	#syscall
	
	#debugging: checking entire encoded and decoded arrays
	#li	$v0, 4
	#la	$a0, new_line
	#syscall
	#la	$a0, encoded
	#syscall
	#la	$a0, new_line
	#syscall	
	#la	$a0, decoded
	#syscall
	
	li	$v0, 10 		#exit
	syscall
	
.data
key_prompt: .asciiz 	"The given key is: "
text_prompt: .asciiz	"The given text is: "
en_prompt: .asciiz	"The encrypted text is: "
de_prompt: .asciiz	"The decrypted text is: "
new_line: .asciiz	"\n"
encoded: .space 	101	#array
decoded: .space		101	#array