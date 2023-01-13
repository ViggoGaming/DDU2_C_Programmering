#define F_CPU 20000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#define Sine_Resolution = 2048

#define PWM_Cycle_Length = 100

#define freqPerIndex 97.65625F // 200k / 2048 = 97.65625

#define testDuration 5000

#define EOT ':'

#define EOL ';'

#define BAUDRATEVAL 9600

#define BIT(x) (1 << (x))

#define SETBITS(x, y) ((x) |= (y))

#define CLEARBITS(x, y) ((x) &= (~(y)))

#define SETBIT(x, y) SETBITS((x), (BIT((y))))

#define CLEARBIT(x, y) CLEARBITS((x), (BIT((y))))

#define BITVAL(x, y) (((x) >> (y)) & 1)

const uint8_t sine100_2048[] PROGMEM = {

    0x32, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34,
    0x34, 0x34, 0x34, 0x34, 0x34, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39,

    0x39, 0x39, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x3b, 0x3b, 0x3b, 0x3b,
    0x3b, 0x3b, 0x3b, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3d, 0x3d, 0x3d,
    0x3d, 0x3d, 0x3d, 0x3d, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

    0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42, 0x42,
    0x42, 0x42, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44,
    0x44, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x46,
    0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47,

    0x47, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x49, 0x49, 0x49,
    0x49, 0x49, 0x49, 0x49, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4b,
    0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c,
    0x4c, 0x4c, 0x4c, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x4e, 0x4e,

    0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f,
    0x4f, 0x4f, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x51, 0x51,
    0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x52, 0x52, 0x52, 0x52, 0x52,
    0x52, 0x52, 0x52, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,

    0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56,
    0x56, 0x56, 0x56, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x59,

    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x5a, 0x5a, 0x5a,
    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5b, 0x5b, 0x5b, 0x5b,
    0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5c, 0x5c, 0x5c, 0x5c,
    0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5d, 0x5d, 0x5d, 0x5d,

    0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5e, 0x5e, 0x5e, 0x5e,
    0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5f, 0x5f,
    0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f,
    0x5f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,

    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
    0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
    0x61, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
    0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,

    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,

    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,

    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,

    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
    0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
    0x62, 0x62, 0x62, 0x62, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,

    0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f,
    0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e,

    0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5d, 0x5d, 0x5d,
    0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5d, 0x5c, 0x5c, 0x5c,
    0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5b, 0x5b, 0x5b,
    0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5a, 0x5a, 0x5a,

    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x59, 0x59, 0x59, 0x59,
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
    0x58, 0x58, 0x58, 0x58, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x55,

    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x54, 0x54, 0x54, 0x54,
    0x54, 0x54, 0x54, 0x54, 0x54, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
    0x53, 0x53, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x51, 0x51,
    0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x50, 0x50, 0x50, 0x50, 0x50,

    0x50, 0x50, 0x50, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4e,
    0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d,
    0x4d, 0x4d, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4b, 0x4b,
    0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a,

    0x4a, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x46,
    0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45,
    0x45, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x43, 0x43, 0x43, 0x43,

    0x43, 0x43, 0x43, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x41, 0x41,
    0x41, 0x41, 0x41, 0x41, 0x41, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e,
    0x3e, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3c, 0x3c, 0x3c, 0x3c,

    0x3c, 0x3c, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3a, 0x3a, 0x3a,
    0x3a, 0x3a, 0x3a, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x38, 0x38,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x36,
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,

    0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x31, 0x31, 0x31, 0x31,
    0x31, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x2f, 0x2f, 0x2f,
    0x2f, 0x2f, 0x2f, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2d, 0x2d,

    0x2d, 0x2d, 0x2d, 0x2d, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2b,
    0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x26, 0x26, 0x26, 0x26,

    0x26, 0x26, 0x26, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x24, 0x24,
    0x24, 0x24, 0x24, 0x24, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x22,
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
    0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x1f, 0x1f, 0x1f,

    0x1f, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1d, 0x1d,
    0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
    0x1c, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1a, 0x1a, 0x1a, 0x1a,
    0x1a, 0x1a, 0x1a, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x18,

    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x17, 0x17, 0x17, 0x17, 0x17,
    0x17, 0x17, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x12,

    0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xe,  0xe,
    0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,  0xd,  0xd,  0xd,  0xd,  0xd,

    0xd,  0xd,  0xd,  0xd,  0xd,  0xc,  0xc,  0xc,  0xc,  0xc,  0xc,  0xc,
    0xc,  0xc,  0xc,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,
    0xb,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,
    0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,

    0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
    0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,
    0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,
    0x6,  0x6,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,

    0x5,  0x5,  0x5,  0x5,  0x5,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,
    0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x3,  0x3,
    0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,
    0x3,  0x3,  0x3,  0x3,  0x3,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,

    0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,
    0x2,  0x2,  0x2,  0x2,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,
    0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,
    0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,

    0x1,  0x1,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,

    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,

    0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,
    0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,
    0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x2,  0x2,  0x2,
    0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,

    0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x3,  0x3,  0x3,  0x3,
    0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,
    0x3,  0x3,  0x3,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,
    0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x5,  0x5,  0x5,  0x5,

    0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x6,
    0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,
    0x6,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,
    0x7,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,

    0x8,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,
    0x9,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,  0xa,
    0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xb,  0xc,  0xc,
    0xc,  0xc,  0xc,  0xc,  0xc,  0xc,  0xc,  0xc,  0xd,  0xd,  0xd,  0xd,

    0xd,  0xd,  0xd,  0xd,  0xd,  0xd,  0xe,  0xe,  0xe,  0xe,  0xe,  0xe,
    0xe,  0xe,  0xe,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,  0xf,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,

    0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x17,
    0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,

    0x18, 0x18, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x1a, 0x1a,
    0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1d, 0x1d, 0x1d, 0x1d,
    0x1d, 0x1d, 0x1d, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f,

    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24,
    0x24, 0x24, 0x24, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x26, 0x26,

    0x26, 0x26, 0x26, 0x26, 0x26, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29,
    0x29, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b,
    0x2b, 0x2b, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2d, 0x2d, 0x2d,

    0x2d, 0x2d, 0x2d, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2f, 0x2f,
    0x2f, 0x2f, 0x2f, 0x2f, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31,
    0x31, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32};

volatile unsigned long toneTimer;

uint16_t toneDuration;

uint16_t reactionTime;

bool pressedButton;

bool tonePlaying;

volatile uint16_t sineIndex = 0;

volatile uint8_t indexIncrement = 1;

volatile char uartBuffer[32]; // 32 characters

volatile uint8_t bufferIndex = 0;

volatile uint8_t readIndex = 0;

volatile bool byteReceived = false;

// Array of frequencies to use in the full test

int test_frequencies[8] = {100, 200, 400, 800, 1600, 3200, 6400, 10000};

void setLight(bool);

ISR(USART_RX_vect) {
  uartBuffer[bufferIndex] = UDR0;
  bufferIndex++;
  // Set byteRecieved flag
  byteReceived = true;
}

// Interrupts when timer0 reaches TOP = 8-bit/255

ISR(TIMER0_COMPA_vect) { toneTimer++; }

// Interupt when timer1 reaches TOP = ICR1, every PWM cycle.

ISR(TIMER1_CAPT_vect) {
  sineIndex += indexIncrement;
  if (sineIndex >= 2047) {
    sineIndex -= 2047;
  }
  OCR1A = pgm_read_byte(&sine100_2048[sineIndex]);
}

void initUart(void) {
  uint16_t ubrr0 = ((F_CPU) / (BAUDRATEVAL * 16UL) - 1);
  UBRR0H = (ubrr0 >> 8);
  UBRR0L = (ubrr0);
  UCSR0B = (1 << TXEN0) | (1 << RXEN0) |
           (1 << RXCIE0); // Transmit enabled, Recieve enabled, Recieve interupt
                          // enabled.
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // 8-bit, no parity, one stop bit
}

void txByte(char data) {
  while (!(UCSR0A & (1 << UDRE0)))
    ; // Wait while register is free
  UDR0 = data;
}

void txLine(char data[]) {
  uint8_t i = 0;
  // Transmit chars one by one
  while (data[i] != 0) {
    txByte(data[i]);
    i++;
  }
}

void txUint(uint16_t n) {
  char s[16];
  utoa(n, s, 10);
  txLine(s);
}

unsigned char rxByte(void) {
  while (!(UCSR0A & (1 << RXC0)))
    ; // Wait while register is free
  return UDR0;
}

bool bufferContains(char val[]) {
  uint8_t i = 0;
  while (val[i]) {
    if (val[i] != uartBuffer[i + readIndex]) {
      return false;
    }
    i++;
  }
  // if every char of val matches with buffer, update readIndex and return true
  readIndex += i + 1; // plus 1 to include space
  return true;
}

// '0' = 48, '1' = 49, '2' = 50, '3' = 51...

uint16_t getBufferDecimal() {
  uint8_t i = 0;
  uint16_t num = 0;
  while (uartBuffer[i + readIndex] != ' ' &&
         uartBuffer[i + readIndex] != EOL) { // Terminate at space of EOL
    // Convert utf-8 to digit
    uint8_t digit = (uint8_t)uartBuffer[i + readIndex] - 48;
    if (digit >= 0 && digit <= 9) {
      num *= 10;
      num += (uint16_t)digit;
    } else {
      // ignore
    }
    i++;
  }
  readIndex += i + 1;
  return num;
}

void setLight(bool on) {
  if (on) {
    SETBIT(PORTC, PC3);
    SETBIT(PORTC, PC2);
  } else {
    CLEARBIT(PORTC, PC3);
    CLEARBIT(PORTC, PC2);
  }
}

uint16_t roundedDivision(uint16_t a, uint16_t b) {
  return a / b + (a % b > b / 2);
}

uint16_t setToneFreq(uint16_t f) {
  indexIncrement = roundedDivision(f, freqPerIndex);
  // Return actual frequency
  return (uint16_t)indexIncrement * freqPerIndex;
}

void setSound(bool on) {
  // Update global variable
  tonePlaying = on;
  if (on) {
    // Turn on PWM signal
    SETBIT(DDRB, PB1);
    // Reset duration timer
    toneTimer = 0;
    // Turn on lights
    setLight(true);
  } else {
    // Turn off PWM signal
    CLEARBIT(DDRB, PB1);
    // Turn off light
    setLight(false);
  }
}

void setEar(bool right) {
  if (right) { // Turn on ONLY right ear
    CLEARBIT(PORTD, PD7);
    SETBIT(PORTB, PB0);
  } else { // Turn on ONLY left ear
    SETBIT(PORTD, PD7);
    CLEARBIT(PORTB, PB0);
  }
}

// bool pLeftIsPressed = false;

// bool pRightIsPressed = false;

void update() {
  if (tonePlaying) {
    // Buttons are pulled up, and 0 when pressed.
    bool leftIsPressed = !BITVAL(PINC, PC4);
    bool rightIsPressed = !BITVAL(PINC, PC5);

    // Check button input
    if (leftIsPressed || rightIsPressed) {
      reactionTime = toneTimer;
      setSound(false);
      // Variable that contains which button is pressed, if the right isn't
      // pressed, the left is.
      pressedButton = rightIsPressed;
    }
    // Check timeout
    if (toneTimer > toneDuration) {
      setSound(false);
    }

    // Update previous-values
    // pLeftIsPressed = leftIsPressed;
    // pRightIsPressed = pRightIsPressed;
  }
}

void setupTimer0() {
  // Timer0 counts milliseconds. Not EXACT milliseconds, but close enough for
  // our use case. Creates interrupt with following frequency:
  // 1001.6025641025641 Hz (20000000/((77+1)*256))
  OCR0A = 77;
  // CTC
  TCCR0A = (1 << WGM01);
  // Prescaler 256
  TCCR0B = (1 << CS02);
  // Output Compare Match A Interrupt Enable
  TIMSK0 = (1 << OCIE0A);
}

void setupTimer1() {
  // Fast PWM, F_CPU / 100 = 200 kHz
  ICR1 = 100; // 0-99 resolution
  // Output compare OC1A 8 bit non inverted PWM
  TCCR1A = (1 << COM1A1) | (1 << WGM11);
  // Interupt when timer1 reaches TOP =  ICR1
  TIMSK1 = (1 << ICIE1);
  // start timer without prescaler
  TCCR1B = (1 << CS10) | (1 << WGM12) | (1 << WGM13);
}

int main(void) {
  // UART SETUP
  initUart();

  // Sets PB1 as output for transmitting sound and PB0 as output for turning on
  // RIGHT ear
  DDRB = (1 << PB1) | (1 << PB0);
  // Sets PC2 and PC3 as output to control the arcade button led
  DDRC = (1 << PC3) | (1 << PC2);
  // PC4 and PC5 are input for the buttons. PC4 is left, PC5 is right.
  // Sets PD1 as output for transmitting UART data, PD7 as output for turning on
  // LEFT ear.
  DDRD = (1 << PD1) | (1 << PD7);

  // Setup timers
  setupTimer0();
  setupTimer1();

  // Right ear on as standard
  // setEar(true);
  // setSound(false);
  // enable global interrups
  sei();

  // Main loop
  while (1) {
    // Listen and update
    bool listening = true;
    while (listening) { // Listen until EOL character
      update();
      if (byteReceived) { // When we recieve a byte
        byteReceived = false;
        if (uartBuffer[bufferIndex - 1] == EOL) {   // If its EOL
          if (uartBuffer[bufferIndex - 2] == EOT) { // If its EOT
            // Exit listening loop and respond
            listening = false;
          }
        }
      }
    }
    bufferIndex = 0;
    // React and respond
    readIndex = 0;
    bool inputError = false;
    if (bufferContains("lights")) { // LIGHTS [On / Off]
      if (bufferContains("on")) {
        setLight(true);
        txLine("Button Lights ON");
      } else {
        setLight(false);
        txLine("Button Lights OFF");
      }
      txByte(EOT);
      txByte(EOL);
    } else if (bufferContains("tone")) { // TONE [Duration] [Frequincy]
      uint16_t duration = getBufferDecimal();
      uint16_t targetFreq = getBufferDecimal();
      // Set tone frequency and duration.
      uint16_t actualFreq = setToneFreq(targetFreq);
      toneDuration = duration;
      setSound(true);
      char duration_s[16];
      utoa(duration, duration_s, 10);
      char freq_s[16];
      utoa(actualFreq, freq_s, 10);
      txLine("Play tone for ");
      txLine(duration_s);
      txLine(" ms with a frequency of ");
      txLine(freq_s);
      txLine(" Hz");
      txByte(EOT);
      txByte(EOL);
    } else if (bufferContains("setear")) { // SETEAR [Left / Right]
      if (bufferContains("right")) {
        setEar(true);
        txLine("Turned on RIGHT ear");
      } else {
        setEar(false);
        txLine("Turned on LEFT ear");
      }
      txByte(EOT);
      txByte(EOL);
    } else if (bufferContains("test")) { // TEST [Left / Right] [Frequency]
      // Get target ear and target frequecy
      bool isEarRight = false;
      if (bufferContains("right")) {
        isEarRight = true;
      } else if (!bufferContains("left")) {
        inputError = true;
      }
      if (!inputError) {
        uint16_t targetFreq = getBufferDecimal();
        // Set ear and frequency.
        setEar(isEarRight);
        uint16_t actualFreq = setToneFreq(targetFreq);
        // Test Duration
        toneDuration = testDuration;
        // Make string for frequency
        char freq_s[16];
        utoa(actualFreq, freq_s, 10);
        // Respond
        txLine("Testing ");
        txLine(isEarRight ? "RIGHT" : "LEFT");
        txLine(" ear with a frequncy of ");
        txLine(freq_s);
        txLine(" Hz. Waiting for test input...");
        txByte(EOL);
        // Start test
        // Play sound by turning on PWM output
        reactionTime = 0;
        setSound(true);
        while (tonePlaying) {
          update();
        }
        // Return results
        if (toneTimer > toneDuration) {
          txLine("No input received");
        } else {
          txLine(pressedButton ? "RIGHT" : "LEFT");
          txLine(" button pressed after ");
          txUint(reactionTime);
          txLine(" ms.");
        }
        txByte(EOT);
        txByte(EOL);
      } else {
        txLine("Input Error");
        txByte(EOT);
        txByte(EOL);
      }
    } else {
      txLine("Unkown command");
      txByte(EOT);
      txByte(EOL);
    }
  }
}
