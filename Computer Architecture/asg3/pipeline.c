/********************************************************************************
William O'Callahan, Eric Ventor, Ivan Espiritu
Assignement 2
********************************************************************************/

//Includes 
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"

//Includes
#include <stdio.h>
#include <stdlib.h>

//Mask macros 
#define OPCODE_MASK     0b01111111
#define REGADD_MASK     0b00011111
#define FUNCT3_MASK     0b00000111
#define FUNCT6_MASK     0b00111111
#define FUNCT7_MASK     0b01111111
#define OP_LUI_MASK     0xFFFFF000
#define SHAMT_MASK      0b00011111
#define BYTE_MASK       0x000000FF
#define HALFW_MASK      0x0000FFFF
#define WORD_MASK       0xFFFFFFFF

//Sign extenders
#define EXTEND08        0xFFFFFFFFFFFFFF00
#define EXTEND12        0xFFFFFFFFFFFFF000
#define EXTEND16        0xFFFFFFFFFFFF0000
#define EXTEND21        0xFFFFFFFFFFE00000
#define EXTEND32        0xFFFFFFFF00000000

//Opcode macros
#define OP_LUI          0b0110111
#define OP_AUIPC        0b0010111
#define OP_JAL          0b1101111 
#define OP_JALR         0b1100111 
#define OP_BRANCH       0b1100011 
#define OP_LOAD         0b0000011
#define OP_STORE        0b0100011 
#define OP_OPIMM        0b0010011
#define OP_OP           0b0110011
#define OP_OPIMM32      0b0011011 
#define OP_OP32         0b0111011

//funct3 macros
#define FUNCT3_0        0b000
#define FUNCT3_1        0b001
#define FUNCT3_2        0b010
#define FUNCT3_3        0b011
#define FUNCT3_4        0b100
#define FUNCT3_5        0b101
#define FUNCT3_6        0b110
#define FUNCT3_7        0b111

//funct7 macros
#define FUNCT7_0        0b0000000
#define FUNCT7_1        0b0000001
#define FUNCT7_6        0b0100000

//funct6 macros
#define FUNCT6_0        0b000000
#define FUNCT6_5        0b010000

//Other macros
#define PCINC           4
#define IS_ONE          & 0b1
#define LONG_MASK       0xFFFFFFFFFFFFFFFF
#define LONG_EXTE       (((__uint128_t)LONG_MASK) << 64)

//Fetch stage
extern void stage_fetch (struct stage_reg_d *new_d_reg)
{
    //variable declaration
    uint64_t    pc;         //current pc
    uint32_t    instrc;      //instruction to be executed
    uint8_t     opcode;     //opcode of instruction
    
    //Get pc and read in instruction
    pc = get_pc ();
    memory_read (pc, &instrc, 4);

    //Get opcode
    opcode = (uint8_t)( (instrc) & OPCODE_MASK );

    //BTB actions
    if ( opcode == OP_JAL || opcode == OP_JALR || opcode == OP_BRANCH ) {
        // go through the BTB in Fetch...aka do nothing now

        // target = btb[tag]
        // if tag exists, update target

        // instr = BTB[arrayCounterTag];
        // arrayCounterTag += 1;
    } else {
        pc += 4;
    }

    //push pc inst and opcode to next stage register
    new_d_reg->new_pc       = pc;
    new_d_reg->instruction  = instrc;
    new_d_reg->opcode       = opcode;

}

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
    uint64_t    rs1Addy;    //Addess of first source register
    uint64_t    *rs1Val;    //value in first source register
    uint64_t    rs2Addy;    //Addess of first source register
    uint64_t    *rs2Val;    //value in first source register
    uint64_t    rdAddy;     //Address of destination register

    //pass instruction and opcode to this stage
    instrc = current_stage_d_register->instruction;
    opcode = current_stage_d_register->opcode;
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
        register_read (rs1Addy, rs2Addy, rs1Val, rs1Val);

        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );

    }

    else if (opcode == OP_JALR || opcode == OP_LOAD || opcode == OP_OPIMM)     //I-type
    {
        //Bit shift and AND to get immediate
        imm = ((instrc >> 20) & 0b111111111111);    
        
        //Sign extend immediate
        if ((imm >> 11) IS_ONE){
            imm |= EXTEND12;
        }
        
        //rs1 and rs2 are not used so theyre set to 0
        *rs1Val = 0;
        *rs2Val = 0;

        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );
    }

    else if (opcode == OP_STORE)                                                //S-type
    {
        //Get values from registers
        rs1Addy = (instrc >> 15) & REGADD_MASK;
        rs2Addy = (instrc >> 20) & REGADD_MASK;
        register_read (rs1Addy, rs2Addy, rs1Val, rs1Val);

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
        register_read (rs1Addy, rs2Addy, rs1Val, rs1Val);

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
        *rs1Val = 0;
        *rs2Val = 0;

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
        *rs1Val = 0;
        *rs2Val = 0;

        //Set destination address
        rdAddy = (uint64_t)( (instrc >> 7) & REGADD_MASK );
    }

    //Pass values onto new excute stage register
    new_x_reg->new_pc       = current_stage_d_register->new_pc;
    new_x_reg->instruction  = current_stage_d_register->instruction;
    new_x_reg->func3        = func3;
    new_x_reg->func7        = func7;
    new_x_reg->rs1          = *rs1Val;
    new_x_reg->rs2          = *rs1Val;
    new_x_reg->imm          = imm;
    new_x_reg->rdAddy       = rdAddy;
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
    uint64_t    rs2Val;     //value in first source register
    uint64_t    rdAddy;     //Address of destination register
    uint64_t    output;     //output variable to be written to rd
    uint64_t    branch;     //branch pc to jump to if condition is met
    uint64_t    address;    //address to be read from address
    uint64_t    bytes;      //Bytes of memory to be read
    __int128_t  sint128_1;  //128 bit int for multiplication
    __int128_t  sint128_2;  //128 bit int for multiplication
    __uint128_t uint128_1;  //128 bit uint for multiplication
    __uint128_t uint128_2;  //128 bit uint for multiplication


    //pass instruction, imm, rs1, rs2, and opcode to this stage
    instrc = current_stage_x_register->instruction;
    pc     = current_stage_x_register->new_pc;
    opcode = current_stage_x_register->opcode;
    funct3 = current_stage_x_register->func3;
    funct7 = current_stage_x_register->func7;
    funct6 = funct7 >> 1;
    imm    = current_stage_x_register->imm;
    rs1Val = current_stage_x_register->rs1;
    rs2Val = current_stage_x_register->rs2;

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
            break;

        case (OP_JALR):                                                         //JALR
            //Store address following jump in destination register
            output = pc + PCINC;
            
            //Add signed immediate to value in rs1
            branch  = (uint64_t) ((int64_t)rs1Val + (int64_t)imm);
            
            //Set least significant bit to 0 and set new pc to branch
            branch &= 0xFFFFFFFFFFFFFFFE;
            break;

        case (OP_BRANCH):                                                       //RV32I Branch operations
            //Add immediate to pc to get branch
            branch = (uint64_t) ((int64_t)pc + (int64_t)imm);

            switch (funct3)
            {
                case (FUNCT3_0):                                                //BEQ
                    output = (( (int64_t)rs1Val == (int64_t)rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_1):                                                //BNE
                    output = (( (int64_t)rs1Val != (int64_t)rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_4):                                                //BLT
                    output = (( (int64_t)rs1Val <  (int64_t)rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_5):                                                //BGE
                    output = (( (int64_t)rs1Val >= (int64_t)rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_6):                                                //BLTU
                    output = (( rs1Val <= rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_7):                                                //BGEU
                    output = (( rs1Val  >  rs2Val) ? branch : pc + PCINC);
                    break; 
            }
            break;

        case (OP_LOAD):                                                         //RV32I Load operations

            address = (uint64_t)( (int64_t)rs1Val + (int64_t)imm );

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
                            output = (( (uint64_t)rs1Val < (uint64_t)rs2Val) ? 1 : 0 );
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
                            output = rs1Val >> rs2Val;
                            break;

                        case (FUNCT7_1):                                        //DIVU
                            //unsigned division
                            output = (uint64_t)( rs1Val / rs2Val );
                            break;

                        case (FUNCT7_6):                                        //SRA
                            //Get shift amount
                            rs2Val &= SHAMT_MASK;

                            //Shift right arithmetically
                            output = (uint64_t)( (int64_t)rs1Val >> rs2Val);
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

    //Pass values onto new memory stage register
    new_m_reg->new_pc       = current_stage_x_register->new_pc;
    new_m_reg->instruction  = current_stage_x_register->instruction;
    new_m_reg->rs1          = rs1Val;
    new_m_reg->rs2          = rs1Val;
    new_m_reg->imm          = imm;
    new_m_reg->rdAddy       = current_stage_x_register->rdAddy;
    new_m_reg->rdVal        = output;

}

//Memory stage
extern void stage_memory (struct stage_reg_w *new_w_reg)
{

}

//Writeback stage
extern void stage_writeback (void)
{

}