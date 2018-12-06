#------------------------------------------------------------------------------
# Makefile for CE110 Programming Assignment 1
#
#       make                   makes RISCVsim
#		make clean			   cleans out executable and object files
#		make test			   makes and executes test
#------------------------------------------------------------------------------
#Add -Wall after c99 for warnings

RISCVsim : riscv_sim_framework.o execute.o
	gcc -o RISCVsim riscv_sim_framework.o execute.o

riscv_sim_framework.o : riscv_sim_framework.c riscv_sim_framework.h
	gcc -c riscv_sim_framework.c

test: execute.o test.o
	gcc -o test test.o execute.o
	./test

test.o: test.c
	gcc -c test.c

execute.o : execute.c riscv_sim_framework.h
	gcc -c execute.c

clean :
	rm -f RISCVsim riscv_sim_framework.o execute.o