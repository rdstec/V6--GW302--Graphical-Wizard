// -----------------------------------------------------------
// Graphic Font Size 1 - IS08859 common for all character sets
// C Source File:    bm_f1all.c    
// -----------------------------------------------------------


// Include Standard files
#include "Board.h"


/*
struct fn1 {
       unsigned char spc;
       const unsigned char *f1[256];
       };
extern struct fn1 const fnt1_EUR;
extern struct fn1 const fnt1_CYR;
extern struct fn1 const * const fnt1[MAXLANGUAGES];
*/

struct fn1 const * const fnt1[MAXLANGUAGES] =
  {
  (struct fn1 *)&fnt1_EUR, /* - bm_f1eur.c */
  (struct fn1 *)&fnt1_CYR  /* - bm_f1cyr.c */
  };

/* special characters */
const unsigned char F1_FILL[9] = { 5, 7, 0x00,
  0x70,
  0x70,
  0x70,
  0x70,
  0x70,
  0x00 };

const unsigned char F1_ENT[8] = { 6, 6, 0x00, /* 1, 0x1, \001 octal */
  0x24,
  0x44,
  0xfc,
  0x40,
  0x20
  };

const unsigned char F1_DEG[9] = { 5, 7, 0x60, /* 2, 0x2, \002 octal */
  0x90,
  0x90,
  0x60,
  0x00,
  0x00,
  0x00
  };

const unsigned char F1_RDS[47] = { 17, 15, 0xff, 0xff, 0x80,  /* 3, 0x3, \003 octal */
  0xc0, 0x01, 0x80,
  0x00, 0x00, 0x00,
  0xf3, 0xc7, 0x80,
  0xfb, 0xef, 0x80,
  0xdb, 0x6c, 0x00,
  0xf3, 0x6f, 0x80,
  0xf3, 0x61, 0x80,
  0xdb, 0xef, 0x80,
  0xdb, 0xcf, 0x00,
  0x00, 0x00, 0x00,
  0xc0, 0x01, 0x80,
  0xf8, 0x0f, 0x80,
  0x1f, 0xfc, 0x00,
  0x01, 0xc0, 0x00
  };

const unsigned char F1_CLK[9] = { 7, 7, 0x38, 
  0x44,
  0x92,
  0x9a,
  0x82,
  0x44,
  0x38
  };

const unsigned char F1_POINT[9] = { 5, 7, 0x80, 
  0xc0,
  0xf0,
  0xf8,
  0xf0,
  0xc0,
  0x80
  };

/* standard characters for ISO8859 */
const unsigned char F1_I120[9] = { 5, 7, 0x00,
 0x00,
 0x00,
 0x00,
 0x00,
 0x00,
 0x00 };

const unsigned char F1_I121[9] = { 5, 7, 0x20,
 0x20,
 0x20,
 0x20,
 0x00,
 0x00,
 0x20 };

const unsigned char F1_I122[9] = { 5, 7, 0x50,
 0x50,
 0x50,
 0x00,
 0x00,
 0x00,
 0x00 };

const unsigned char F1_I123[9] = { 5, 7, 0x50,
 0x50,
 0xf8,
 0x50,
 0xf8,
 0x50,
 0x50 };

const unsigned char F1_I124[9] = { 5, 7, 0x20,
 0x78,
 0xa0,
 0x70,
 0x28,
 0xf0,
 0x20 };

const unsigned char F1_I125[9] = { 5, 7, 0xc0,
 0xc8,
 0x10,
 0x20,
 0x40,
 0x98,
 0x18 };

const unsigned char F1_I126[9] = { 5, 7, 0x60,
 0x90,
 0xa0,
 0x40,
 0xa8,
 0x90,
 0x68 };

const unsigned char F1_I127[9] = { 5, 7, 0x30,
 0x10,
 0x20,
 0x00,
 0x00,
 0x00,
 0x00 };

const unsigned char F1_I128[9] = { 3, 7, 0x20,
 0x40,
 0x80,
 0x80,
 0x80,
 0x40,
 0x20 };

const unsigned char F1_I129[9] = { 3, 7, 0x80,
 0x40,
 0x20,
 0x20,
 0x20,
 0x40,
 0x80 };

const unsigned char F1_I12A[9] = { 5, 7, 0x00,
 0x20,
 0xa8,
 0x70,
 0xa8,
 0x20,
 0x00 };

const unsigned char F1_I12B[9] = { 5, 7, 0x00,
 0x20,
 0x20,
 0xf8,
 0x20,
 0x20,
 0x00 };

const unsigned char F1_I12C[9] = { 5, 7, 0x00,
 0x00,
 0x00,
 0x00,
 0x60,
 0x20,
 0x40 };

const unsigned char F1_I12D[9] = { 5, 7, 0x00,
 0x00,
 0x00,
 0xf8,
 0x00,
 0x00,
 0x00 };

const unsigned char F1_I12E[9] = { 2, 7, 0x00,
 0x00,
 0x00,
 0x00,
 0x00,
 0xc0,
 0xc0 };

const unsigned char F1_I12F[9] = { 5, 7, 0x00,
 0x08,
 0x10,
 0x20,
 0x40,
 0x80,
 0x00 };

const unsigned char F1_I130[9] = { 5, 7, 0x70,
 0x88,
 0x98,
 0xa8,
 0xc8,
 0x88,
 0x70 };

const unsigned char F1_I131[9] = { 5, 7, 0x20,
 0x60,
 0x20,
 0x20,
 0x20,
 0x20,
 0x70 };

const unsigned char F1_I132[9] = { 5, 7, 0x70,
 0x88,
 0x08,
 0x10,
 0x20,
 0x40,
 0xf8 };

const unsigned char F1_I133[9] = { 5, 7, 0xf8,
 0x10,
 0x20,
 0x10,
 0x08,
 0x88,
 0x70 };

const unsigned char F1_I134[9] = { 5, 7, 0x10,
 0x30,
 0x50,
 0x90,
 0xf8,
 0x10,
 0x10 };

const unsigned char F1_I135[9] = { 5, 7, 0xf8,
 0x80,
 0xf0,
 0x08,
 0x08,
 0x88,
 0x70 };

const unsigned char F1_I136[9] = { 5, 7, 0x30,
 0x40,
 0x80,
 0xf0,
 0x88,
 0x88,
 0x70 };

const unsigned char F1_I137[9] = { 5, 7, 0xf8,
 0x08,
 0x10,
 0x20,
 0x20,
 0x20,
 0x20 };

const unsigned char F1_I138[9] = { 5, 7, 0x70,
 0x88,
 0x88,
 0x70,
 0x88,
 0x88,
 0x70 };

const unsigned char F1_I139[9] = { 5, 7, 0x70,
 0x88,
 0x88,
 0x78,
 0x08,
 0x10,
 0x60 };

const unsigned char F1_I13A[9] = { 2, 7, 0x00,
 0xc0,
 0xc0,
 0x00,
 0xc0,
 0xc0,
 0x00 };

const unsigned char F1_I13B[9] = { 5, 7, 0x00,
 0x60,
 0x60,
 0x00,
 0x60,
 0x20,
 0x40 };

const unsigned char F1_I13C[9] = { 4, 7, 0x10,
 0x20,
 0x40,
 0x80,
 0x40,
 0x20,
 0x10 };

const unsigned char F1_I13D[9] = { 5, 7, 0x00,
 0x00,
 0xf8,
 0x00,
 0xf8,
 0x00,
 0x00 };

const unsigned char F1_I13E[9] = { 4, 7, 0x80,
 0x40,
 0x20,
 0x10,
 0x20,
 0x40,
 0x80 };

const unsigned char F1_I13F[9] = { 5, 7, 0x70,
 0x88,
 0x08,
 0x10,
 0x20,
 0x00,
 0x20 };

const unsigned char F1_I140[9] = { 5, 7, 0x70,
 0x88,
 0x08,
 0x68,
 0xa8,
 0xa8,
 0x70 };

const unsigned char F1_I141[9] = { 5, 7, 0x70,
 0x88,
 0x88,
 0x88,
 0xf8,
 0x88,
 0x88 };

const unsigned char F1_I142[9] = { 5, 7, 0xf0,
 0x48,
 0x48,
 0x70,
 0x48,
 0x48,
 0xf0 };

const unsigned char F1_I143[9] = { 5, 7, 0x70,
 0x88,
 0x80,
 0x80,
 0x80,
 0x88,
 0x70 };

const unsigned char F1_I144[9] = { 5, 7, 0xf0,
 0x48,
 0x48,
 0x48,
 0x48,
 0x48,
 0xf0 };

const unsigned char F1_I145[9] = { 5, 7, 0xf8,
 0x80,
 0x80,
 0xf8,
 0x80,
 0x80,
 0xf8 };

const unsigned char F1_I146[9] = { 5, 7, 0xf8,
 0x80,
 0x80,
 0xf0,
 0x80,
 0x80,
 0x80 };

const unsigned char F1_I147[9] = { 5, 7, 0x70,
 0x88,
 0x80,
 0x98,
 0x88,
 0x88,
 0x78 };

const unsigned char F1_I148[9] = { 5, 7, 0x88,
 0x88,
 0x88,
 0xf8,
 0x88,
 0x88,
 0x88 };

const unsigned char F1_I149[9] = { 5, 7, 0x70,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20,
 0x70 };

const unsigned char F1_I14A[9] = { 5, 7, 0x38,
 0x10,
 0x10,
 0x10,
 0x10,
 0x90,
 0x60 };

const unsigned char F1_I14B[9] = { 5, 7, 0x88,
 0x90,
 0xa0,
 0xc0,
 0xa0,
 0x90,
 0x88 };

const unsigned char F1_I14C[9] = { 5, 7, 0x80,
 0x80,
 0x80,
 0x80,
 0x80,
 0x80,
 0xf8 };

const unsigned char F1_I14D[9] = { 5, 7, 0x88,
 0xd8,
 0xa8,
 0xa8,
 0x88,
 0x88,
 0x88 };

const unsigned char F1_I14E[9] = { 5, 7, 0x88,
 0xc8,
 0xc8,
 0xa8,
 0x98,
 0x98,
 0x88 };

const unsigned char F1_I14F[9] = { 5, 7, 0x70,
 0x88,
 0x88,
 0x88,
 0x88,
 0x88,
 0x70 };

const unsigned char F1_I150[9] = { 5, 7, 0xf0,
 0x88,
 0x88,
 0xf0,
 0x80,
 0x80,
 0x80 };

const unsigned char F1_I151[9] = { 5, 7, 0x70,
 0x88,
 0x88,
 0x88,
 0xa8,
 0x90,
 0x68 };

const unsigned char F1_I152[9] = { 5, 7, 0xf0,
 0x88,
 0x88,
 0xf0,
 0xa0,
 0x90,
 0x88 };

const unsigned char F1_I153[9] = { 5, 7, 0x70,
 0x88,
 0x80,
 0x70,
 0x08,
 0x88,
 0x70 };

const unsigned char F1_I154[9] = { 5, 7, 0xf8,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20 };

const unsigned char F1_I155[9] = { 5, 7, 0x88,
 0x88,
 0x88,
 0x88,
 0x88,
 0x88,
 0x70 };

const unsigned char F1_I156[9] = { 5, 7, 0x88,
 0x88,
 0x88,
 0x88,
 0x88,
 0x50,
 0x20 };

const unsigned char F1_I157[9] = { 5, 7, 0x88,
 0x88,
 0x88,
 0xa8,
 0xa8,
 0xd8,
 0x88 };

const unsigned char F1_I158[9] = { 5, 7, 0x88,
 0x88,
 0x50,
 0x20,
 0x50,
 0x88,
 0x88 };

const unsigned char F1_I159[9] = { 5, 7, 0x88,
 0x88,
 0x88,
 0x50,
 0x20,
 0x20,
 0x20 };

const unsigned char F1_I15A[9] = { 5, 7, 0xf8,
 0x08,
 0x10,
 0x20,
 0x40,
 0x80,
 0xf8 };

const unsigned char F1_I15B[9] = { 3, 7, 0xe0,
 0x80,
 0x80,
 0x80,
 0x80,
 0x80,
 0xe0 };

const unsigned char F1_I15C[9] = { 5, 7, 0x00,
 0x80,
 0x40,
 0x20,
 0x10,
 0x08,
 0x00 };

const unsigned char F1_I15D[9] = { 3, 7, 0xe0,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20,
 0xe0 };

const unsigned char F1_I15E[9] = { 5, 7, 0x20,
 0x50,
 0x88,
 0x00,
 0x00,
 0x00,
 0x00 };

const unsigned char F1_I15F[9] = { 5, 7, 0x00,
 0x00,
 0x00,
 0x00,
 0x00,
 0x00,
 0xf8 };

const unsigned char F1_I160[9] = { 4, 7, 0x80,
 0x40,
 0x20,
 0x00,
 0x00,
 0x00,
 0x00 };

const unsigned char F1_I161[9] = { 5, 7, 0x00,
 0x00,
 0x70,
 0x08,
 0x78,
 0x88,
 0x78 };

const unsigned char F1_I162[9] = { 5, 7, 0x80,
 0x80,
 0xb0,
 0xc8,
 0x88,
 0x88,
 0xf0 };

const unsigned char F1_I163[9] = { 5, 7, 0x00,
 0x00,
 0x70,
 0x88,
 0x80,
 0x88,
 0x70 };

const unsigned char F1_I164[9] = { 5, 7, 0x08,
 0x08,
 0x68,
 0x98,
 0x88,
 0x88,
 0x78 };

const unsigned char F1_I165[9] = { 5, 7, 0x00,
 0x00,
 0x70,
 0x88,
 0xf8,
 0x80,
 0x70 };

const unsigned char F1_I166[9] = { 5, 7, 0x10,
 0x28,
 0x20,
 0x70,
 0x20,
 0x20,
 0x20 };

const unsigned char F1_I167[9] = { 5, 7, 0x00,
 0x68,
 0x98,
 0x98,
 0x68,
 0x08,
 0x70 };

const unsigned char F1_I168[9] = { 5, 7, 0x80,
 0x80,
 0xb0,
 0xc8,
 0x88,
 0x88,
 0x88 };

const unsigned char F1_I169[9] = { 5, 7, 0x20,
 0x00,
 0x60,
 0x20,
 0x20,
 0x20,
 0x70 };

const unsigned char F1_I16A[9] = { 5, 7, 0x10,
 0x00,
 0x30,
 0x10,
 0x10,
 0x90,
 0x60 };

const unsigned char F1_I16B[9] = { 5, 7, 0x40,
 0x40,
 0x48,
 0x50,
 0x60,
 0x50,
 0x48 };

const unsigned char F1_I16C[9] = { 5, 7, 0x60,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20,
 0x70 };

const unsigned char F1_I16D[9] = { 5, 7, 0x00,
 0x00,
 0xd0,
 0xa8,
 0xa8,
 0xa8,
 0xa8 };

const unsigned char F1_I16E[9] = { 5, 7, 0x00,
 0x00,
 0xb0,
 0xc8,
 0x88,
 0x88,
 0x88 };

const unsigned char F1_I16F[9] = { 5, 7, 0x00,
 0x00,
 0x70,
 0x88,
 0x88,
 0x88,
 0x70 };

const unsigned char F1_I170[9] = { 5, 7, 0x00,
 0xb0,
 0xc8,
 0xc8,
 0xb0,
 0x80,
 0x80 };

const unsigned char F1_I171[9] = { 5, 7, 0x00,
 0x68,
 0x98,
 0x98,
 0x68,
 0x08,
 0x08 };

const unsigned char F1_I172[9] = { 5, 7, 0x00,
 0x00,
 0xb0,
 0xc8,
 0x80,
 0x80,
 0x80 };

const unsigned char F1_I173[9] = { 5, 7, 0x00,
 0x00,
 0x78,
 0x80,
 0xf0,
 0x08,
 0xf8 };

const unsigned char F1_I174[9] = { 5, 7, 0x40,
 0x40,
 0xe0,
 0x40,
 0x40,
 0x48,
 0x30 };

const unsigned char F1_I175[9] = { 5, 7, 0x00,
 0x00,
 0x90,
 0x90,
 0x90,
 0x90,
 0x68 };

const unsigned char F1_I176[9] = { 5, 7, 0x00,
 0x00,
 0x88,
 0x88,
 0x88,
 0x50,
 0x20 };

const unsigned char F1_I177[9] = { 5, 7, 0x00,
 0x00,
 0x88,
 0x88,
 0xa8,
 0xa8,
 0x50 };

const unsigned char F1_I178[9] = { 5, 7, 0x00,
 0x00,
 0x88,
 0x50,
 0x20,
 0x50,
 0x88 };

const unsigned char F1_I179[9] = { 5, 7, 0x00,
 0x88,
 0x88,
 0x98,
 0x68,
 0x08,
 0x70 };

const unsigned char F1_I17A[9] = { 5, 7, 0x00,
 0x00,
 0xf8,
 0x10,
 0x20,
 0x40,
 0xf8 };

const unsigned char F1_I17B[9] = { 5, 7, 0x10,
 0x20,
 0x20,
 0x40,
 0x20,
 0x20,
 0x10 };

const unsigned char F1_I17C[9] = { 5, 7, 0x20,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20,
 0x20 };

const unsigned char F1_I17D[9] = { 5, 7, 0x40,
 0x20,
 0x20,
 0x10,
 0x20,
 0x20,
 0x40 };

const unsigned char F1_I17E[9] = { 5, 7, 0x40,
 0xa8,
 0x10,
 0x00,
 0x00,
 0x00,
 0x00 };