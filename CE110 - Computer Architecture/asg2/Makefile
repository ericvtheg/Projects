#------------------------------------------------------------------------------
# Makefile for CE110 Programming Assignment 2
#
#       make                   makes RISCVsim
#		make clean			   cleans out executable and object files
#		make test			   makes and executes test
#------------------------------------------------------------------------------
#Add -Wall after c99 for warnings
C_FILES   	   = riscv_sim_pipeline_framework.c fetch.c decode.c x.c memory.c writeback.c
OBJECT_FILES   = riscv_sim_pipeline_framework.o fetch.o decode.o x.o memory.o writeback.o
HEADERS        = macros.h riscv_sim_pipeline_framework.h riscv_pipeline_registers.h
COMPILE        = gcc -c 
LINK           = gcc -o
REMOVE         = rm -f

RISCVsim : $(OBJECT_FILES)
	$(LINK) RISCVsim $(OBJECT_FILES)

riscv_sim_pipeline_framework.o : riscv_sim_pipeline_framework.c $(HEADERS)
	$(COMPILE) riscv_sim_pipeline_framework.c

fetch.o : fetch.c $(HEADERS)
	$(COMPILE) fetch.c

decode.o : decode.c $(HEADERS)
	$(COMPILE) decode.c

x.o : x.c $(HEADERS)
	$(COMPILE) x.c

memory.o : memory.c $(HEADERS)
	$(COMPILE) memory.c

writeback.o : writeback.c $(HEADERS)
	$(COMPILE) writeback.c

test: fetch.o decode.o x.o memory.o writeback.o test.o
	$(LINK) test fetch.o decode.o x.o memory.o writeback.o test.o
	./test

test.o: test.c
	$(COMPILE) test.c

clean :
	$(REMOVE) RISCVsim *.o