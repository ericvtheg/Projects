/*
 * External routines for use in CMPE 110, Fall 2018
 *
 * (c) 2018 Ethan L. Miller
 * Redistribution not permitted without explicit permission from the copyright holder.
 *
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

extern bool memory_read (uint64_t address, void * value, uint64_t size_in_bytes);
extern bool memory_write (uint64_t address, uint64_t value, uint64_t size_in_bytes);
extern bool memory_status (uint64_t address, void *value);

extern void register_read (uint64_t register_a, uint64_t register_b, uint64_t * value_a, uint64_t * value_b);
extern void register_write (uint64_t register_d, uint64_t value_d);

extern void     set_pc (uint64_t pc);
extern uint64_t get_pc (void);

/*
 * These pipeline stage registers are loaded at the end of the cycle with whatever
 * values were filled in by the relevant pipeline stage.
 *
 * Add fields to these stage registers for whatever values you'd like to have passed from
 * one stage to the next.  You may have as many values as you like in each stage.  However,
 * this is the ONLY information that may be passed from one stage to stages in the next
 * cycle.
 */
struct stage_reg_d {
    uint64_t    new_pc;
    uint32_t    instruction;
    uint8_t     opcode;
    bool        prevMem;
    bool        cacheStall;
};

struct stage_reg_x {
    uint64_t    new_pc;
    uint32_t    instruction;
    uint8_t     opcode;
    uint8_t     func3;
    uint8_t     func7;
    uint64_t    rs1;
    uint64_t    rs2;
    uint64_t    rs1Addy;
    uint64_t    rs2Addy;
    uint64_t    imm;
    uint64_t    rdAddy;
    bool        cacheStall;
};

struct stage_reg_m {
    uint64_t    new_pc;
    uint32_t    instruction;
    uint8_t     opcode;
    uint8_t     func3;
    uint64_t    rs1;
    uint64_t    rs2;
    uint64_t    rs1Addy;
    uint64_t    rs2Addy;
    uint64_t    imm;
    uint64_t    rdAddy;
    uint64_t    output;
    uint64_t    address;
    uint64_t    bytes;
    bool        misprediction;
    bool        wasLd;
};

struct stage_reg_w {
    uint64_t    new_pc;
    uint32_t    instruction;
    uint8_t     opcode;
    uint64_t    rdAddy;
    uint64_t    rdVal;
    bool        prevMem;
    bool        memStall;
};

/*
 * These are the functions students need to implement for Assignment 2.
 * Each of your functions must fill in the fields for the stage register
 * passed by reference.  The contents of the fields that you fill in
 * will be copied to the pipeline stage registers above (current_stage_?_register)
 * at the end of a CPU cycle, and will be available for the pipeline
 * stage functions in the next cycle.
 */
extern void stage_fetch (struct stage_reg_d *new_d_reg);
extern void stage_decode (struct stage_reg_x *new_x_reg);
extern void stage_execute (struct stage_reg_m *new_m_reg);
extern void stage_memory (struct stage_reg_w *new_w_reg);
extern void stage_writeback (void);
