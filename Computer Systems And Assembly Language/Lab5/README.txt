Eric Ventor 
eventor - 1492157
01E-Justin Lee
Lab5
11/26/2017
Encrypting/decrypting using a cipher

Discuss the encode/decode algorithms. What similarities are there between them?
	The encode and decode functions were very similar to one another. They are basically just inverses. 
For encrypt you need to add together the ascii values of they key and clear text. Then save the remainder
of dividing the sum by 128. 

	For decrypt you simply do the opposite of encrypt. Instead of adding the key and cleartext you subtract 
the encrypted text by the key. But still save the remainder of dividing the difference by 128.

	Some other similarities include having multiple pointers for different locations on arrays. Then loading in
one letter from a select few of these pointers and use the letter for comparisons and operations. A crucial 
part of each of these functions was to reset the pointer once the end of key was reached. 

Discuss the functions you made and what you were required to save.
	As stated earlier encrypt and decrypt are p much the same except for it being an 
inverse math process. 

	The pointers that I saved were: base of key
									key pointer to be incremented
									base of clear text (or encoded array for decode function)
									pointer to encoded array(or decoded array for decode function)

	A good way to discuss the function I made is through my pseudo code:
Encrypt:
	For length of clear text
	get the letter that is being pointed to in key
	get letter that is pointed to in cleartext
	add both letters together
	take sum then mod 128
	store rem in encoded array
	check if cleartext pointer has reached null
	if yes then done
	else increment clear text pointer
	check if key pointer has reached null
	if yes set equal to base key pointer
	else increment key pointer
	j Encrypt
	