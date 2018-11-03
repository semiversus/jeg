#ifndef CPU6502_H
#define CPU6502_H

#include <stdint.h>
#include "jeg_cfg.h"

typedef enum address_mode_enum_t {
  ADR_ABSOLUTE, ADR_ABSOLUTE_X, ADR_ABSOLUTE_Y, ADR_ACCUMULATOR, ADR_IMMEDIATE,
  ADR_IMPLIED, ADR_INDEXED_INDIRECT, ADR_INDIRECT, ADR_INDIRECT_INDEXED,
  ADR_RELATIVE, ADR_ZERO_PAGE, ADR_ZERO_PAGE_X, ADR_ZERO_PAGE_Y
} address_mode_enum_t;

typedef enum operation_enum_t {
  OP_ADC, OP_AHX, OP_ALR, OP_ANC, OP_AND, OP_ARR, OP_ASL, OP_AXS, OP_BCC, OP_BCS, OP_BEQ, OP_BIT, OP_BMI, OP_BNE, OP_BPL,
  OP_BRK, OP_BVC, OP_BVS, OP_CLC, OP_CLD, OP_CLI, OP_CLV, OP_CMP, OP_CPX, OP_CPY, OP_DCP, OP_DEC, OP_DEX, OP_DEY, OP_EOR,
  OP_INC, OP_INX, OP_INY, OP_ISC, OP_JMP, OP_JSR, OP_KIL, OP_LAS, OP_LAX, OP_LDA, OP_LDX, OP_LDY, OP_LSR, OP_NOP, OP_ORA,
  OP_PHA, OP_PHP, OP_PLA, OP_PLP, OP_RLA, OP_ROL, OP_ROR, OP_RRA, OP_RTI, OP_RTS, OP_SAX, OP_SBC, OP_SEC, OP_SED, OP_SEI,
  OP_SHX, OP_SHY, OP_SLO, OP_SRE, OP_STA, OP_STX, OP_STY, OP_TAS, OP_TAX, OP_TAY, OP_TSX, OP_TXA, OP_TXS, OP_TYA, OP_XAA
} operation_enum_t;

typedef struct opcode_tbl_entry_t {
  char *mnemonic;
  operation_enum_t operation;
  address_mode_enum_t address_mode;
  int bytes;
  int cycles;
  int page_cross_cycles;
} opcode_tbl_entry_t;

extern const opcode_tbl_entry_t opcode_tbl[];

typedef enum {INTERRUPT_NONE=0, INTERRUPT_NMI, INTERRUPT_IRQ} cpu6502_interrupt_enum_t;

typedef uint_fast8_t cpu6502_read_func_t (void *, uint_fast16_t hwAddress); // read data [8bit] from address [16bit]
typedef void cpu6502_write_func_t (void *, uint_fast16_t hwAddress, uint_fast8_t chValue); // write data [8bit] to address [16bit]

#if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
typedef uint_fast16_t cpu6502_readw_func_t (void *, uint_fast16_t hwAddress);
typedef void cpu6502_writew_func_t(void *, uint_fast16_t hwAddress, uint_fast16_t hwValue);
#endif

typedef struct cpu6502_t {
    // internal registers (16bit is needed for correct overflow handling)
    int_fast16_t reg_A; // accumulator [8Bit]
    int_fast16_t reg_X; // x register [8Bit]
    int_fast16_t reg_Y; // y register [8Bit]
    int_fast16_t reg_SP; // stack pointer [8Bit]
    uint_fast16_t reg_PC; // program counter [16Bit]

    // status flags
    union {
        struct {
            uint8_t status_C : 1; // carry flag [Bit0]
            uint8_t status_Z : 1; // zero flag [Bit1]
            uint8_t status_I : 1; // interrupt flag [Bit2]
            uint8_t status_D : 1; // decimal mode flag [Bit3]
            uint8_t status_B : 1; // break command flag [Bit4]
            uint8_t status_U : 1; // unused flag [Bit5]
            uint8_t status_V : 1; // overflow flag [Bit6]
            uint8_t status_N : 1; // negative flag [Bit7]
        };
        uint8_t chStatus;
    };
    // emulation internals
    uint64_t cycle_number; // number of actual cycle (measured in ppu cycles)
    int_fast32_t stall_cycles; // number of stall cycles
    cpu6502_interrupt_enum_t interrupt_pending; // type of pending interrupt

    // memory interface
    void *reference; // pointer to a reference, added as argument to read and write functions
    cpu6502_read_func_t *read;
    cpu6502_write_func_t *write;
#if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
    cpu6502_readw_func_t *readw;
    cpu6502_writew_func_t *writew;
#endif
} cpu6502_t;

#if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
typedef struct
{
    void                    *reference;
    cpu6502_read_func_t     *read;
    cpu6502_write_func_t    *write;
#if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
    cpu6502_readw_func_t    *readw;
    cpu6502_writew_func_t   *writew;
#endif
}cpu6502_cfg_t;

extern bool cpu6502_init(cpu6502_t *cpu, cpu6502_cfg_t *ptCFG);
#else
extern void cpu6502_init(cpu6502_t *cpu, void *reference, cpu6502_read_func_t read, cpu6502_write_func_t write);
#endif

extern void cpu6502_reset(cpu6502_t *cpu); // reset cpu to powerup state

//! \brief run cpu for (at least) n_cycles; a started instruction will not be "truncated";
extern uint_fast32_t cpu6502_run(cpu6502_t *cpu, int_fast32_t n_cycles);
                                               // returns number of cycles cpu ran
extern void cpu6502_trigger_interrupt(cpu6502_t *cpu, cpu6502_interrupt_enum_t interrupt); // trigger an interrupt

#endif
