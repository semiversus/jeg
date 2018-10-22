#include "cpu6502.h"
#include "cpu6502_debug.h"

#include "common.h"
#include "jeg_cfg.h"

const opcode_tbl_entry_t opcode_tbl[256]={
  // mnemonic   |address mode            |cycles
  //    |operation                    |bytes|page crossed cycles
  {"BRK", OP_BRK, ADR_IMPLIED         , 1, 7, 0}, // 0x00
  {"ORA", OP_ORA, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0x01
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x02
  {"SLO", OP_SLO, ADR_INDEXED_INDIRECT, 0, 8, 0}, // 0x03
  {"NOP", OP_NOP, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x04
  {"ORA", OP_ORA, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x05
  {"ASL", OP_ASL, ADR_ZERO_PAGE       , 2, 5, 0}, // 0x06
  {"SLO", OP_SLO, ADR_ZERO_PAGE       , 0, 5, 0}, // 0x07
  {"PHP", OP_PHP, ADR_IMPLIED         , 1, 3, 0}, // 0x08
  {"ORA", OP_ORA, ADR_IMMEDIATE       , 2, 2, 0}, // 0x09
  {"ASL", OP_ASL, ADR_ACCUMULATOR     , 1, 2, 0}, // 0x0A
  {"ANC", OP_ANC, ADR_IMMEDIATE       , 0, 2, 0}, // 0x0B
  {"NOP", OP_NOP, ADR_ABSOLUTE        , 3, 4, 0}, // 0x0C
  {"ORA", OP_ORA, ADR_ABSOLUTE        , 3, 4, 0}, // 0x0D
  {"ASL", OP_ASL, ADR_ABSOLUTE        , 3, 6, 0}, // 0x0E
  {"SLO", OP_SLO, ADR_ABSOLUTE        , 0, 6, 0}, // 0x0F
  {"BPL", OP_BPL, ADR_RELATIVE        , 2, 2, 1}, // 0x10
  {"ORA", OP_ORA, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0x11
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x12
  {"SLO", OP_SLO, ADR_INDIRECT_INDEXED, 0, 8, 0}, // 0x13
  {"NOP", OP_NOP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x14
  {"ORA", OP_ORA, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x15
  {"ASL", OP_ASL, ADR_ZERO_PAGE_X     , 2, 6, 0}, // 0x16
  {"SLO", OP_SLO, ADR_ZERO_PAGE_X     , 0, 6, 0}, // 0x17
  {"CLC", OP_CLC, ADR_IMPLIED         , 1, 2, 0}, // 0x18
  {"ORA", OP_ORA, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0x19
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0x1A
  {"SLO", OP_SLO, ADR_ABSOLUTE_Y      , 0, 7, 0}, // 0x1B
  {"NOP", OP_NOP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x1C
  {"ORA", OP_ORA, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x1D
  {"ASL", OP_ASL, ADR_ABSOLUTE_X      , 3, 7, 0}, // 0x1E
  {"SLO", OP_SLO, ADR_ABSOLUTE_X      , 0, 7, 0}, // 0x1F
  {"JSR", OP_JSR, ADR_ABSOLUTE        , 3, 6, 0}, // 0x20
  {"AND", OP_AND, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0x21
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x22
  {"RLA", OP_RLA, ADR_INDEXED_INDIRECT, 0, 8, 0}, // 0x23
  {"BIT", OP_BIT, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x24
  {"AND", OP_AND, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x25
  {"ROL", OP_ROL, ADR_ZERO_PAGE       , 2, 5, 0}, // 0x26
  {"RLA", OP_RLA, ADR_ZERO_PAGE       , 0, 5, 0}, // 0x27
  {"PLP", OP_PLP, ADR_IMPLIED         , 1, 4, 0}, // 0x28
  {"AND", OP_AND, ADR_IMMEDIATE       , 2, 2, 0}, // 0x29
  {"ROL", OP_ROL, ADR_ACCUMULATOR     , 1, 2, 0}, // 0x2A
  {"ANC", OP_ANC, ADR_IMMEDIATE       , 0, 2, 0}, // 0x2B
  {"BIT", OP_BIT, ADR_ABSOLUTE        , 3, 4, 0}, // 0x2C
  {"AND", OP_AND, ADR_ABSOLUTE        , 3, 4, 0}, // 0x2D
  {"ROL", OP_ROL, ADR_ABSOLUTE        , 3, 6, 0}, // 0x2E
  {"RLA", OP_RLA, ADR_ABSOLUTE        , 0, 6, 0}, // 0x2F
  {"BMI", OP_BMI, ADR_RELATIVE        , 2, 2, 1}, // 0x30
  {"AND", OP_AND, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0x31
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x32
  {"RLA", OP_RLA, ADR_INDIRECT_INDEXED, 0, 8, 0}, // 0x33
  {"NOP", OP_NOP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x34
  {"AND", OP_AND, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x35
  {"ROL", OP_ROL, ADR_ZERO_PAGE_X     , 2, 6, 0}, // 0x36
  {"RLA", OP_RLA, ADR_ZERO_PAGE_X     , 0, 6, 0}, // 0x37
  {"SEC", OP_SEC, ADR_IMPLIED         , 1, 2, 0}, // 0x38
  {"AND", OP_AND, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0x39
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0x3A
  {"RLA", OP_RLA, ADR_ABSOLUTE_Y      , 0, 7, 0}, // 0x3B
  {"NOP", OP_NOP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x3C
  {"AND", OP_AND, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x3D
  {"ROL", OP_ROL, ADR_ABSOLUTE_X      , 3, 7, 0}, // 0x3E
  {"RLA", OP_RLA, ADR_ABSOLUTE_X      , 0, 7, 0}, // 0x3F
  {"RTI", OP_RTI, ADR_IMPLIED         , 1, 6, 0}, // 0x40
  {"EOR", OP_EOR, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0x41
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x42
  {"SRE", OP_SRE, ADR_INDEXED_INDIRECT, 0, 8, 0}, // 0x43
  {"NOP", OP_NOP, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x44
  {"EOR", OP_EOR, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x45
  {"LSR", OP_LSR, ADR_ZERO_PAGE       , 2, 5, 0}, // 0x46
  {"SRE", OP_SRE, ADR_ZERO_PAGE       , 0, 5, 0}, // 0x47
  {"PHA", OP_PHA, ADR_IMPLIED         , 1, 3, 0}, // 0x48
  {"EOR", OP_EOR, ADR_IMMEDIATE       , 2, 2, 0}, // 0x49
  {"LSR", OP_LSR, ADR_ACCUMULATOR     , 1, 2, 0}, // 0x4A
  {"ALR", OP_ALR, ADR_IMMEDIATE       , 0, 2, 0}, // 0x4B
  {"JMP", OP_JMP, ADR_ABSOLUTE        , 3, 3, 0}, // 0x4C
  {"EOR", OP_EOR, ADR_ABSOLUTE        , 3, 4, 0}, // 0x4D
  {"LSR", OP_LSR, ADR_ABSOLUTE        , 3, 6, 0}, // 0x4E
  {"SRE", OP_SRE, ADR_ABSOLUTE        , 0, 6, 0}, // 0x4F
  {"BVC", OP_BVC, ADR_RELATIVE        , 2, 2, 1}, // 0x50
  {"EOR", OP_EOR, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0x51
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x52
  {"SRE", OP_SRE, ADR_INDIRECT_INDEXED, 0, 8, 0}, // 0x53
  {"NOP", OP_NOP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x54
  {"EOR", OP_EOR, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x55
  {"LSR", OP_LSR, ADR_ZERO_PAGE_X     , 2, 6, 0}, // 0x56
  {"SRE", OP_SRE, ADR_ZERO_PAGE_X     , 0, 6, 0}, // 0x57
  {"CLI", OP_CLI, ADR_IMPLIED         , 1, 2, 0}, // 0x58
  {"EOR", OP_EOR, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0x59
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0x5A
  {"SRE", OP_SRE, ADR_ABSOLUTE_Y      , 0, 7, 0}, // 0x5B
  {"NOP", OP_NOP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x5C
  {"EOR", OP_EOR, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x5D
  {"LSR", OP_LSR, ADR_ABSOLUTE_X      , 3, 7, 0}, // 0x5E
  {"SRE", OP_SRE, ADR_ABSOLUTE_X      , 0, 7, 0}, // 0x5F
  {"RTS", OP_RTS, ADR_IMPLIED         , 1, 6, 0}, // 0x60
  {"ADC", OP_ADC, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0x61
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x62
  {"RRA", OP_RRA, ADR_INDEXED_INDIRECT, 0, 8, 0}, // 0x63
  {"NOP", OP_NOP, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x64
  {"ADC", OP_ADC, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x65
  {"ROR", OP_ROR, ADR_ZERO_PAGE       , 2, 5, 0}, // 0x66
  {"RRA", OP_RRA, ADR_ZERO_PAGE       , 0, 5, 0}, // 0x67
  {"PLA", OP_PLA, ADR_IMPLIED         , 1, 4, 0}, // 0x68
  {"ADC", OP_ADC, ADR_IMMEDIATE       , 2, 2, 0}, // 0x69
  {"ROR", OP_ROR, ADR_ACCUMULATOR     , 1, 2, 0}, // 0x6A
  {"ARR", OP_ARR, ADR_IMMEDIATE       , 0, 2, 0}, // 0x6B
  {"JMP", OP_JMP, ADR_INDIRECT        , 3, 5, 0}, // 0x6C
  {"ADC", OP_ADC, ADR_ABSOLUTE        , 3, 4, 0}, // 0x6D
  {"ROR", OP_ROR, ADR_ABSOLUTE        , 3, 6, 0}, // 0x6E
  {"RRA", OP_RRA, ADR_ABSOLUTE        , 0, 6, 0}, // 0x6F
  {"BVS", OP_BVS, ADR_RELATIVE        , 2, 2, 1}, // 0x70
  {"ADC", OP_ADC, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0x71
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x72
  {"RRA", OP_RRA, ADR_INDIRECT_INDEXED, 0, 8, 0}, // 0x73
  {"NOP", OP_NOP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x74
  {"ADC", OP_ADC, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x75
  {"ROR", OP_ROR, ADR_ZERO_PAGE_X     , 2, 6, 0}, // 0x76
  {"RRA", OP_RRA, ADR_ZERO_PAGE_X     , 0, 6, 0}, // 0x77
  {"SEI", OP_SEI, ADR_IMPLIED         , 1, 2, 0}, // 0x78
  {"ADC", OP_ADC, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0x79
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0x7A
  {"RRA", OP_RRA, ADR_ABSOLUTE_Y      , 0, 7, 0}, // 0x7B
  {"NOP", OP_NOP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x7C
  {"ADC", OP_ADC, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0x7D
  {"ROR", OP_ROR, ADR_ABSOLUTE_X      , 3, 7, 0}, // 0x7E
  {"RRA", OP_RRA, ADR_ABSOLUTE_X      , 0, 7, 0}, // 0x7F
  {"NOP", OP_NOP, ADR_IMMEDIATE       , 2, 2, 0}, // 0x80
  {"STA", OP_STA, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0x81
  {"NOP", OP_NOP, ADR_IMMEDIATE       , 0, 2, 0}, // 0x82
  {"SAX", OP_SAX, ADR_INDEXED_INDIRECT, 0, 6, 0}, // 0x83
  {"STY", OP_STY, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x84
  {"STA", OP_STA, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x85
  {"STX", OP_STX, ADR_ZERO_PAGE       , 2, 3, 0}, // 0x86
  {"SAX", OP_SAX, ADR_ZERO_PAGE       , 0, 3, 0}, // 0x87
  {"DEY", OP_DEY, ADR_IMPLIED         , 1, 2, 0}, // 0x88
  {"NOP", OP_NOP, ADR_IMMEDIATE       , 0, 2, 0}, // 0x89
  {"TXA", OP_TXA, ADR_IMPLIED         , 1, 2, 0}, // 0x8A
  {"XAA", OP_XAA, ADR_IMMEDIATE       , 0, 2, 0}, // 0x8B
  {"STY", OP_STY, ADR_ABSOLUTE        , 3, 4, 0}, // 0x8C
  {"STA", OP_STA, ADR_ABSOLUTE        , 3, 4, 0}, // 0x8D
  {"STX", OP_STX, ADR_ABSOLUTE        , 3, 4, 0}, // 0x8E
  {"SAX", OP_SAX, ADR_ABSOLUTE        , 0, 4, 0}, // 0x8F
  {"BCC", OP_BCC, ADR_RELATIVE        , 2, 2, 1}, // 0x90
  {"STA", OP_STA, ADR_INDIRECT_INDEXED, 2, 6, 0}, // 0x91
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0x92
  {"AHX", OP_AHX, ADR_INDIRECT_INDEXED, 0, 6, 0}, // 0x93
  {"STY", OP_STY, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x94
  {"STA", OP_STA, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0x95
  {"STX", OP_STX, ADR_ZERO_PAGE_Y     , 2, 4, 0}, // 0x96
  {"SAX", OP_SAX, ADR_ZERO_PAGE_Y     , 0, 4, 0}, // 0x97
  {"TYA", OP_TYA, ADR_IMPLIED         , 1, 2, 0}, // 0x98
  {"STA", OP_STA, ADR_ABSOLUTE_Y      , 3, 5, 0}, // 0x99
  {"TXS", OP_TXS, ADR_IMPLIED         , 1, 2, 0}, // 0x9A
  {"TAS", OP_TAS, ADR_ABSOLUTE_Y      , 0, 5, 0}, // 0x9B
  {"SHY", OP_SHY, ADR_ABSOLUTE_X      , 0, 5, 0}, // 0x9C
  {"STA", OP_STA, ADR_ABSOLUTE_X      , 3, 5, 0}, // 0x9D
  {"SHX", OP_SHX, ADR_ABSOLUTE_Y      , 0, 5, 0}, // 0x9E
  {"AHX", OP_AHX, ADR_ABSOLUTE_Y      , 0, 5, 0}, // 0x9F
  {"LDY", OP_LDY, ADR_IMMEDIATE       , 2, 2, 0}, // 0xA0
  {"LDA", OP_LDA, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0xA1
  {"LDX", OP_LDX, ADR_IMMEDIATE       , 2, 2, 0}, // 0xA2
  {"LAX", OP_LAX, ADR_INDEXED_INDIRECT, 0, 6, 0}, // 0xA3
  {"LDY", OP_LDY, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xA4
  {"LDA", OP_LDA, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xA5
  {"LDX", OP_LDX, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xA6
  {"LAX", OP_LAX, ADR_ZERO_PAGE       , 0, 3, 0}, // 0xA7
  {"TAY", OP_TAY, ADR_IMPLIED         , 1, 2, 0}, // 0xA8
  {"LDA", OP_LDA, ADR_IMMEDIATE       , 2, 2, 0}, // 0xA9
  {"TAX", OP_TAX, ADR_IMPLIED         , 1, 2, 0}, // 0xAA
  {"LAX", OP_LAX, ADR_IMMEDIATE       , 0, 2, 0}, // 0xAB
  {"LDY", OP_LDY, ADR_ABSOLUTE        , 3, 4, 0}, // 0xAC
  {"LDA", OP_LDA, ADR_ABSOLUTE        , 3, 4, 0}, // 0xAD
  {"LDX", OP_LDX, ADR_ABSOLUTE        , 3, 4, 0}, // 0xAE
  {"LAX", OP_LAX, ADR_ABSOLUTE        , 0, 4, 0}, // 0xAF
  {"BCS", OP_BCS, ADR_RELATIVE        , 2, 2, 1}, // 0xB0
  {"LDA", OP_LDA, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0xB1
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0xB2
  {"LAX", OP_LAX, ADR_INDIRECT_INDEXED, 0, 5, 1}, // 0xB3
  {"LDY", OP_LDY, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0xB4
  {"LDA", OP_LDA, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0xB5
  {"LDX", OP_LDX, ADR_ZERO_PAGE_Y     , 2, 4, 0}, // 0xB6
  {"LAX", OP_LAX, ADR_ZERO_PAGE_Y     , 0, 4, 0}, // 0xB7
  {"CLV", OP_CLV, ADR_IMPLIED         , 1, 2, 0}, // 0xB8
  {"LDA", OP_LDA, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0xB9
  {"TSX", OP_TSX, ADR_IMPLIED         , 1, 2, 0}, // 0xBA
  {"LAS", OP_LAS, ADR_ABSOLUTE_Y      , 0, 4, 1}, // 0xBB
  {"LDY", OP_LDY, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0xBC
  {"LDA", OP_LDA, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0xBD
  {"LDX", OP_LDX, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0xBE
  {"LAX", OP_LAX, ADR_ABSOLUTE_Y      , 0, 4, 1}, // 0xBF
  {"CPY", OP_CPY, ADR_IMMEDIATE       , 2, 2, 0}, // 0xC0
  {"CMP", OP_CMP, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0xC1
  {"NOP", OP_NOP, ADR_IMMEDIATE       , 0, 2, 0}, // 0xC2
  {"DCP", OP_DCP, ADR_INDEXED_INDIRECT, 0, 8, 0}, // 0xC3
  {"CPY", OP_CPY, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xC4
  {"CMP", OP_CMP, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xC5
  {"DEC", OP_DEC, ADR_ZERO_PAGE       , 2, 5, 0}, // 0xC6
  {"DCP", OP_DCP, ADR_ZERO_PAGE       , 0, 5, 0}, // 0xC7
  {"INY", OP_INY, ADR_IMPLIED         , 1, 2, 0}, // 0xC8
  {"CMP", OP_CMP, ADR_IMMEDIATE       , 2, 2, 0}, // 0xC9
  {"DEX", OP_DEX, ADR_IMPLIED         , 1, 2, 0}, // 0xCA
  {"AXS", OP_AXS, ADR_IMMEDIATE       , 0, 2, 0}, // 0xCB
  {"CPY", OP_CPY, ADR_ABSOLUTE        , 3, 4, 0}, // 0xCC
  {"CMP", OP_CMP, ADR_ABSOLUTE        , 3, 4, 0}, // 0xCD
  {"DEC", OP_DEC, ADR_ABSOLUTE        , 3, 6, 0}, // 0xCE
  {"DCP", OP_DCP, ADR_ABSOLUTE        , 0, 6, 0}, // 0xCF
  {"BNE", OP_BNE, ADR_RELATIVE        , 2, 2, 1}, // 0xD0
  {"CMP", OP_CMP, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0xD1
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0xD2
  {"DCP", OP_DCP, ADR_INDIRECT_INDEXED, 0, 8, 0}, // 0xD3
  {"NOP", OP_NOP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0xD4
  {"CMP", OP_CMP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0xD5
  {"DEC", OP_DEC, ADR_ZERO_PAGE_X     , 2, 6, 0}, // 0xD6
  {"DCP", OP_DCP, ADR_ZERO_PAGE_X     , 0, 6, 0}, // 0xD7
  {"CLD", OP_CLD, ADR_IMPLIED         , 1, 2, 0}, // 0xD8
  {"CMP", OP_CMP, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0xD9
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0xDA
  {"DCP", OP_DCP, ADR_ABSOLUTE_Y      , 0, 7, 0}, // 0xDB
  {"NOP", OP_NOP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0xDC
  {"CMP", OP_CMP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0xDD
  {"DEC", OP_DEC, ADR_ABSOLUTE_X      , 3, 7, 0}, // 0xDE
  {"DCP", OP_DCP, ADR_ABSOLUTE_X      , 0, 7, 0}, // 0xDF
  {"CPX", OP_CPX, ADR_IMMEDIATE       , 2, 2, 0}, // 0xE0
  {"SBC", OP_SBC, ADR_INDEXED_INDIRECT, 2, 6, 0}, // 0xE1
  {"NOP", OP_NOP, ADR_IMMEDIATE       , 0, 2, 0}, // 0xE2
  {"ISC", OP_ISC, ADR_INDEXED_INDIRECT, 0, 8, 0}, // 0xE3
  {"CPX", OP_CPX, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xE4
  {"SBC", OP_SBC, ADR_ZERO_PAGE       , 2, 3, 0}, // 0xE5
  {"INC", OP_INC, ADR_ZERO_PAGE       , 2, 5, 0}, // 0xE6
  {"ISC", OP_ISC, ADR_ZERO_PAGE       , 0, 5, 0}, // 0xE7
  {"INX", OP_INX, ADR_IMPLIED         , 1, 2, 0}, // 0xE8
  {"SBC", OP_SBC, ADR_IMMEDIATE       , 2, 2, 0}, // 0xE9
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0xEA
  {"SBC", OP_SBC, ADR_IMMEDIATE       , 0, 2, 0}, // 0xEB
  {"CPX", OP_CPX, ADR_ABSOLUTE        , 3, 4, 0}, // 0xEC
  {"SBC", OP_SBC, ADR_ABSOLUTE        , 3, 4, 0}, // 0xED
  {"INC", OP_INC, ADR_ABSOLUTE        , 3, 6, 0}, // 0xEE
  {"ISC", OP_ISC, ADR_ABSOLUTE        , 0, 6, 0}, // 0xEF
  {"BEQ", OP_BEQ, ADR_RELATIVE        , 2, 2, 1}, // 0xF0
  {"SBC", OP_SBC, ADR_INDIRECT_INDEXED, 2, 5, 1}, // 0xF1
  {"KIL", OP_KIL, ADR_IMPLIED         , 0, 2, 0}, // 0xF2
  {"ISC", OP_ISC, ADR_INDIRECT_INDEXED, 0, 8, 0}, // 0xF3
  {"NOP", OP_NOP, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0xF4
  {"SBC", OP_SBC, ADR_ZERO_PAGE_X     , 2, 4, 0}, // 0xF5
  {"INC", OP_INC, ADR_ZERO_PAGE_X     , 2, 6, 0}, // 0xF6
  {"ISC", OP_ISC, ADR_ZERO_PAGE_X     , 0, 6, 0}, // 0xF7
  {"SED", OP_SED, ADR_IMPLIED         , 1, 2, 0}, // 0xF8
  {"SBC", OP_SBC, ADR_ABSOLUTE_Y      , 3, 4, 1}, // 0xF9
  {"NOP", OP_NOP, ADR_IMPLIED         , 1, 2, 0}, // 0xFA
  {"ISC", OP_ISC, ADR_ABSOLUTE_Y      , 0, 7, 0}, // 0xFB
  {"NOP", OP_NOP, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0xFC
  {"SBC", OP_SBC, ADR_ABSOLUTE_X      , 3, 4, 1}, // 0xFD
  {"INC", OP_INC, ADR_ABSOLUTE_X      , 3, 7, 0}, // 0xFE
  {"ISC", OP_ISC, ADR_ABSOLUTE_X      , 0, 7, 0}  // 0xFF
};


#define SET_FLAGS(v)        do {cpu->chStatus = (v);} while(0)
#define GET_FLAGS()         (cpu->chStatus)

#define __BV(__N)           ((uint32_t)1<<(__N))

#define RECALC_ZN(v)                                                            \
            do{                                                                 \
                cpu->status_Z=(v)?0:1;                                          \
                cpu->status_N=(v)&0x80?1:0;                                     \
            } while(0)

#define PUSH(v)                                                                 \
    do {                                                                        \
        cpu->write(cpu->reference, 0x100|cpu->reg_SP, (v)&0xFF);                \
        cpu->reg_SP--;                                                          \
        if (cpu->reg_SP<0) {                                                    \
            cpu->reg_SP=0xFF;                                                   \
        }                                                                       \
    } while(0)

#if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
#define READ16(adr)     (cpu->readw(cpu->reference, adr))
#else
#define READ16(adr)     (cpu->read(cpu->reference, adr)|(cpu->read(cpu->reference, (adr)+1)<<8))
#endif
#define READ16BUG(adr)  (   (cpu->read(     cpu->reference, adr))               \
                        |   ((cpu->read(    cpu->reference,     ((adr)&0xFF00)  \
                                                            +   (((adr)+1)&0xFF)\
                                       )<<8)))

#define PAGE_DIFFERS(a,b) (((a)&0xFF00)!=((b)&0xFF00))

#define BRANCH(c)                                                               \
    do {                                                                        \
        if (c) {                                                                \
            cycles_passed+=PAGE_DIFFERS(cpu->reg_PC, address)?2:1;              \
            cpu->reg_PC=address;                                                \
        }                                                                       \
    } while(0)

#define COMPARE(a, b)                                                           \
    do {                                                                        \
        RECALC_ZN((a)-(b));                                                     \
        cpu->status_C= (a)>=(b)?1:0;                                            \
    } while(0)


#if  JEG_USE_DUMMY_READS == ENABLED
#   define DUMMY_READ(adr)
#else
#   define DUMMY_READ(adr) cpu->read(cpu->reference, adr);
#endif

#if JEG_USE_DMA_MEMORY_COPY_ACCELERATION == ENABLED ||                          \
    JEG_USE_EXTRA_16BIT_BUS_ACCESS       == ENABLED
bool cpu6502_init(cpu6502_t *ptCPU, cpu6502_cfg_t *ptCFG)
{
    do {
        if (NULL == ptCPU || NULL == ptCFG) {
            break;
        } else if (     (NULL == ptCFG->reference)
                #if JEG_USE_DMA_MEMORY_COPY_ACCELERATION == ENABLED
                    ||  (NULL == ptCFG->fnDMAGetSourceAddress)
                #endif
                #if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
                    ||  (NULL == ptCFG->readw)
                    ||  (NULL == ptCFG->writew)
                #endif
                    ||  (NULL == ptCFG->read)
                    ||  (NULL == ptCFG->write)) {
            break;
        }

        ptCPU->reference =              ptCFG->reference;
        ptCPU->read =                   ptCFG->read;
        ptCPU->write =                  ptCFG->write;
    #if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
        ptCPU->readw =                  ptCFG->readw;
        ptCPU->writew =                 ptCFG->writew;
    #endif
    #if JEG_USE_DMA_MEMORY_COPY_ACCELERATION == ENABLED
        ptCPU->fnDMAGetSourceAddress =  ptCFG->fnDMAGetSourceAddress;
    #endif
        return true;
    } while(false);

    return false;
}
#else
void cpu6502_init(cpu6502_t *cpu, void *reference, cpu6502_read_func_t read, cpu6502_write_func_t write)
{
    cpu->reference = reference;
    cpu->read=read;
    cpu->write=write;
}
#endif

void cpu6502_reset(cpu6502_t *cpu) {
  // load program counter with address stored at 0xFFFC (low byte) and 0xFFFD (high byte)
    cpu->reg_A = 0;
    cpu->reg_X = 0;
    cpu->reg_Y = 0;

    cpu->reg_PC = READ16(0xFFFC);
    cpu->reg_SP = 0xFD; // reset stack pointer
    SET_FLAGS(0x24); // set following status flags: UNUSED, INTERRUPT
    cpu->cycle_number = 0;
    cpu->interrupt_pending = INTERRUPT_NONE;
    cpu->stall_cycles = 0;
}


uint_fast32_t cpu6502_run(cpu6502_t *cpu, int_fast32_t cycles_to_run)
{
  const opcode_tbl_entry_t *ptOpcode;
  uint_fast32_t cycles_passed; // cycles used in one iteration
  int address=0; // calculated address for memory interaction
  int temp_value, temp_value2; // temporary value used for calculation

  do {
    cycles_passed=0;

    // check if cpu is stalling
    if (cpu->stall_cycles) {
      cycles_passed=cpu->stall_cycles;
      cpu->stall_cycles=0;
    }

    // check for interrupts
    if (cpu->interrupt_pending!=INTERRUPT_NONE) {
      PUSH(cpu->reg_PC>>8); // high byte of program counter to stack
      PUSH(cpu->reg_PC); // low byte
      PUSH(GET_FLAGS() | 0x10); // push status flags to stack
      if (cpu->interrupt_pending==INTERRUPT_NMI) {
        cpu->reg_PC=READ16(0xFFFA);
      }
      else {
        cpu->reg_PC=READ16(0xFFFE);
      }
      cpu->status_I=1;
      cycles_passed+=7;
      cpu->interrupt_pending=INTERRUPT_NONE;
    }

    // read op code
    ptOpcode = &opcode_tbl[cpu->read(cpu->reference, cpu->reg_PC)];

    // handle address mode
    switch (ptOpcode->address_mode) {
      case ADR_ABSOLUTE:
        address=READ16(cpu->reg_PC+1);
        break;
      case ADR_ABSOLUTE_X:
        address = READ16(cpu->reg_PC+1)+cpu->reg_X;
        if (PAGE_DIFFERS(address-cpu->reg_X, address)) {
          cycles_passed+=ptOpcode->page_cross_cycles;
          DUMMY_READ(address-0x100);                                            //!< dummy read
        }
        break;
      case ADR_ABSOLUTE_Y:
        address = READ16(cpu->reg_PC+1)+cpu->reg_Y;
        if (PAGE_DIFFERS(address-cpu->reg_Y, address)) {
          cycles_passed+=ptOpcode->page_cross_cycles;
          DUMMY_READ(address-0x100);                                            //!< dummy read
        }
        break;
      case ADR_ACCUMULATOR:
        DUMMY_READ(cpu->reg_PC+1);                                              // dummy read
        address=0;
        break;
      case ADR_IMMEDIATE:
        address=cpu->reg_PC+1;
        break;
      case ADR_IMPLIED:
        DUMMY_READ(cpu->reg_PC+1);                                              // dummy read
        address=0;
        break;
      case ADR_INDEXED_INDIRECT:
        address=READ16BUG((cpu->read(cpu->reference, cpu->reg_PC+1)+cpu->reg_X)&0xFF);
        break;
      case ADR_INDIRECT:
        address=READ16BUG(READ16(cpu->reg_PC+1));
        break;
      case ADR_INDIRECT_INDEXED:
        address=READ16BUG(cpu->read(cpu->reference, cpu->reg_PC+1))+cpu->reg_Y;
        if (PAGE_DIFFERS(address-cpu->reg_Y, address)) {
          cycles_passed+=ptOpcode->page_cross_cycles;
          DUMMY_READ(address-0x100);                                            // dummy read
        }
        break;
      case ADR_RELATIVE:
        address=cpu->read(cpu->reference, cpu->reg_PC+1);
        if (address<0x80) {
          address+=cpu->reg_PC+2;
        }
        else {
          address+=cpu->reg_PC+2-0x100;
        }
        break;
      case ADR_ZERO_PAGE:
        address=cpu->read(cpu->reference, cpu->reg_PC+1)&0xFF;
        break;
      case ADR_ZERO_PAGE_X:
        address=(cpu->read(cpu->reference, cpu->reg_PC+1)+cpu->reg_X)&0xFF;
        break;
      case ADR_ZERO_PAGE_Y:
        address=(cpu->read(cpu->reference, cpu->reg_PC+1)+cpu->reg_Y)&0xFF;
        break;
    }

    address&=0xFFFF; // mask 16 bit

    cpu->reg_PC+=ptOpcode->bytes; // update program counter
    cycles_passed+=ptOpcode->cycles; // update cycles for this opcode

    switch(ptOpcode->operation) {
      case OP_ADC:
        temp_value2=cpu->read(cpu->reference, address);
        temp_value=cpu->reg_A+temp_value2+cpu->status_C;
        #if JEG_USE_6502_DECIMAL_MODE == ENABLED
        if (cpu->status_D) { // bcd mode
          if (( (cpu->reg_A&0x0F)+(temp_value2&0x0F)+cpu->status_C)>9) {
            temp_value+=6;
          }
          cpu->status_V=(~(cpu->reg_A^temp_value2))&(cpu->reg_A^temp_value)&0x80?1:0;
          if (temp_value>0x99) {
            temp_value+=96;
          }
          cpu->status_C=temp_value>0x99?1:0;
        }
        else {
        #endif
          cpu->status_C=temp_value>0xFF?1:0;
          cpu->status_V=(~(cpu->reg_A^temp_value2))&(cpu->reg_A^temp_value)&0x80?1:0;
        #if JEG_USE_6502_DECIMAL_MODE == ENABLED
        }
        #endif
        cpu->reg_A=temp_value&0xFF;
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_AND:
        cpu->reg_A=cpu->reg_A&cpu->read(cpu->reference, address);
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_ASL:
        if (ptOpcode->address_mode==ADR_ACCUMULATOR) {
          cpu->status_C= cpu->reg_A&0x80?1:0;
          cpu->reg_A= (cpu->reg_A&0x7F)<<1;
          RECALC_ZN(cpu->reg_A);
        }
        else {
          temp_value=cpu->read(cpu->reference, address);
          cpu->status_C= temp_value&0x80?1:0;
          temp_value=(temp_value&0x7F)<<1;
          cpu->write(cpu->reference, address, temp_value);
          RECALC_ZN(temp_value);
        }
        break;
      case OP_BCC:
        BRANCH(!cpu->status_C);
        break;
      case OP_BCS:
        BRANCH(cpu->status_C);
        break;
      case OP_BEQ:
        BRANCH(cpu->status_Z);
        break;
      case OP_BIT:
        temp_value=cpu->read(cpu->reference, address);
        cpu->status_V=(temp_value&0x40)?1:0;
        cpu->status_Z=(temp_value&cpu->reg_A)?0:1;
        cpu->status_N=temp_value&0x80?1:0;
        break;
      case OP_BMI:
        BRANCH(cpu->status_N);
        break;
      case OP_BNE:
        BRANCH(!cpu->status_Z);
        break;
      case OP_BPL:
        BRANCH(!cpu->status_N);
        break;
      case OP_BRK:
        cpu->reg_PC++;
        PUSH(cpu->reg_PC>>8); // high byte of program counter to stack
        PUSH(cpu->reg_PC); // low byte
        PUSH(GET_FLAGS() | 0x10); // push status flags to stack
        cpu->status_I=1;
        cpu->reg_PC=READ16(0xFFFE);
        break;
      case OP_BVC:
        BRANCH(!cpu->status_V);
        break;
      case OP_BVS:
        BRANCH(cpu->status_V);
        break;
      case OP_CLC:
        cpu->status_C=0;
        break;
      case OP_CLD:
        cpu->status_D=0;
        break;
      case OP_CLI:
        cpu->status_I=0;
        break;
      case OP_CLV:
        cpu->status_V=0;
        break;
      case OP_CMP:
        COMPARE(cpu->reg_A, cpu->read(cpu->reference, address));
        break;
      case OP_CPX:
        COMPARE(cpu->reg_X, cpu->read(cpu->reference, address));
        break;
      case OP_CPY:
        COMPARE(cpu->reg_Y, cpu->read(cpu->reference, address));
        break;
      case OP_DEC:
        temp_value=cpu->read(cpu->reference, address)-1;
        if (temp_value<0) {
          temp_value=0xFF;
        }
        cpu->write(cpu->reference, address, temp_value);
        RECALC_ZN(temp_value);
        break;
      case OP_DEX:
        cpu->reg_X--;
        cpu->reg_X &= 0xFF;
        RECALC_ZN(cpu->reg_X);
        break;
      case OP_DEY:
        cpu->reg_Y--;
        cpu->reg_Y &= 0xFF;
        RECALC_ZN(cpu->reg_Y);
        break;
      case OP_EOR:
        cpu->reg_A=cpu->reg_A^cpu->read(cpu->reference, address);
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_INC:
        temp_value=cpu->read(cpu->reference, address)+1;
        if (temp_value>255) {
          temp_value=0x00;
        }
        cpu->write(cpu->reference, address, temp_value);
        RECALC_ZN(temp_value);
        break;
      case OP_INX:
        cpu->reg_X++;
        cpu->reg_X &= 0xFF;
        RECALC_ZN(cpu->reg_X);
        break;
      case OP_INY:
        cpu->reg_Y++;
        cpu->reg_Y &= 0xFF;
        RECALC_ZN(cpu->reg_Y);
        break;
      case OP_JMP:
        cpu->reg_PC=address;
        break;
      case OP_JSR:
        cpu->reg_PC--;
        PUSH(cpu->reg_PC>>8); // high byte of program counter to stack
        PUSH(cpu->reg_PC); // low byte
        cpu->reg_PC=address;
        break;
      case OP_LDA:
        cpu->reg_A=cpu->read(cpu->reference, address);
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_LDX:
        cpu->reg_X=cpu->read(cpu->reference, address);
        RECALC_ZN(cpu->reg_X);
        break;
      case OP_LDY:
        cpu->reg_Y=cpu->read(cpu->reference, address);
        RECALC_ZN(cpu->reg_Y);
        break;
      case OP_LSR:
        if (ptOpcode->address_mode==ADR_ACCUMULATOR) {
          cpu->status_C= cpu->reg_A&0x01?1:0;
          cpu->reg_A= cpu->reg_A>>1;
          RECALC_ZN(cpu->reg_A);
        }
        else {
          temp_value=cpu->read(cpu->reference, address);
          cpu->status_C= temp_value&0x01?1:0;
          temp_value>>=1;
          cpu->write(cpu->reference, address, temp_value);
          RECALC_ZN(temp_value);
        }
        break;
      case OP_NOP:
        break;
      case OP_ORA:
        cpu->reg_A=cpu->reg_A|cpu->read(cpu->reference, address);
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_PHA:
        PUSH(cpu->reg_A);
        break;
      case OP_PHP:
        PUSH(GET_FLAGS()|0x10);
        break;
      case OP_PLA:
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        cpu->reg_A=cpu->read(cpu->reference, 0x100|cpu->reg_SP);
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_PLP:
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        SET_FLAGS((cpu->read(cpu->reference, 0x100|cpu->reg_SP)&0xEF)|0x20);
        break;
      case OP_ROL:
        if (ptOpcode->address_mode==ADR_ACCUMULATOR) {
          cpu->reg_A=(cpu->reg_A<<1)+cpu->status_C;
          cpu->status_C=cpu->reg_A&0x100?1:0;
          cpu->reg_A&=0xFF;
          RECALC_ZN(cpu->reg_A);
        }
        else {
          temp_value=cpu->read(cpu->reference, address);
          temp_value=(temp_value<<1)+cpu->status_C;
          cpu->status_C=temp_value&0x100?1:0;
          temp_value&=0xFF;
          cpu->write(cpu->reference, address, temp_value);
          RECALC_ZN(temp_value);
        }
        break;
      case OP_ROR:
        if (ptOpcode->address_mode==ADR_ACCUMULATOR) {
          cpu->reg_A|=cpu->status_C<<8;
          cpu->status_C=cpu->reg_A&0x01;
          cpu->reg_A>>=1;
          RECALC_ZN(cpu->reg_A);
        }
        else {
          temp_value=cpu->read(cpu->reference, address)|(cpu->status_C<<8);
          cpu->status_C=temp_value&0x01;
          temp_value>>=1;
          cpu->write(cpu->reference, address, temp_value);
          RECALC_ZN(temp_value);
        }
        break;
      case OP_RTI:
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        SET_FLAGS((cpu->read(cpu->reference, 0x100|cpu->reg_SP)&0xEF)|0x20);
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        cpu->reg_PC=cpu->read(cpu->reference, 0x100|cpu->reg_SP);
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        cpu->reg_PC+=cpu->read(cpu->reference, 0x100|cpu->reg_SP)<<8;
        break;
      case OP_RTS:
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        cpu->reg_PC=cpu->read(cpu->reference, 0x100|cpu->reg_SP);
        cpu->reg_SP=(cpu->reg_SP+1)&0xFF;
        cpu->reg_PC+=cpu->read(cpu->reference, 0x100|cpu->reg_SP)<<8;
        cpu->reg_PC+=1;
        break;
      case OP_SBC:
        // TODO: ugly hack
        temp_value2=cpu->read(cpu->reference, address);
        temp_value=(cpu->reg_A-temp_value2-(1-cpu->status_C))&0xFFFF;
        RECALC_ZN(temp_value&0xFF);
        cpu->status_V=(cpu->reg_A^temp_value2)&(cpu->reg_A^temp_value)&0x80?1:0;
        #ifndef WITHOUT_DECIMAL_MODE
        if (cpu->status_D) { // bcd mode
          if ( ((cpu->reg_A&0x0F)-(1-cpu->status_C))<(temp_value2&0x0F)) {
            temp_value-=6;
          }
          if (temp_value>0x99) {
            temp_value-=0x60;
          }
          cpu->status_C=temp_value<0x100?1:0;
        }
        else {
        #endif
          cpu->status_C=temp_value<0x100?1:0;
        #ifndef WITHOUT_DECIMAL_MODE
        }
        #endif
        cpu->reg_A=temp_value&0xFF;
        break;
      case OP_SEC:
        cpu->status_C=1;
        break;
      case OP_SED:
        cpu->status_D=1;
        break;
      case OP_SEI:
        cpu->status_I=1;
        break;
      case OP_STA:
        cpu->write(cpu->reference, address, cpu->reg_A);
        break;
      case OP_STX:
        cpu->write(cpu->reference, address, cpu->reg_X);
        break;
      case OP_STY:
        cpu->write(cpu->reference, address, cpu->reg_Y);
        break;
      case OP_TAX:
        cpu->reg_X=cpu->reg_A;
        RECALC_ZN(cpu->reg_X);
        break;
      case OP_TAY:
        cpu->reg_Y=cpu->reg_A;
        RECALC_ZN(cpu->reg_Y);
        break;
      case OP_TSX:
        cpu->reg_X=cpu->reg_SP;
        RECALC_ZN(cpu->reg_X);
        break;
      case OP_TXA:
        cpu->reg_A=cpu->reg_X;
        RECALC_ZN(cpu->reg_A);
        break;
      case OP_TXS:
        cpu->reg_SP=cpu->reg_X;
        break;
      case OP_TYA:
        cpu->reg_A=cpu->reg_Y;
        RECALC_ZN(cpu->reg_A);
        break;
      // undocumented opcodes
      case OP_AHX:
      case OP_ALR:
      case OP_ANC:
      case OP_ARR:
      case OP_AXS:
      case OP_DCP:
      case OP_ISC:
      case OP_KIL:
      case OP_LAS:
      case OP_LAX:
      case OP_RLA:
      case OP_RRA:
      case OP_SAX:
      case OP_SHX:
      case OP_SHY:
      case OP_SLO:
      case OP_SRE:
      case OP_TAS:
      case OP_XAA:
        break;
    }

    cycles_to_run-=cycles_passed;
    cpu->cycle_number+=cycles_passed;
  } while (cycles_to_run>0);

  return cycles_passed;
}

void cpu6502_trigger_interrupt(cpu6502_t *cpu, cpu6502_interrupt_enum_t interrupt) {
  // TODO: could it happen that NMI and IRQ occuring the same time?
  switch (interrupt) {
    case INTERRUPT_NMI:
      cpu->interrupt_pending=INTERRUPT_NMI;
      break;
    case INTERRUPT_IRQ:
      if (cpu->status_I) {
        cpu->interrupt_pending=INTERRUPT_NMI;
      }
      break;
    default:
      break;
  }
}
