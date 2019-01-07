//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"
#include "macros.h"
#include <stdlib.h>
#include <stdio.h>

//Writeback stage
extern void stage_writeback (void)
{
    //local variables
    uint8_t     opcode;
    uint64_t    rdAddy;
    uint64_t    rdVal;

    //retrieve values from stage registers
    opcode = current_stage_w_register->opcode;
    rdAddy = current_stage_w_register->rdAddy;
    rdVal  = current_stage_w_register->rdVal;


    if(opcode == OP_STORE || opcode == OP_BRANCH)
    {
        //do nothing for store
    }
    else if(1)
    {
        register_write (rdAddy, rdVal);
    }
    //printf("wb out PC: %llx*****************\n", current_stage_w_register->new_pc);
    //printf("    rdAddy: %llx rdVal: %llx\n", rdAddy, rdVal);

}
