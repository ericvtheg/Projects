//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"
#include "macros.h"
#include "cache.h"
#include <stdlib.h>
#include <stdio.h>

// Branch History Buffer (Extra Credit)
// int BHT[32]

//External declarations of I-Cache
Block iArray[512] = { false , 0 , 0 };      //Init w/ false valid bit
Cache instCache = { iArray, 8, 16, 512 , false , 9, 51};
Cache * iCache  = &instCache;

//Fetch stall
extern void FetchStall(struct stage_reg_d *new_d_reg){
    new_d_reg->new_pc      = current_stage_d_register->new_pc;
    new_d_reg->instruction = current_stage_d_register->instruction;
    new_d_reg->opcode      = current_stage_d_register->opcode;
}

//Fetch stage
extern void stage_fetch (struct stage_reg_d *new_d_reg)
{
    // variable declaration
    uint32_t    instr;
    uint8_t     opcode;         //opcode of instruction
    int         fiveBitAddy;
    uint64_t    tag;
    uint64_t    target;
    bool        misprediction;
    uint64_t    memVal;
    uint64_t    memStatusVal;
    bool        doneMem = true;
    bool        prevMem = false;

    //Get pc and read instruction
    uint64_t pc = get_pc ();

    //check memStall to see if memory is stalled, if so we need to hold fetch where it stays
    if(current_stage_d_register->prevMem == true){
        //want to use same pc as previous iteration
        //double check this...I'd want to do prev iteration because it was that
        //iteration we needed to start stalling correct?
        prevMem = true;
        pc = pc - 4;
    } else if(current_stage_w_register->memStall == true){
        pc = pc - 4;
    }
    //Check cache to see if instruction is in it
    Block* returnBlock = CheckCache(iCache , pc);
        if( returnBlock == NULL ) {
            doneMem = false;
            if( prevMem == false ) {
                doneMem = memory_read (pc, &instr, 4);
                prevMem = true;
                //printf("In prevMem\n");
            } else {
                doneMem = memory_status(pc, &memStatusVal);
                //printf("In else %x %x\n", pc, instr, memStatusVal);
            }
            if( doneMem == true ) {
                prevMem  = false;
                instr = memStatusVal;
                //do we write to cache in Fetch for I cache?
                CacheWrite(iCache, pc, instr);
                //printf("In doneMem %x %x\n", instr, memStatusVal);
            }
        } else {
            instr = returnBlock->data;
            //printf("Cache hit\n");
        }

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

    //used for wasLD conditional
    uint64_t temp =  current_stage_m_register->rdAddy;

    if(misprediction == 1){ //if mispredict, flush
        new_d_reg->new_pc       = 0;
        new_d_reg->instruction  = 0;
        new_d_reg->opcode       = 0;
        new_d_reg->prevMem      = false;
    }else if( doneMem == false ){ //if not doneMem reading from memory, stall
        //this checks if Fetch is still reading memory if so:
        //then that means Fetch is currently reading from memory

        //this conditional alone makes it so that way Fetch and M
        //can both read from memory at the same time

        FetchStall(new_d_reg);
        new_d_reg->prevMem     = true;
    }else if(current_stage_w_register->memStall == true){
        //If this gets hit that means M stage is currently reading from Memory
        //and needs to stall all prior steps
        FetchStall(new_d_reg);
        new_d_reg->prevMem     = false;
    }
    else if (current_stage_m_register->wasLd == 1 && (current_stage_x_register->rs1Addy == temp || current_stage_x_register->rs2Addy == temp)){ //if ld command and rsAddy are equal then stall
        FetchStall(new_d_reg);
    } else { //else we are done and can continue
        //are these values right
        new_d_reg->new_pc      = pc;
        new_d_reg->instruction = instr;
        new_d_reg->opcode      = opcode;
        new_d_reg->prevMem     = false;
    }
    //printf("Fetch out PC: %llx*****************\n", pc);
    //printf("    opcode: %x instr: %x\n", opcode, instr);

}
