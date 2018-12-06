//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"
#include "macros.h"
#include <stdio.h>

//Decode stage
extern void stage_decode (struct stage_reg_x *new_x_reg)
{
    //local variables
    uint32_t    instrc;     //instruction
    uint8_t     opcode;     //opcode
    uint8_t     func3;      //func3 of instruction
    uint8_t     func7;      //func7 of instruction
    uint64_t    destin;     //destination register
    uint64_t    imm;        //immeadiate variable
    uint64_t    rs1Addy = 0;//Addess of first source register
    uint64_t    rs1Val;     //value in first source register
    uint64_t    rs2Addy = 0;//Addess of second source register
    uint64_t    rs2Val;     //value in second source register
    uint64_t    rdAddy;     //Address of destination register

    //pass instruction and opcode to this stage
    //if(current_stage_d_register != NULL){
        instrc = current_stage_d_register->instruction;
        opcode = current_stage_d_register->opcode;
    //}
    func3  = (uint8_t )( (instrc >> 12) & FUNCT3_MASK );
    func7  = (uint8_t )( (instrc >> 25) & FUNCT7_MASK );

    //Go through the six instruction formats to pull values
    if (opcode == OP_OP || opcode == OP_OP32)                                  //R-type
    {
        //Immediate not needed so its set to 0
        imm = 0;

        //Get values from registers
        rs1Addy = (instrc >> 15) & REGADD_MASK;
        rs2Addy = (instrc >> 20) & REGADD_MASK;
        register_read (rs1Addy, rs2Addy, &rs1Val, &rs2Val);
        //printf("rs1Addy: %llx Val: %llx\nrs2Addy: %llx Val: %llx\n", rs1Addy, rs1Val, rs2Addy, rs2Val);
        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );

    }

    else if (opcode == OP_JALR  || opcode == OP_LOAD                            //I-type
          || opcode == OP_OPIMM || opcode == OP_OPIMM32)
    {
        //Get values from registers
        rs1Addy = (instrc >> 15) & REGADD_MASK;
        rs2Addy = 0;
        register_read (rs1Addy, rs2Addy, &rs1Val, &rs2Val);

        //Bit shift and AND to get immediate
        imm = ((instrc >> 20) & 0b111111111111);

        //Sign extend immediate
        if ((imm >> 11) IS_ONE){
            imm |= EXTEND12;
        }

        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );
    }

    else if (opcode == OP_STORE)                                                //S-type
    {
        //Get values from registers
        rs1Addy = (instrc >> 15) & REGADD_MASK;
        rs2Addy = (instrc >> 20) & REGADD_MASK;
        register_read (rs1Addy, rs2Addy, &rs1Val, &rs2Val);

        //Get immediate, sign extended
        imm = (instrc >> 20) & 0xFFF;
        if ((imm >> 11) IS_ONE){
            imm |= EXTEND12;
        }

        //Register destination not needed so its set to 0
        rdAddy = 0;
    }

    else if (opcode == OP_BRANCH)                                               //B-type
    {
        //Bit shifts and ANDs to get immediate
        imm  =  ((instrc >> 31) & 0b000001) << 12;   //Gets imm[12]
        imm |=  ((instrc >>  7) & 0b000001) << 11;   //Gets imm[11]
        imm |=  ((instrc >> 25) & 0b111111) <<  5;   //Gets imm[10:5]
        imm |=  ((instrc >>  8) & 0b001111) <<  1;   //Gets imm[4:1]

        //Sign extend immediate
        if ((imm >> 11) IS_ONE){
            imm |= EXTEND12;
        }

        //Get values from registers
        rs1Addy = (instrc >> 15) & REGADD_MASK;
        rs2Addy = (instrc >> 20) & REGADD_MASK;
        register_read (rs1Addy, rs2Addy, &rs1Val, &rs2Val);


        //Register destination not needed so its set to 0
        rdAddy = 0;
    }

    else if (opcode == OP_LUI || opcode == OP_AUIPC)                            //U-type
    {
        //Get immediate from instruction
        imm = instrc & OP_LUI_MASK;

        //Sign extend
        if ((imm >> 31) IS_ONE){
            imm |= EXTEND32;
        }

        //rs1 and rs2 are not used so theyre set to 0
        rs1Val = 0;
        rs2Val = 0;

        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );
    }

    else if (opcode == OP_JAL)                                                  //J-type
    {
        //Bit shifts and ANDs to get immediate
        imm  =  ((instrc >> 31) & 0b0000000001) << 20; //Gets imm[20]
        imm |=  ((instrc >> 12) & 0b0011111111) << 12; //Gets imm[19:12]
        imm |=  ((instrc >> 20) & 0b0000000001) <<  5; //Gets imm[11]
        imm |=  ((instrc >> 21) & 0b1111111111) <<  1; //Gets imm[10:1]

        //rs1 and rs2 are not used so theyre set to 0
        rs1Val = 0;
        rs2Val = 0;

        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );
    }

    //Pass values onto new excute stage register
    if(current_stage_m_register->misprediction == 1){
        new_x_reg->new_pc       = 0;
        new_x_reg->instruction  = 0;
        new_x_reg->opcode       = 0;
        new_x_reg->func3        = 0;
        new_x_reg->func7        = 0;
        new_x_reg->rs1          = 0;
        new_x_reg->rs2          = 0;
        new_x_reg->rs1Addy      = 0;
        new_x_reg->rs2Addy      = 0;
        new_x_reg->imm          = 0;
        new_x_reg->rdAddy       = 0;
    } else if (current_stage_m_register->wasLd == 1){
        uint64_t    temp =  current_stage_m_register->rdAddy;
        if(current_stage_x_register->rs1Addy == temp|| current_stage_x_register->rs2Addy == temp){
            new_x_reg->new_pc       = current_stage_x_register->new_pc;
            new_x_reg->instruction  = current_stage_x_register->instruction;
            new_x_reg->opcode       = current_stage_x_register->opcode;
            new_x_reg->func3        = current_stage_x_register->func3;
            new_x_reg->func7        = current_stage_x_register->func7;
            new_x_reg->rs1          = current_stage_x_register->rs1;
            new_x_reg->rs2          = current_stage_x_register->rs2;
            new_x_reg->rs1Addy      = current_stage_x_register->rs1Addy;
            new_x_reg->rs2Addy      = current_stage_x_register->rs2Addy;
            new_x_reg->imm          = current_stage_x_register->imm;
            new_x_reg->rdAddy       = current_stage_x_register->rdAddy;
        }
    } else {
        new_x_reg->new_pc       = current_stage_d_register->new_pc;
        new_x_reg->instruction  = current_stage_d_register->instruction;
        new_x_reg->opcode       = current_stage_d_register->opcode;
        new_x_reg->func3        = func3;
        new_x_reg->func7        = func7;
        new_x_reg->rs1          = rs1Val;
        new_x_reg->rs2          = rs2Val;
        new_x_reg->rs1Addy      = rs1Addy;
        new_x_reg->rs2Addy      = rs2Addy;
        new_x_reg->imm          = imm;
        new_x_reg->rdAddy       = rdAddy;
    }
    /*printf("Decode out PC: %llx\n", current_stage_d_register->new_pc);
    printf("    rs1Addy: %llx Val: %llx rs2Addy: %llx Val: %llx imm: %llx\n", rs1Addy, rs1Val, rs2Addy, rs2Val, imm);*/
}

