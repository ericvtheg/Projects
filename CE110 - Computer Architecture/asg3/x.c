//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"
#include "macros.h"
#include <stdio.h>

//Execute stall
void ExecuteStall(struct stage_reg_m *new_m_reg){
    new_m_reg->new_pc       = current_stage_m_register->new_pc;
    new_m_reg->instruction  = current_stage_m_register->instruction;
    new_m_reg->rs1          = current_stage_m_register->rs1;
    new_m_reg->rs2          = current_stage_m_register->rs2;
    new_m_reg->rs1Addy      = current_stage_m_register->rs1Addy;
    new_m_reg->rs2Addy      = current_stage_m_register->rs2Addy;
    new_m_reg->imm          = current_stage_m_register->imm;
    new_m_reg->rdAddy       = current_stage_m_register->rdAddy;
    new_m_reg->func3        = current_stage_m_register->func3;
    new_m_reg->opcode       = current_stage_m_register->opcode;
    new_m_reg->output       = current_stage_m_register->output;
    new_m_reg->address      = current_stage_m_register->address;
    new_m_reg->bytes        = current_stage_m_register->bytes;
}

//Execute stage
extern void stage_execute (struct stage_reg_m *new_m_reg)
{
    //local variables
    uint32_t    instrc;     //instruction to be executed
    uint64_t    pc;         //current pc
    uint8_t     opcode;     //opcode of instruction
    uint8_t     funct3;     //func3 of instruction
    uint8_t     funct6;     //func6 of instruction
    uint8_t     funct7;     //func7 of instruction
    uint64_t    imm;        //immeadiate variable
    uint64_t    rs1Val;     //value in first source register
    uint64_t    rs1Addy;
    uint64_t    rs2Val;     //value in first source register
    uint64_t    rs2Addy;
    uint64_t    rdAddy;     //Address of destination register
    uint64_t    output;     //output variable to be written to rd
    uint64_t    branch;     //branch pc to jump to if condition is met
    uint64_t    address;    //address to be read from address
    uint64_t    bytes;      //Bytes of memory to be read
    __int128_t  sint128_1;  //128 bit int for multiplication
    __int128_t  sint128_2;  //128 bit int for multiplication
    __uint128_t uint128_1;  //128 bit uint for multiplication
    __uint128_t uint128_2;  //128 bit uint for multiplication
    int         fiveBitAddy;


    //pass instruction, imm, rs1, rs2, and opcode to this stage
    if(current_stage_x_register != NULL){
        instrc = current_stage_x_register->instruction;
        pc     = current_stage_x_register->new_pc;
        opcode = current_stage_x_register->opcode;
        funct3 = current_stage_x_register->func3;
        funct7 = current_stage_x_register->func7;
        funct6 = funct7 >> 1;
        imm    = current_stage_x_register->imm;
        rs1Val = current_stage_x_register->rs1;
        rs2Val = current_stage_x_register->rs2;
        rs1Addy = current_stage_x_register->rs1Addy;
        rs2Addy = current_stage_x_register->rs2Addy;
        fiveBitAddy = instrc & 0b11111;
    }

    new_m_reg->misprediction = 0;
    new_m_reg->wasLd = 0;

    //Forwarding
    if (rs1Addy != 0 && rs1Addy == current_stage_m_register->rdAddy){
        rs1Val = current_stage_m_register->output;
    }
    if (rs2Addy != 0 && rs2Addy == current_stage_m_register->rdAddy){
        rs2Val = current_stage_m_register->output;
    }
    if (rs1Addy != 0 && rs1Addy == current_stage_w_register->rdAddy){
        rs1Val = current_stage_w_register->rdVal;
    }
    if (rs2Addy != 0 && rs2Addy == current_stage_w_register->rdAddy){
        rs2Val = current_stage_w_register->rdVal;
    }

    //Execute instruction based on distinct values of opcode, maybe funct3, and maybe funct6/7
    //Opcode->Funct3->Funct6/Funct7
    switch(opcode)
    {
        case (OP_LUI):                                                          //LUI
            output = imm;
            break;

        case (OP_AUIPC):                                                        //AUIPC
            //Add immediate to pc and store results
            output = (uint64_t)( (int64_t)imm + (int64_t)pc );
            break;

        case (OP_JAL):                                                          //JAL
            //Store address following jump in destination register
            output = pc + PCINC;

            //Add immediate to branch and set new pc to branch
            branch = (uint64_t)( (int64_t) pc + (int64_t) imm );

            BTB[fiveBitAddy][0] = pc;
            BTB[fiveBitAddy][1] = branch;

            set_pc(branch);
            break;

        case (OP_JALR):                                                         //JALR
            //Store address following jump in destination register
            output = pc + PCINC;

            //Add signed immediate to value in rs1
            branch  = (uint64_t) ((int64_t)rs1Val + (int64_t)imm);

            //Set least significant bit to 0 and set new pc to branch
            branch &= 0xFFFFFFFFFFFFFFFE;

            BTB[fiveBitAddy][0] = pc;
            BTB[fiveBitAddy][1] = branch;

            set_pc(branch);
            break;

        case (OP_BRANCH):                                                       //RV32I Branch operations
        //Add immediate to pc to get branch
        branch = (uint64_t) ((int64_t)pc + (int64_t)imm);

            // int fiveBitAddy = instrc & 0b11111;
            // storing into the BTB
            // if (backward branches we store, don't know actual yet)
            if ( pc > branch){
                BTB[fiveBitAddy][0] = pc;
                BTB[fiveBitAddy][1] = branch;
            }
            switch (funct3)
            {
                case (FUNCT3_0):                                                //BEQ
                    output = (( (int64_t)rs1Val == (int64_t)rs2Val) ? branch : pc + PCINC);
                    if ( output == branch &&  output != BTB[fiveBitAddy][1]){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;
                    } else if ( output == branch && output == BTB[fiveBitAddy][1] ) {

                        new_m_reg->misprediction = 0;

                    } else if ( output == pc + PCINC && output != BTB[fiveBitAddy][1] ){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;

                    } else if ( output == pc + PCINC){

                        new_m_reg->misprediction = 0;
                    }

                    break;

                case (FUNCT3_1):                                                //BNE
                    output = (( (int64_t)rs1Val != (int64_t)rs2Val) ? branch : pc + PCINC);
                    if ( output == branch &&  output != BTB[fiveBitAddy][1]){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;
                    } else if ( output == branch && output == BTB[fiveBitAddy][1] ) {

                        new_m_reg->misprediction = 0;

                    } else if ( output == pc + PCINC && output != BTB[fiveBitAddy][1] ){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;

                    } else if ( output == pc + PCINC){

                        new_m_reg->misprediction = 0;

                    }

                    break;

                case (FUNCT3_4):                                                //BLT
                    output = (( (int64_t)rs1Val <  (int64_t)rs2Val) ? branch : pc + PCINC);
                    if ( output == branch &&  output != BTB[fiveBitAddy][1]){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;
                    } else if ( output == branch && output == BTB[fiveBitAddy][1] ) {

                        new_m_reg->misprediction = 0;

                    } else if ( output == pc + PCINC && output != BTB[fiveBitAddy][1] ){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;

                    } else if ( output == pc + PCINC){

                        new_m_reg->misprediction = 0;

                    }

                    break;

                case (FUNCT3_5):                                                //BGE
                    output = (( (int64_t)rs1Val >= (int64_t)rs2Val) ? branch : pc + PCINC);
                    if ( output == branch &&  output != BTB[fiveBitAddy][1]){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;
                    } else if ( output == branch && output == BTB[fiveBitAddy][1] ) {

                        new_m_reg->misprediction = 0;

                    } else if ( output == pc + PCINC && output != BTB[fiveBitAddy][1] ){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;

                    } else if ( output == pc + PCINC){

                        new_m_reg->misprediction = 0;

                    }

                    break;

                case (FUNCT3_6):                                                //BLTU
                    output = (( rs1Val <= rs2Val) ? branch : pc + PCINC);
                    if ( output == branch &&  output != BTB[fiveBitAddy][1]){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;
                    } else if ( output == branch && output == BTB[fiveBitAddy][1] ) {

                        new_m_reg->misprediction = 0;

                    } else if ( output == pc + PCINC && output != BTB[fiveBitAddy][1] ){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;

                    } else if ( output == pc + PCINC){

                        new_m_reg->misprediction = 0;

                    }

                    break;

                case (FUNCT3_7):                                                //BGEU
                    output = (( rs1Val  >  rs2Val) ? branch : pc + PCINC);
                    if ( output == branch &&  output != BTB[fiveBitAddy][1]){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;
                    } else if ( output == branch && output == BTB[fiveBitAddy][1] ) {

                        new_m_reg->misprediction = 0;

                    } else if ( output == pc + PCINC && output != BTB[fiveBitAddy][1] ){

                        BTB[fiveBitAddy][0] = 0;
                        BTB[fiveBitAddy][1] = 0;

                        //flush

                        set_pc(output);
                        new_m_reg->misprediction = 1;

                    } else if ( output == pc + PCINC){

                        new_m_reg->misprediction = 0;

                    }

                    break;
            }
            break;

        case (OP_LOAD):                                                         //RV32I Load operations

            address = (uint64_t)( (int64_t)rs1Val + (int64_t)imm );
            new_m_reg->wasLd = 1;

            switch (funct3)//done
            {
                case (FUNCT3_0):                                                //LB
                    bytes = 1;
                    break;

                case (FUNCT3_1):                                                //LH
                    bytes = 2;
                    break;

                case (FUNCT3_2):                                                //LW
                    bytes = 4;
                    break;

                case (FUNCT3_3):                                                //LD
                    bytes = 8;
                    break;

                case (FUNCT3_4):                                                //LBU
                    bytes = 1;
                    break;

                case (FUNCT3_5):                                                //LHU
                    bytes = 2;
                    break;

                case (FUNCT3_6):                                                //LWU
                    bytes = 4;
                    break;
            }
            break;

        case (OP_STORE):                                                        //RV32I Store operations

            address = (uint64_t)( (int64_t)rs1Val + (int64_t)imm );
            output  = rs2Val;

            switch (funct3)
            {
                case (FUNCT3_0):                                                //SB
                    bytes = 1;
                    output &= BYTE_MASK;
                    break;

                case (FUNCT3_1):                                                //SH
                    bytes = 2;
                    output &= HALFW_MASK;
                    break;

                case (FUNCT3_2):                                                //SW
                    bytes = 4;
                    output &= WORD_MASK;
                    break;
                case (FUNCT3_3):                                                //SD                                          //SW
                    bytes = 8;
                    break;

            }
            break;

        case (OP_OPIMM):                                                        //RV32I Integer w/ immediate Operations

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADDI
                    //Signed addition of rs1 and immediate
                    output = (uint64_t) (( (int64_t)rs1Val + (int64_t)imm ));
                    break;

                case (FUNCT3_1):                                                //SLLI
                    //Get shift ammount from immediate
                    imm &= SHAMT_MASK;

                    //Shift rs1Val by immediate
                    output = rs1Val << imm;
                    break;

                case (FUNCT3_2):                                                //SLTI
                    //If signed rs1 is less than signed immediate, output 1, else output 0
                    output = (( (int64_t)rs1Val < (int64_t)imm) ? 1 : 0 );
                    break;

                case (FUNCT3_3):                                                //SLTIU
                    //If rs1 is less than immediate, output 1, else output 0
                    output = ((rs1Val < imm) ? 1 : 0 );
                    break;

                case (FUNCT3_4):                                                //XORI
                    //XOR rs1 and imm
                    output = rs1Val ^ imm;
                    break;

                case (FUNCT3_5):                                                //SRLI and SRAI
                    switch(funct6)
                    {
                        case (FUNCT6_0):                                        //SRLI
                            //Get shift ammount from immediate
                            imm &= SHAMT_MASK;

                            //Shift rs1Val by immediate
                            output = rs1Val >> imm;
                            break;

                        case (FUNCT6_5):                                        //SRAI
                            //Get shift ammount from immediate
                            imm &= SHAMT_MASK;

                            //Shift rs1Val arithmetically by immediate
                            output = (uint64_t)( (int64_t)rs1Val >> imm);
                            break;

                    }
                    break;

                case (FUNCT3_6):                                                //ORI
                    output = rs1Val | imm;
                    break;

                case (FUNCT3_7):                                                //ANDI
                    output = rs1Val & imm;
                    break;
            }
            break;

        case (OP_OP):                                                           //RV32I/M Integer Operations

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADD, MUL, and SUB

                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //ADD
                            //Signed addition of rs1 and rs2
                            output = (uint64_t)( (int64_t)rs1Val + (int64_t)rs2Val );
                            break;

                        case (FUNCT7_1):                                        //MUL
                            //Convert rs1 and rs2 vals to 128 bits
                            sint128_1 = (__int128_t)rs1Val;
                            sint128_2 = (__int128_t)rs2Val;

                            //Sign extend both vals
                            if ((sint128_1 >> 63) IS_ONE){
                                sint128_1 |= LONG_EXTE;
                            }
                            if ((sint128_2 >> 63) IS_ONE){
                                sint128_2 |= (LONG_EXTE);
                            }

                            //Low bits of signed multiplication of rs1 and rs2
                            output = (uint64_t)( (sint128_1 * sint128_2 & LONG_MASK) );
                            break;

                        case (FUNCT7_6):                                        //SUB
                            //Signed subtraction of rs1 and rs2
                            output = (uint64_t)( (int64_t)rs1Val - (int64_t)rs2Val );
                            break;

                    }
                    break;

                case (FUNCT3_1):                                                //SLL and MULH
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SLL
                            //Get shift amount
                            rs2Val &= SHAMT_MASK;

                            //Shift rs1 by lower 5 bits of rs2
                            output = rs1Val << rs2Val;
                            break;

                        case (FUNCT7_1):                                        //MULH
                            //Convert rs1 and rs2 vals to 128 bits
                            sint128_1 = (__int128_t)rs1Val;
                            sint128_2 = (__int128_t)rs2Val;

                            //Sign extend both vals
                            if ((sint128_1 >> 63) IS_ONE){
                                sint128_1 |= LONG_EXTE;
                            }
                            if ((sint128_2 >> 63) IS_ONE){
                                sint128_2 |= LONG_EXTE;
                            }

                            //High bits of signed multiplication of rs1 and rs2
                            output = (uint64_t)( (sint128_1 * sint128_2) >> 64);
                            break;
                    }
                    break;

                case (FUNCT3_2):                                                //SLT and MULHSU
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SLT
                            //If signed rs1 is less than signed rs2, output 1, else output 0
                            output = (( (int64_t)rs1Val < (int64_t)rs2Val) ? 1 : 0 );
                            break;

                        case (FUNCT7_1):                                        //MULHSU NOT DONE
                            //High bits of signed/unsigned multiplication of rs1 and rs2
                            //Convert rs1 and rs2 vals to 128 bits
                            sint128_1 = (__int128_t) rs1Val;
                            uint128_2 = (__uint128_t)rs2Val;

                            //Sign extend rs1
                            if ((sint128_1 >> 63) IS_ONE){
                                sint128_1 |= LONG_EXTE;
                            }

                            //High bits of signed multiplication of rs1 and rs2
                            output = (uint64_t)( (sint128_1 * uint128_2) >> 64);
                            break;
                    }
                    break;

                case (FUNCT3_3):                                                //SLTU and MULHU
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SLTU
                            //If rs1 is less than immediate, output 1, else output 0
                            output = ((rs1Val < rs2Val) ? 1 : 0 );
                            break;

                        case (FUNCT7_1):                                        //MULHU
                            //High bits of unsigned multiplication of rs1 and rs2
                            //Convert rs1 and rs2 vals to 128 bits
                            uint128_1 = (__uint128_t)rs1Val;
                            uint128_2 = (__uint128_t)rs2Val;

                            //High bits of signed multiplication of rs1 and rs2
                            output = (uint64_t)( (uint128_1 * uint128_2) >> 64);
                            break;
                    }
                    break;
                case (FUNCT3_4):                                                //XOR and DIV
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //XOR
                            //Exclusive OR of rs1 and rs2
                            output = rs1Val ^ rs2Val;
                            break;

                        case (FUNCT7_1):                                        //DIV
                            //Signed rs1 / rs2
                            output = (uint64_t)( (int64_t)rs1Val / (int64_t)rs2Val );
                            break;
                    }
                    break;
                case (FUNCT3_5):                                                //SRL DIVU and SRA
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SRL
                            //Get shift amount
                            rs2Val &= SHAMT_MASK;

                            //Shift rs1 right by rs2
                            output = rs1Val << rs2Val;
                            break;

                        case (FUNCT7_1):                                        //DIVU
                            //unsigned division
                            output = (uint64_t)( rs1Val / rs2Val );
                            break;

                        case (FUNCT7_6):                                        //SRA
                            //Get shift amount
                            rs2Val &= SHAMT_MASK;

                            //Shift right arithmetically
                            output = (uint64_t)( ((int64_t)rs1Val) >> rs2Val);
                            break;
                    }
                    break;
                case (FUNCT3_6):                                                //OR and REM
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //OR
                            //OR of rs1 and rs2
                            output = rs1Val | rs2Val;
                            break;

                        case (FUNCT7_1):                                        //REM
                            //Remainder of signed rs1 / rs2
                            output = (uint64_t)( (int64_t)rs1Val % (int64_t)rs2Val );
                            break;
                    }
                    break;
                case (FUNCT3_7):                                                //AND and REMU
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //AND
                            //And of rs1 and rs2
                            output = rs1Val & rs2Val;
                            break;

                        case (FUNCT7_1):                                        //REMU
                            //Remainder of unsigned rs1 / rs2
                            output = rs1Val % rs2Val;
                            break;
                    }
                    break;
            }

            break;

        case (OP_OPIMM32):                                                      //RV64I Integer w/ immediate Operations

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADDIW
                    //Signed addition of rs1 and immediate
                    output = (uint64_t) (( (int64_t)rs1Val + (int64_t)imm ));

                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_1):                                                //SLLIW
                    //Get shift ammount from immediate
                    imm &= SHAMT_MASK;

                    //Shift rs1Val by immediate
                    output = rs1Val << imm;

                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_5):                                                //SRLIW and SRAIW
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SRLIW
                            //Get shift ammount from immediate
                            imm &= SHAMT_MASK;

                            //Shift rs1Val by immediate
                            output = rs1Val >> imm;

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_6):                                        //SRAIW
                            //Get shift ammount from immediate
                            imm &= SHAMT_MASK;

                            //Shift rs1Val arithmetically by immediate
                            output = (uint64_t)( (int64_t)rs1Val >> imm );

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;
                    }
                    break;
            }
            break;

        case (OP_OP32):                                                         //RV64I/M Integer Operations

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADDW MULW and SUBW
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //ADDW
                            //Shorten rs1 and rs2 to words
                            rs1Val &= WORD_MASK;
                            rs2Val &= WORD_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( (int32_t)rs1Val + (int32_t)rs2Val ));

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_1):                                        //MULW
                            //Shorten rs1 and rs2 to words
                            rs1Val &= WORD_MASK;
                            rs2Val &= WORD_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( (int32_t)rs1Val * (int32_t)rs2Val ));

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_6):                                        //SUBW
                            //Shorten rs1 and rs2 to words
                            rs1Val &= WORD_MASK;
                            rs2Val &= WORD_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( (int32_t)rs1Val - (int32_t)rs2Val ));

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;
                    }
                    break;

                case (FUNCT3_1):                                                //SLLW
                    //Shorten rs1 to a word and get shift amount
                    rs1Val &= WORD_MASK;
                    rs2Val &= SHAMT_MASK;

                    //Signed addition of rs1 and rs2
                    output = (uint64_t) (( rs1Val << rs2Val ));

                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                       output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_4):                                                //DIVW
                    //Shorten rs1and rs2 to words
                    rs1Val &= WORD_MASK;
                    rs2Val &= WORD_MASK;

                    //Unsigned division of rs1 and rs2
                    output = (uint64_t) (( (int64_t)rs1Val / (int64_t)rs2Val ));

                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_5):                                                //SRLW DIVUW and SRAW
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SRLW
                            //Shorten rs1 to a word and get shift amount
                            rs1Val &= WORD_MASK;
                            rs2Val &= SHAMT_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( rs1Val >> rs2Val ));

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_1):                                        //DIVUW
                            //Shorten rs1and rs2 to words
                            rs1Val &= WORD_MASK;
                            rs2Val &= WORD_MASK;

                            //Unsigned division of rs1 and rs2
                            output = (uint64_t) (( rs1Val / rs2Val ));

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_6):                                        //SRAW
                            //Shorten rs1 to a word and get shift amount
                            rs1Val &= WORD_MASK;
                            rs2Val &= SHAMT_MASK;

                            //Shift rs1 by rs2
                            output = (uint64_t) (( (int32_t)rs1Val >> rs2Val ));

                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;
                    }
                    break;

                case (FUNCT3_6):                                                //REMW
                    //Shorten rs1and rs2 to words
                    rs1Val &= WORD_MASK;
                    rs2Val &= WORD_MASK;

                    //Signed addition of rs1 and rs2
                    output = (uint64_t) (( (int32_t)rs1Val % (int32_t)rs2Val ));

                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_7):                                                //REMWU
                    //Shorten rs1and rs2 to words
                    rs1Val &= WORD_MASK;
                    rs2Val &= WORD_MASK;

                    //Signed addition of rs1 and rs2
                    output = rs1Val % rs2Val;

                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;
            }

    }
    //printf("Output: %llx\nPC: %llx\n", output, current_stage_x_register->new_pc);
    //printf("RS1: %llx\nRS2: %llx\n", rs1Val, rs2Val);
    //Pass values onto new memory stage register
    new_m_reg->new_pc       = current_stage_x_register->new_pc;
    new_m_reg->instruction  = current_stage_x_register->instruction;
    new_m_reg->rs1          = rs1Val;
    new_m_reg->rs2          = rs2Val;
    new_m_reg->rs1Addy      = rs1Addy;
    new_m_reg->rs2Addy      = rs2Addy;
    new_m_reg->imm          = imm;
    new_m_reg->rdAddy       = current_stage_x_register->rdAddy;
    new_m_reg->func3        = funct3;
    new_m_reg->opcode       = opcode;
    new_m_reg->output       = output;
    new_m_reg->address      = address;
    new_m_reg->bytes        = bytes;

    uint64_t    temp =  current_stage_m_register->rdAddy;
    if(current_stage_w_register->memStall == 1){
        ExecuteStall(new_m_reg);
    } else if (current_stage_m_register->wasLd == 1 && (rs1Addy == temp || rs2Addy == temp)){
        //if ld command and rsAddy are equal then stall
        ExecuteStall(new_m_reg);
        //this was zero'd out but i think that was type...woop
        /*new_m_reg->new_pc       = 0;
        new_m_reg->instruction  = 0;
        new_m_reg->rs1          = 0;
        new_m_reg->rs2          = 0;
        new_m_reg->rs1Addy      = 0;
        new_m_reg->rs2Addy      = 0;
        new_m_reg->imm          = 0;
        new_m_reg->rdAddy       = 0;
        new_m_reg->func3        = 0;
        new_m_reg->opcode       = 0;
        new_m_reg->output       = 0;
        new_m_reg->address      = 0;
        new_m_reg->bytes        = 0;*/
    }
    //printf("Execute out PC: %llx\n  Output: %llx RdAddy: %llx RS1: %llx RS2:
    //%llx\n", current_stage_x_register->new_pc, output, current_stage_x_register->rdAddy, rs1Val, rs2Val);
    //printf("   opcode: %x func3: %x func7: %x\n", opcode, funct3, funct7);
}
