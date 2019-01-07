#------------------------------------------------------------------------------
# Makefile for CE110 Programming Assignment 3
#
#       make                   makes RISCVsim
#		make clean			   cleans out executable and object files
#------------------------------------------------------------------------------
#Add -Wall after c99 for warnings
C_FILES   	   = riscv_sim_pipeline_framework.c fetch.c decode.c x.c memory.c writeback.c cache.c
OBJECT_FILES   = riscv_sim_pipeline_framework.o fetch.o decode.o x.o memory.o writeback.o cache.o
HEADERS        = macros.h riscv_sim_pipeline_framework.h riscv_pipeline_registers.h
COMPILE        = gcc -c 
LINK           = gcc -o
REMOVE         = rm -f
OBJDIR		   = obj
OBJS  		   = $(addprefix ./$(OBJDIR)/, $(OBJECT_FILES))

RISCVsim : $(OBJS)
	$(LINK) RISCVsim $(OBJS)

%.o : %.c $(HEADERS)
	$(COMPILE) $<
	mv $@ $(OBJDIR)

$(OBJS) : $(OBJDIR) $(OBJECT_FILES)

$(OBJDIR) :
	mkdir $(OBJDIR)

clean :
	$(REMOVE) RISCVsim $(OBJDIR)/*.o