/********************************************************************************
William O'Callahan, Eric Ventor, Ivan Espiritu
Assignment 2
********************************************************************************/

//Includes
#include "riscv_pipeline_registers.h"
#include "riscv_sim_pipeline_framework.h"

//Make sure all the variables below are constant, read only, and never written to!

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


// -------------------Assignment 2 declarations----------------

// Branch Target Buffer
uint64_t BTB[31][1];  

// example 
// BTB[21][0] = tag
// BTB[21][1] = target


// Branch History Buffer (Extra Credit)
// int BHT[32]

//Fetch stage
extern void stage_fetch (struct stage_reg_d *new_d_reg)
{

    // variable declaration
    uint8_t     opcode;         //opcode of instruction
    uint64_t pc = get_pc ();

    instr  = malloc(sizeof(uint64_t));
    memory_read (pc, instr, 4);

    // #define OP_JAL          0b1101111
    // #define OP_JALR         0b1100111
    // #define OP_BRANCH       0b1100011

    opcode = (uint8_t )( (*instr) & OPCODE_MASK );

    int tag = 0;

    if ( opcode == OP_JAL || opcode == OP_JALR || opcode == OP_BRANCH ) {
        // go through the BTB in Fetch...aka do nothing now

        // target = btb[tag]
        // if tag exists, update target

        // instr = BTB[arrayCounterTag];
        // arrayCounterTag += 1;
    } else {
        pc += 4;
    }

    *new_d_reg.pc     =     pc;
    *new_d_reg.instr  =  instr;
    *new_d_reg.opcode = opcode;

}

//Decode stage
extern void stage_decode (struct stage_reg_x *new_x_reg)
{
    uint32_t    instrc;      //instruction
    uint8_t     opcode;     //opcode
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
    new_x_reg->rs1          = *rs1Val;
    new_x_reg->rs2          = *rs1Val;
    new_x_reg->imm          = imm;
    new_x_reg->rdAddy       = rdAddy;
}
