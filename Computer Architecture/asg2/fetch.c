//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"
#include "macros.h"
#include <stdlib.h>
#include <stdio.h>

// Branch History Buffer (Extra Credit)
// int BHT[32]

//Fetch stage
extern void stage_fetch (struct stage_reg_d *new_d_reg)
{
    // variable declaration
    uint32_t    instr;
    uint8_t     opcode;         //opcode of instruction
    int fiveBitAddy;
    uint64_t tag;
    uint64_t target;
    bool misprediction;

    //Get pc and read instruction
    uint64_t pc = get_pc ();
    memory_read (pc, &instr, 4);
    //printf("Intruction: %x\n", instr);

    if(current_stage_m_register != NULL)
        misprediction = current_stage_m_register->misprediction;

    //Get opcode
    opcode = (uint8_t )( (instr) & OPCODE_MASK );
    //printf("opcode in fetch: %x\n", opcode);
    if ( opcode == OP_JAL || opcode == OP_JALR || opcode == OP_BRANCH ) {
        // target = btb[tag]
        // if tag exists, update target


        tag = BTB[fiveBitAddy][0];
        if(tag == pc){
            target = BTB[fiveBitAddy][1];
            set_pc(target);
        }
    } else if(misprediction == 1){
        //do nothing
        //set pc in x should have correctly updated the pc
        //set_pc(current_stage_m_register->new_pc);
    } else {
        set_pc(pc + 4);
    }

    if(misprediction == 1){
        new_d_reg->new_pc       = 0;
        new_d_reg->instruction  = 0;
        new_d_reg->opcode       = 0;
    }else if (current_stage_m_register->wasLd == 1){
        uint64_t    temp =  current_stage_m_register->rdAddy;
        if(current_stage_x_register->rs1Addy == temp|| current_stage_x_register->rs2Addy == temp){
            new_d_reg->new_pc       = current_stage_d_register->new_pc;
            new_d_reg->instruction  = current_stage_d_register->instruction;
            new_d_reg->opcode       = current_stage_d_register->opcode;
        }
    } else {
        new_d_reg->new_pc       = pc;
        new_d_reg->instruction  = instr;
        new_d_reg->opcode       = opcode;
    }
    /*printf("Fetch out PC: %llx*****************\n", pc);
    printf("    opcode: %x instr: %x\n", opcode, instr);*/

}
