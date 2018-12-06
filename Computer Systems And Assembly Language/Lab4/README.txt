Eric Ventor 
eventor - 1492157
01E-Justin Lee
Lab4
11/12/2017

Finding primes for all numbers less or equal to input 
by implementing a sieve of eratosthenes. 

I did this by starting at a base reg 2 incrementing it by itself,
and recording that the numbers hit as not prime.

I then increment the base number by 1 and check if it has 
been marked as not prime. If it has then add base by 1 again.

If it has been marked as prime then add that number to itself 
continously marking numbers hit as not prime.

This process continues until base reg equals or is greater than the 
input number. 

This programming assignment I really had to sit down and write every 
aspect of the pseudocode then slowly convert each step into 
assembly. This made the process and grasping the concept much easier 
and left me with little to no debugging. 