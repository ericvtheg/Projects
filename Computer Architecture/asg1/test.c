#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

//Memory
uint64_t memory[2048];
bool memory_read (uint64_t address, void *value, uint64_t size_in_bytes){
    *(uint64_t*)value = memory[address];
    printf("Reading memory:\nopcode = %x\nfunc3 = %x\nfunc7 = %x\n",
            (memory[address]) & 0b1111111,
            (memory[address] >> 12) & 0b111,
            (memory[address] >> 25) & 0b1111111);
    return true;
}

bool memory_write (uint64_t address, uint64_t value, uint64_t size_in_bytes){
    memory[address] = value;
    return true;
}
//xtern bool memory_status (uint64_t address, uint64_t *value);
//Registers
uint64_t registers[32];
void register_read (uint64_t register_a, uint64_t register_b, uint64_t * value_a, uint64_t * value_b){
    *value_a = registers[register_a];
    printf("Reading register[%llu] = %llu\n", register_a, *value_a);
    *value_b = registers[register_b];
    printf("Reading register[%llu] = %llu\n", register_b, *value_b);
}

void register_write (uint64_t register_d, uint64_t value_d){
    printf("Setting register[%llu] = %llu\n", register_d, value_d);
    registers[register_d] = value_d;
}

/*
 * This is the function that students have to implement for Assignment 1.
 */
void execute_single_instruction (uint64_t pc, uint64_t * new_pc);

/* CUSTOM FUNCTIONS **************************************/
uint64_t *new_pc;
int testCounter;
int testsPassed;

void AddTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("ADD test: %lld + %lld = %lld\n", (int64_t)a, (int64_t)b, (int64_t)a + (int64_t)b);
                //funct7//rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000000000100010000000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || (int64_t)registers[3] != (int64_t)a + (int64_t)b || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void SubTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("SUB test: %lld - %lld = %lld\n", (int64_t)a, (int64_t)b, (int64_t)a - (int64_t)b);
                //funct7//rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b01000000001000001000000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || (int64_t)registers[3] != (int64_t)a - (int64_t)b || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void DivTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("DIV test: %lld / %lld = %lld\n", (int64_t)a, (int64_t)b, (int64_t)a / (int64_t)b);
                //funct7/rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000010001000001100000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || (int64_t)registers[3] != (int64_t)a / (int64_t)b || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void ORTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("OR test: %llx ^ %llx = %llx\n", a, b, a & b);
                //funct7/rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000000001000001110000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llx \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || registers[3] != (a | b) || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void ANDTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("AND test: %llx ^ %llx = %llx\n", a, b, a & b);
                //funct7/rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000000001000001111000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llx \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || registers[3] != (a & b) || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void XORTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("XOR test: %llx ^ %llx = %llx\n", a, b, a ^ b);
                //funct7/rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000000001000001100000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llx \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || registers[3] != (a ^ b) || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void LUITest(uint64_t a){
    *new_pc = 0;
    testCounter++;
    printf("LUI test: %llx\n", (a & 0xFFFFF000));
                //funct7/rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b000110110111 | (a & 0xFFFFF000);
    
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llx \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || registers[3] != (a & 0xFFFFF000) || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void AUIPCTest(uint64_t a){
    *new_pc = 0;
    testCounter++;
    printf("AUIPC test: %llx\n", (a & 0xFFFFF000));
                //funct7/rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b000110010111 | (a & 0xFFFFF000);
    
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llx \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc !=  4 || registers[3] != (a & 0xFFFFF000)){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

void MULTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("MUL test: %lld * %lld = %lld\n", (int64_t)a, (int64_t)b, (int64_t)a * (int64_t)b);
                //funct7//rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000010000100010000000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || (int64_t)registers[3] != (int64_t)a * (int64_t)b || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

//Not good at grading itself but still useful to test functionality
void MULHTest(uint64_t a, uint64_t b){
    *new_pc = 0;
    testCounter++;
    printf("MULH test: (%lld * %lld) >> 64 = %lld\n", a, b, (uint64_t)(((__int128_t)a * (__int128_t)b) >> 64));
                //funct7//rs2//rs1//f3/rd///opcode/  
    memory[0] = 0b00000010000100010001000110110011;
    
    printf("Setting registers\n");
    register_write(1, a);
    register_write(2, b);
    register_write(3, 0);

    execute_single_instruction(0, new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    printf("Output:\nrd = %llu \nnew pc = %llu\n", registers[3], *new_pc);
    if (*new_pc != 4 || (int64_t)registers[3] != ((__int128_t)a * (__int128_t)b) >> 64 || *new_pc != 4){
        printf("FAILED");
    }
    else{
        testsPassed++;
        printf("PASSED");
    }
    printf("\n\n");
}

int main (int argc, const char *argv[])
{
    int i;
    testCounter = 0;
    testsPassed = 0;
    printf("Starting test\n");
    new_pc = malloc(sizeof(uint64_t));
    srand(time(0));

    //Add tests
    for (i = 0; i < 100; i++)
        AddTest(rand(), rand());

    //Sub tests
    for (i = 0; i < 100; i++)
        SubTest(rand(), rand());

    //Div tests
    for (i = 0; i < 100; i++)
        DivTest(rand() & 0xFFFF, rand() & 0xFFFF);
    
    //OR tests
    for (i = 0; i < 100; i++)
        ORTest(rand(), rand());

    //AND tests
    for (i = 0; i < 100; i++)
        ANDTest(rand(), rand());

    //XOR tests
    for (i = 0; i < 100; i++)
        XORTest(rand(), rand());

    //LUI tests
    for (i = 0; i < 100; i++)
        LUITest(rand());

    //AUIPC tests
    for (i = 0; i < 100; i++)
        AUIPCTest(rand());

    //MULTests
    for (i = 0; i < 100; i++)
        MULTest(rand(), rand());
    
    //MULHTests
    for (i = 0; i < 100; i++)
        MULHTest(rand(), rand());
    MULHTest(-1, -1);
    MULHTest(-1, 1);

    printf("SCORE: %d / %d\n", testsPassed, testCounter);
    
}