/********************************************************************************
William O'Callahan, Eric Ventor, Ivan Espiritu
Assignment 1

Provided functions:
    bool memory_read (uint64_t address, uint64_t *value, uint64_t size_in_bytes);
    bool memory_write (uint64_t address, uint64_t value, uint64_t size_in_bytes);
    bool memory_status (uint64_t address, uint64_t *value);

    void register_read (uint64_t register_a, uint64_t register_b, uint64_t * value_a, uint64_t * value_b);
    void register_write (uint64_t register_d, uint64_t value_d);

********************************************************************************/
//Includes
#include "riscv_sim_framework.h"
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

void execute_single_instruction(const uint64_t pc, uint64_t *new_pc)
{
    //Declare variables
    uint32_t    *instr;     //instruction to be executed
    uint8_t     opcode;     //opcode of instruction
    uint64_t    destin;     //destination register
    uint8_t     funct3;     //used to hold function3 value if needed
    uint8_t     funct6;     //used to hold function6 value if needed
    uint8_t     funct7;     //used to hold function7 value if needed
    uint64_t    output;     //output variable
    uint64_t    branch;     //branch pc to jump to if condition is met
    uint64_t    imm;        //immeadiate variable
    uint64_t    rs1Addy;    //Addess of first source register
    uint64_t    *rs1Val;    //value in first cource register
    uint64_t    rs2Addy;    //Addess of first source register
    uint64_t    *rs2Val;    //value in first cource register
    uint64_t    address;    //memory address
    uint64_t    *value;     //memory value
    uint64_t    bytes;      //byte size of output to memory
    __int128_t  sint128_1;  //128 bit int for multiplication
    __int128_t  sint128_2;  //128 bit int for multiplication
    __uint128_t uint128_1;  //128 bit uint for multiplication
    __uint128_t uint128_2;  //128 bit uint for multiplication

    //Allocate memory for pointers
    instr  = malloc(sizeof(uint64_t));
    rs1Val = malloc(sizeof(uint64_t));
    rs2Val = malloc(sizeof(uint64_t));
    value  = malloc(sizeof(uint64_t));

    //Get instruction from pc
    memory_read (pc, instr, 4);

    //Get opcode, funct3, funct6, funct7, and dest from instruction
    opcode = (uint8_t )( (*instr)       & OPCODE_MASK );
    destin = (uint64_t)( (*instr >>  7) & REGADD_MASK );
    funct3 = (uint8_t )( (*instr >> 12) & FUNCT3_MASK );
    funct6 = (uint8_t )( (*instr >> 26) & FUNCT6_MASK );
    funct7 = (uint8_t )( (*instr >> 25) & FUNCT7_MASK );

    //Execute instruction based on distinct values of opcode, maybe funct3, and maybe funct6/7
    //Opcode->Funct3->Funct6/Funct7
    switch(opcode)
    {
        case (OP_LUI):                                                          //LUI
            //Get immediate from instruction
            output = *instr & OP_LUI_MASK;
            
            //Sign extend 
            if ((output >> 31) IS_ONE){ 
                output |= EXTEND32;        
            } 

            //Store results
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;

        case (OP_AUIPC):                                                        //AUIPC
            //Get immediate from instruction
            imm = *instr & OP_LUI_MASK;
            
            //Sign extend 
            if ((imm >> 31) IS_ONE){ 
                imm |= EXTEND32;       
            } 

            //Add immediate to pc and store results
            output = (uint64_t)( (int64_t)imm + (int64_t)pc );
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;

        case (OP_JAL):                                                          //JAL
            //Store address following jump in destination register
            output = pc + PCINC;
            register_write (destin, output);

            //Bit shifts and ANDs to get immediate
            imm  =  ((*instr >> 31) & 0b0000000001) << 20; //Gets imm[20]
            imm |=  ((*instr >> 12) & 0b0011111111) << 12; //Gets imm[19:12]
            imm |=  ((*instr >> 20) & 0b0000000001) <<  5; //Gets imm[11]
            imm |=  ((*instr >> 21) & 0b1111111111) <<  1; //Gets imm[10:1]

            //Sign extend immediate
            if ((imm >> 20) IS_ONE){
                imm |= EXTEND21;
            }

            //Add immediate to branch and set new pc to branch
            branch = (uint64_t)( (int64_t) pc + (int64_t) imm );
            *new_pc = branch;
            break;

        case (OP_JALR):                                                         //JALR
            //Store address following jump in destination register
            output = pc + PCINC;
            register_write (destin, output);

            //Bit shift and AND to get immediate
            imm = ((*instr >> 20) & 0b111111111111);

            //Sign extend immediate
            if ((imm >> 11) IS_ONE){
                imm |= EXTEND12;
            }

            //Get value from register
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            register_read (rs1Addy, 0, rs1Val, rs1Val);

            //Add signed immediate to value in rs1
            branch  = (uint64_t) ((int64_t)*rs1Val + (int64_t)imm);
            
            //Set least significant bit to 0 and set new pc to branch
            branch &= 0xFFFFFFFFFFFFFFFE;
            *new_pc = branch;
            break;

        case (OP_BRANCH):                                                       //RV32I Branch operations

            //Bit shifts and ANDs to get immediate
            imm  =  ((*instr >> 31) & 0b000001) << 12;   //Gets imm[12]
            imm |=  ((*instr >>  7) & 0b000001) << 11;   //Gets imm[11]
            imm |=  ((*instr >> 25) & 0b111111) <<  5;   //Gets imm[10:5]
            imm |=  ((*instr >>  8) & 0b001111) <<  1;   //Gets imm[4:1]
            
            //Sign extend immediate
            if ((imm >> 11) IS_ONE){
                imm |= EXTEND12;
            }

            //Add immediate to pc to get branch
            branch = (uint64_t) ((int64_t)pc + (int64_t)imm);

            //Get values from registers
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            rs2Addy = (*instr >> 20) & REGADD_MASK;
            register_read (rs1Addy, rs2Addy, rs1Val, rs2Val);

            switch (funct3)
            {
                case (FUNCT3_0):                                                //BEQ
                    *new_pc = (( (int64_t)*rs1Val == (int64_t)*rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_1):                                                //BNE
                    *new_pc = (( (int64_t)*rs1Val != (int64_t)*rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_4):                                                //BLT
                    *new_pc = (( (int64_t)*rs1Val <  (int64_t)*rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_5):                                                //BGE
                    *new_pc = (( (int64_t)*rs1Val >= (int64_t)*rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_6):                                                //BLTU
                    *new_pc = (( *rs1Val <= *rs2Val) ? branch : pc + PCINC);
                    break;

                case (FUNCT3_7):                                                //BGEU
                    *new_pc = (( *rs1Val  >  *rs2Val) ? branch : pc + PCINC);
                    break; 
            }
            break;

        case (OP_LOAD):                                                         //RV32I Load operations

            //Get values from registers
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            rs2Addy = (*instr >> 20) & REGADD_MASK;
            register_read (rs1Addy, rs2Addy, rs1Val, rs1Val);

            //Get address from immediate, sign extended
            imm = (*instr >> 20) & 0xFFF;
            if ((imm >> 11) IS_ONE){ 
                imm |= EXTEND12;         
            }
            address = (uint64_t)( (int64_t)*rs1Val + (int64_t)imm );

            switch (funct3)//done
            {
                case (FUNCT3_0):                                                //LB
                    memory_read (address, value, 1);
                    output = *value;

                    //Sign extend  
                    if ((output >> 7) IS_ONE){ 
                        output |= EXTEND08;       
                    } 
                    break;

                case (FUNCT3_1):                                                //LH
                    memory_read (address, value, 2);
                    output = *value;

                    //Sign extend  
                    if ((output >> 15) IS_ONE){ 
                        output |= EXTEND16;          
                    } 
                    break;

                case (FUNCT3_2):                                                //LW
                    memory_read (address, value, 4);
                    output = *value;

                    //Sign extend  
                    if ((output >> 31) IS_ONE){ 
                        output |= EXTEND32;       
                    } 
                    break;

                case (FUNCT3_3):                                                //LD
                    memory_read (address, value, 8);
                    output = *value;
                    break;

                case (FUNCT3_4):                                                //LBU
                    memory_read (address, value, 1);
                    output = *value; 
                    break;

                case (FUNCT3_5):                                                //LHU
                    memory_read (address, value, 2);
                    output = *value; 
                    break; 
                    
                case (FUNCT3_6):                                                //LWU
                    memory_read (address, value, 4);
                    output = *value; 
                    break;
            }

            //Increment pc and write output
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;

        case (OP_STORE):                                                        //RV32I Store operations
            
            //Get values from registers
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            rs2Addy = (*instr >> 20) & REGADD_MASK;
            register_read (rs1Addy, rs2Addy, rs1Val, rs1Val);

            //Get address from immediate, sign extended
            imm = (*instr >> 20) & 0xFFF;
            if ((imm >> 11) IS_ONE){ 
                imm |= EXTEND12;   //Sign extend         
            }
            address = (uint64_t)( (int64_t)*rs1Val + (int64_t)imm );
            output  = *rs2Val;

            switch (funct3)//maybe done
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

            }
            
            //Increment pc and write to memory
            memory_write (address, output, bytes);
            *new_pc = pc + PCINC;
            break;

        case (OP_OPIMM):                                                        //RV32I Integer w/ immediate Operations

            //Get values from register and immediate
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            register_read (rs1Addy, 0, rs1Val, rs2Val);
            imm = (*instr >> 20) & 0b111111111111;
            
            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADDI
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }
                    
                    //Signed addition of rs1 and immediate
                    output = (uint64_t) (( (int64_t)*rs1Val + (int64_t)imm ));
                    break;

                case (FUNCT3_1):                                                //SLLI 
                    //Get shift ammount from immediate
                    imm &= SHAMT_MASK;

                    //Shift rs1Val by immediate
                    output = *rs1Val << imm;
                    break;

                case (FUNCT3_2):                                                //SLTI
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }
                    
                    //If signed rs1 is less than signed immediate, output 1, else output 0
                    output = (( (int64_t)*rs1Val < (int64_t)imm) ? 1 : 0 );
                    break;

                case (FUNCT3_3):                                                //SLTIU
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }

                    //If rs1 is less than immediate, output 1, else output 0
                    output = ((*rs1Val < imm) ? 1 : 0 );
                    break;

                case (FUNCT3_4):                                                //XORI
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }

                    //XOR rs1 and imm
                    output = *rs1Val ^ imm;
                    break;

                case (FUNCT3_5):                                                //SRLI and SRAI
                    switch(funct6)
                    {
                        case (FUNCT6_0):                                        //SRLI
                            //Get shift ammount from immediate
                            imm &= SHAMT_MASK;

                            //Shift rs1Val by immediate
                            output = *rs1Val >> imm;
                            break;

                        case (FUNCT6_5):                                        //SRAI
                            //Get shift ammount from immediate
                            imm &= SHAMT_MASK;

                            //Shift rs1Val arithmetically by immediate
                            output = (uint64_t)( (int64_t)*rs1Val >> imm);
                            break;

                    }
                    break; 

                case (FUNCT3_6):                                                //ORI
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }

                    output = *rs1Val | imm;
                    break;

                case (FUNCT3_7):                                                //ANDI
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }

                    output = *rs1Val & imm;
                    break;
            }

            //Increment pc and write output
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;

        case (OP_OP):                                                           //RV32I/M Integer Operations NOT DONE

            //Get values from registers
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            rs2Addy = (*instr >> 20) & REGADD_MASK;
            register_read (rs1Addy, rs2Addy, rs1Val, rs2Val);

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADD, MUL, and SUB
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //ADD
                            //Signed addition of rs1 and rs2
                            output = (uint64_t)( (int64_t)*rs1Val + (int64_t)*rs2Val );
                            break;

                        case (FUNCT7_1):                                        //MUL 
                            //Convert rs1 and rs2 vals to 128 bits
                            sint128_1 = (__int128_t)*rs1Val;
                            sint128_2 = (__int128_t)*rs2Val;

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
                            output = (uint64_t)( (int64_t)*rs1Val - (int64_t)*rs2Val );
                            break;

                    }
                    break;

                case (FUNCT3_1):                                                //SLL and MULH
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SLL
                            //Get shift amount
                            *rs2Val &= SHAMT_MASK;

                            //Shift rs1 by lower 5 bits of rs2
                            output = *rs1Val << *rs2Val;
                            break;

                        case (FUNCT7_1):                                        //MULH
                            //Convert rs1 and rs2 vals to 128 bits
                            sint128_1 = (__int128_t)*rs1Val;
                            sint128_2 = (__int128_t)*rs2Val;

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
                            output = (( (uint64_t)*rs1Val < (uint64_t)*rs2Val) ? 1 : 0 );
                            break;

                        case (FUNCT7_1):                                        //MULHSU NOT DONE
                            //High bits of signed/unsigned multiplication of rs1 and rs2
                            //Convert rs1 and rs2 vals to 128 bits
                            sint128_1 = (__int128_t) *rs1Val;
                            uint128_2 = (__uint128_t)*rs2Val;

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
                            output = ((*rs1Val < *rs2Val) ? 1 : 0 );
                            break;

                        case (FUNCT7_1):                                        //MULHU
                            //High bits of unsigned multiplication of rs1 and rs2
                            //Convert rs1 and rs2 vals to 128 bits
                            uint128_1 = (__uint128_t)*rs1Val;
                            uint128_2 = (__uint128_t)*rs2Val;

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
                            output = *rs1Val ^ *rs2Val;
                            break;

                        case (FUNCT7_1):                                        //DIV
                            //Signed rs1 / rs2
                            output = (uint64_t)( (int64_t)*rs1Val / (int64_t)*rs2Val );
                            break;
                    }
                    break;
                case (FUNCT3_5):                                                //SRL DIVU and SRA
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //SRL
                            //Get shift amount
                            *rs2Val &= SHAMT_MASK;
                            
                            //Shift rs1 right by rs2
                            output = *rs1Val >> *rs2Val;
                            break;

                        case (FUNCT7_1):                                        //DIVU
                            //unsigned division
                            output = (uint64_t)( *rs1Val / *rs2Val );
                            break;

                        case (FUNCT7_6):                                        //SRA
                            //Get shift amount
                            *rs2Val &= SHAMT_MASK;

                            //Shift right arithmetically
                            output = (uint64_t)( (int64_t)*rs1Val >> *rs2Val);
                            break;
                    }
                    break; 
                case (FUNCT3_6):                                                //OR and REM
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //OR
                            //OR of rs1 and rs2
                            output = *rs1Val | *rs2Val;
                            break;

                        case (FUNCT7_1):                                        //REM
                            //Remainder of signed rs1 / rs2
                            output = (uint64_t)( (int64_t)*rs1Val % (int64_t)*rs2Val );
                            break;
                    }
                    break;
                case (FUNCT3_7):                                                //AND and REMU
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //AND
                            //And of rs1 and rs2
                            output = *rs1Val & *rs2Val;
                            break;

                        case (FUNCT7_1):                                        //REMU
                            //Remainder of unsigned rs1 / rs2
                            output = *rs1Val % *rs2Val;
                            break;
                    }
                    break;
            }
            
            //Increment pc and write output
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;

        case (OP_OPIMM32):                                                      //RV64I Integer w/ immediate Operations

            //Get values from registers and immeadiate
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            register_read (rs1Addy, 0, rs1Val, rs2Val);
            imm = (*instr >> 20) & 0b111111111111;

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADDIW
                    //Sign extend immediate
                    if ((imm >> 11) IS_ONE){
                        imm |= EXTEND12;
                    }
                    
                    //Signed addition of rs1 and immediate
                    output = (uint64_t) (( (int64_t)*rs1Val + (int64_t)imm ));
                    
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
                    output = *rs1Val << imm;

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
                            output = *rs1Val >> imm;

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
                            output = (uint64_t)( (int64_t)*rs1Val >> imm );

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
            //Increment pc and write output
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;

        case (OP_OP32):                                                         //RV64I/M Integer Operations

            //Get values from registers
            rs1Addy = (*instr >> 15) & REGADD_MASK;
            rs2Addy = (*instr >> 20) & REGADD_MASK;
            register_read (rs1Addy, rs2Addy, rs1Val, rs2Val);

            switch (funct3)
            {
                case (FUNCT3_0):                                                //ADDW MULW and SUBW
                    switch(funct7)
                    {
                        case (FUNCT7_0):                                        //ADDW
                            //Shorten rs1 and rs2 to words
                            *rs1Val &= WORD_MASK;
                            *rs2Val &= WORD_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( (int32_t)*rs1Val + (int32_t)*rs2Val ));
                    
                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_1):                                        //MULW
                            //Shorten rs1 and rs2 to words
                            *rs1Val &= WORD_MASK;
                            *rs2Val &= WORD_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( (int32_t)*rs1Val * (int32_t)*rs2Val ));
                    
                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_6):                                        //SUBW
                            //Shorten rs1 and rs2 to words
                            *rs1Val &= WORD_MASK;
                            *rs2Val &= WORD_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( (int32_t)*rs1Val - (int32_t)*rs2Val ));
                    
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
                    *rs1Val &= WORD_MASK;
                    *rs2Val &= SHAMT_MASK;

                    //Signed addition of rs1 and rs2
                    output = (uint64_t) (( *rs1Val << *rs2Val ));
                    
                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                       output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_4):                                                //DIVW
                    //Shorten rs1and rs2 to words 
                    *rs1Val &= WORD_MASK;
                    *rs2Val &= WORD_MASK;

                    //Unsigned division of rs1 and rs2
                    output = (uint64_t) (( (int64_t)*rs1Val / (int64_t)*rs2Val ));
                    
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
                            *rs1Val &= WORD_MASK;
                            *rs2Val &= SHAMT_MASK;

                            //Signed addition of rs1 and rs2
                            output = (uint64_t) (( *rs1Val >> *rs2Val ));
                    
                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_1):                                        //DIVUW
                            //Shorten rs1and rs2 to words 
                            *rs1Val &= WORD_MASK;
                            *rs2Val &= WORD_MASK;

                            //Unsigned division of rs1 and rs2
                            output = (uint64_t) (( *rs1Val / *rs2Val ));
                    
                            //Shorten to be a word
                            output &= WORD_MASK;

                            //Sign extend to 64 bits
                            if ((output >> 31) IS_ONE){
                                output |= EXTEND32;
                            }
                            break;

                        case (FUNCT7_6):                                        //SRAW
                            //Shorten rs1 to a word and get shift amount
                            *rs1Val &= WORD_MASK;
                            *rs2Val &= SHAMT_MASK;

                            //Shift rs1 by rs2
                            output = (uint64_t) (( (int32_t)*rs1Val >> *rs2Val ));
                    
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
                    *rs1Val &= WORD_MASK;
                    *rs2Val &= WORD_MASK;

                    //Signed addition of rs1 and rs2
                    output = (uint64_t) (( (int32_t)*rs1Val % (int32_t)*rs2Val ));
                    
                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;

                case (FUNCT3_7):                                                //REMWU
                    //Shorten rs1and rs2 to words 
                    *rs1Val &= WORD_MASK;
                    *rs2Val &= WORD_MASK;

                    //Signed addition of rs1 and rs2
                    output = *rs1Val % *rs2Val;
                    
                    //Shorten to be a word
                    output &= WORD_MASK;

                    //Sign extend to 64 bits
                    if ((output >> 31) IS_ONE){
                        output |= EXTEND32;
                    }
                    break;
            }

            //Increment pc and write output
            register_write (destin, output);
            *new_pc = pc + PCINC;
            break;
    }

    //Free pointers
    free(instr);
    free(rs1Val);
    free(rs2Val);
    free(value);
}