Eric Ventor
Eventor@ucsc.edu
1492157
Lab3
01E-Justin Lee

1.Discuss the algorithm(s) you designed. Were there any issues in implementing them?
    When I was designing my StringToInt algorithim I was unaware I wouldn't be able to 
easily call a built-in function to find the length of my String. This caused me to have to create 
a helper function to count the number of chars including the null character.
    
    Having to impelement algorithims that were mainly based on higher level languages 
did cause certain problems/errors. After this assignment I feel much more comftorable in
my ability to design and implement algorithims in lower level languages. 

    The printing in binary algorithim on the assignment sheet of comparing each individual 
bit to a 'bitmask' and 'and'ing each bit to see whether it was 0 or 1 then printing individually 
on one line. Worked very well and felt easy to code within assembly. 
    
    The way allocating memory instead of actual variables was a bit confusing at first,
but I quickly got the hang of it. MIPS as a program was extremely helpful in being able 
to understand my code and see what exactly is going on behind the scenes.

    I originally designed my program to use syscall 8 to receive an input from the user, 
but soon after realized that we were using the program arguments within the excecute tab 
of MIPS. 



2.Discuss any assembly language techniques you developed or discovered.
    I discovered how loops are implemented in assembly. Instead of parameters being 
included within the loop it simply just reiterates by "jumping" back to the called 
function. The way to leave this loop would be a conditional "branching" method.

    I learned about bitmasks and how they are used to "see" into the memory and be able 
to know what values of the 32 bits are 1's or 0's by using an and function to compare 
and iterating. 
 
    One of my favorite parts of assembly was being able to run the function one command a time 
making it much easier to be able to see where exactly bugs were occuring. 

    Overall the learning curve for assembly was pretty steep, but once I got the hang of it 
things started to click much easier. 



3.Several pseudo-ops were needed to complete this lab. Compare the code written with the
assembled code and look at what the assembler did. How many more lines of assembly were
written? 

    As I stated above there was a large learning curve for me in assembly. I was used to
a higher level programming language where several functions are built in or importable which
obviously makes coding much easier. Without these and with the somewhat obscure command names
(i.e. li, lb, move, syscall, etc) it was very difficult to understand how I should be writing my code.

    The number of lines in the assembled program was 69, verses my 155 in my non-assembled code. 
MIPS works by using built in commands that can be combinations of things like OR or AND. That return 
the desired result. Some built in commands require more than just one line of actual operation so in the 
assembled code it uses commands that I had not used. For example addu, ori, and lui. So I suppose that 
there is an even lower level assembly language that I have yet to tackle.  

    