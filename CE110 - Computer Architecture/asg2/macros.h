/********************************************************************************
William O'Callahan, Eric Ventor, Ivan Espiritu
Assignement 2
********************************************************************************/
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

// Branch Target Buffer
uint64_t BTB[31][2];  
