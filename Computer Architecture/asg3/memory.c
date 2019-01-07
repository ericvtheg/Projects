//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"
#include "macros.h"
#include "cache.h"
#include <stdlib.h>
#include <stdio.h>

//External declarations of D-Cache
Block dArray[2048] = { false , 0 , 0 };          //Init w/ false valid bit
Cache dataCache = { dArray, 16, 8, 2048 , true, 11, 50};
Cache * dCache  = &dataCache;

//Memory Stall
void MemoryStall(struct stage_reg_w *new_w_reg){
    new_w_reg->new_pc      = current_stage_w_register->new_pc;
    new_w_reg->instruction = current_stage_w_register->instruction;
    new_w_reg->opcode      = current_stage_w_register->opcode;
    new_w_reg->rdAddy      = current_stage_w_register->rdAddy;
    new_w_reg->rdVal       = current_stage_w_register->rdVal;
}

//Memory stage
extern void stage_memory (struct stage_reg_w *new_w_reg)
{
    //local variables
    uint8_t     opcode;
    uint8_t     funct3;
    uint64_t    rdAddy;
    uint64_t    rs1Addy;
    uint64_t    rs2Addy;
    uint64_t    imm;
    uint64_t    output;
    uint64_t    memVal;
    uint64_t    memStatusVal;
    uint64_t    bytes;
    uint64_t    address;
    Block*      returnBlock;
    bool        doneMem = 1;
    bool        prevMem = 0;
    bool        memStall;

    if(current_stage_m_register != NULL)
    {
        opcode  = current_stage_m_register->opcode;
        funct3  = current_stage_m_register->func3;
        rdAddy  = current_stage_m_register->rdAddy;
        rs1Addy = current_stage_m_register->rs1Addy;
        rs2Addy = current_stage_m_register->rs2Addy;
        imm     = current_stage_m_register->imm;
        output  = current_stage_m_register->output;
        address = current_stage_m_register->address;
        bytes   = current_stage_m_register->bytes;
        prevMem = current_stage_w_register->prevMem;
    }


    //Forwarding
    if (opcode == OP_LOAD || opcode == OP_STORE)
    {
        if(rs1Addy == current_stage_w_register->rdAddy)
            address = (uint64_t)( (int64_t)current_stage_w_register->rdVal + (int64_t)imm);
    }

    // if load or store then load or store necessary results
    //address is the ALU result from X
    if (opcode == OP_LOAD)
    {
        //what exactly is dCache
        returnBlock = CheckCache(dCache, address);
        if(returnBlock == NULL){
            doneMem = 0;
            if(prevMem == 0){
                doneMem = memory_read (address, &memVal, bytes);
                prevMem = 1;
            }else{
                doneMem = memory_status(address, &memStatusVal);
            }

            if(doneMem == true){
                memVal = memStatusVal;
                prevMem  = 0;
                CacheWrite(dCache, address, memVal);
            }

        } else {
            memVal = returnBlock->data;
        }

        switch (funct3)
            {
                case (FUNCT3_0):                                                //LB
                    //Sign extend
                    if ((memVal >> 7) IS_ONE){
                        memVal |= EXTEND08;
                    }
                    break;

                case (FUNCT3_1):                                                //LH
                    //Sign extend
                    if ((memVal >> 7) IS_ONE){
                        memVal |= EXTEND16;
                    }
                    break;

                case (FUNCT3_2):                                                //LW
                    //Sign extend
                    if ((memVal >> 7) IS_ONE){
                        memVal |= EXTEND32;
                    }
                    break;

                case (FUNCT3_3):                                                //LD
                    break;

                case (FUNCT3_4):                                                //LBU
                    break;

                case (FUNCT3_5):                                                //LHU
                    break;

                case (FUNCT3_6):                                                //LWU
                    break;
            }

        output = memVal;


    } else if (opcode == OP_STORE) {
        returnBlock = CheckCache(dCache, address);
        //if cache miss write around and go straight to mem
        if(returnBlock == NULL){
            doneMem = 0;
            if(prevMem == 0){
                doneMem = memory_write (address, output, bytes);
                prevMem = 1;
            } else {
                doneMem = memory_status(address, &memStatusVal);
            }

            if(doneMem == true){
                prevMem  = 0;
            }

        } else {
            if(prevMem == 0){
                doneMem = memory_write (address, output, bytes);
                prevMem = 1;
            } else{
                doneMem = memory_status(address, &memStatusVal);
            }
            if(doneMem == true){
                prevMem = 0;
                prevMem = 0;
                //need to make sure memStatusVal is what I want
                CacheWrite(dCache, address, memStatusVal);
                //dont stall
            }
        }
    }
    if(doneMem == true){
        MemoryStall(new_w_reg);
        new_w_reg->prevMem     = prevMem;
        new_w_reg->memStall    = 0;
    }else{
        MemoryStall(new_w_reg);
        new_w_reg->prevMem     = prevMem;
        new_w_reg->memStall    = 1;
    }
    //printf("Memory Out PC: %llx\n    rdAddy: %llx rdVal: %llx\n",
    //current_stage_m_register->new_pc, current_stage_m_register->rdAddy, current_stage_m_register->output);
}
