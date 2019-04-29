/*------------------------------------------------------------------------------
//*-         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*------------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*-----------------------------------------------------------------------------
//*- File source          : Cstartup.s
//*- Object               : Generic CStartup for KEIL and GCC No Use REMAP
//*- Compilation flag     : None
//*-
//*- 1.0 10/Mar/05 JPP    : Creation
//*- 1.1 01/Apr/05 JPP    : save SPSR 
//*-----------------------------------------------------------------------------*/

#;------------------------------------------------------------------------------
#;- Section Definition
#;-----------------
#;-  Section    
#;- .internal_ram_top 	Top_Stack: used by the cstartup for vector initalisation 
#;-			management defined by ld and affect from ldscript 
#;------------------------------------------------------------------------------
	.section .internal_ram_top
	.code 32
	.align 	0
	.global	Top_Stack
Top_Stack:
	
/*-----------------------------------------------------------------------------
  Define RAM Vector Table 
------------------------------------------------------------------------------*/
	.text
	.arm
	.section .vectRAM, "ax"

reset_RAM:/* 0x00 Reset handler */
	LDR		PC, reset_RAM_add        
undef_RAM:	/* 0x04 Undefined Instruction */
	LDR		PC, undef_RAM_add
swint_RAM:	/* 0x08 Software Interrupt */
	LDR		PC, swint_RAM_add
preab_RAM:	/* 0x0C Prefetch Abort */
	LDR		PC, preab_RAM_add
datab_RAM:	/* 0x10 Data Abort */
	LDR		PC, datab_RAM_add
rsrvd_RAM:	/* 0x14 Reserved */
	LDR		PC,	rsrvd_RAM_add
irqvc_RAM:	/* 0x18 IRQ - Jump directly to the address given by the AIC */
	LDR		PC,[PC, #-0xF20]
fiqvc_RAM:	/* 0x1c FIQ	*/
	/* Switch in SVC/User Mode to allow User Stack access for C code */
	/*  because the FIQ is not yet acknowledged*/
	/* Save and r0 in FIQ_Register */
	mov		r9,r0
	ldr		r0 , [r8, #AIC_FVR]
	msr		CPSR_c,#I_BIT | F_BIT | ARM_MODE_SVC

	/* Save scratch/used registers and LR in User Stack */
	stmfd	sp!, { r1-r3, r12, lr}

	/* Branch to the routine pointed by the AIC_FVR */
	mov		r14, pc
	bx		r0

	/*- Restore scratch/used registers and LR from User Stack */
	ldmia	sp!, { r1-r3, r12, lr}

	/*- Leave Interrupts disabled and switch back in FIQ mode */
	msr		CPSR_c, #I_BIT | F_BIT | ARM_MODE_FIQ

	/*- Restore the R0 ARM_MODE_SVC register */
	mov		r0,r9

	/*- Restore the Program Counter using the LR_fiq directly in the PC */
	subs	pc,lr,#4
	/* end of FIQ_Handler_Entry */

/* fix up external exception vector addresses */	
reset_RAM_add:	.word	InitReset         
undef_RAM_add:	.word	AT91F_Undefined_Instruction_handler
swint_RAM_add:	.word	vPortYieldProcessor
preab_RAM_add:	.word	AT91F_Prefetch_Abort_handler
datab_RAM_add:	.word	AT91F_Data_Abort_handler
rsrvd_RAM_add:	.word	AT91F_Reserved_handler
	
/* end of RAM vector table */
/*------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  Define ROM Vector Table 
------------------------------------------------------------------------------*/
	.text
	.arm
	.section .vectROM, "ax"

reset_ROM:/* 0x00 Reset handler */
	LDR		PC, reset_ROM_add        
undef_ROM:	/* 0x04 Undefined Instruction */
	LDR		PC, undef_ROM_add
swint_ROM:	/* 0x08 Software Interrupt */
	LDR		PC, swint_ROM_add
preab_ROM:	/* 0x0C Prefetch Abort */
	LDR		PC, preab_ROM_add
datab_ROM:	/* 0x10 Data Abort */
	LDR		PC, datab_ROM_add
rsrvd_ROM:	/* 0x14 Reserved */
	LDR		PC,	rsrvd_ROM_add
irqvc_ROM:	/* 0x18 IRQ - Jump directly to the address given by the AIC */
	LDR		PC,[PC, #-0xF20]
fiqvc_ROM:	/* 0x1c FIQ	*/
	/* Switch in SVC/User Mode to allow User Stack access for C code */
	/*  because the FIQ is not yet acknowledged*/
	/* Save and r0 in FIQ_Register */
	mov		r9,r0
	ldr		r0 , [r8, #AIC_FVR]
	msr		CPSR_c,#I_BIT | F_BIT | ARM_MODE_SVC

	/* Save scratch/used registers and LR in User Stack */
	stmfd	sp!, { r1-r3, r12, lr}

	/* Branch to the routine pointed by the AIC_FVR */
	mov		r14, pc
	bx		r0

	/*- Restore scratch/used registers and LR from User Stack */
	ldmia	sp!, { r1-r3, r12, lr}

	/*- Leave Interrupts disabled and switch back in FIQ mode */
	msr		CPSR_c, #I_BIT | F_BIT | ARM_MODE_FIQ

	/*- Restore the R0 ARM_MODE_SVC register */
	mov		r0,r9

	/*- Restore the Program Counter using the LR_fiq directly in the PC */
	subs	pc,lr,#4
	/* end of FIQ_Handler_Entry */

/* fix up external exception vector addresses */	
reset_ROM_add:	.word	InitReset         
undef_ROM_add:	.word	AT91F_Undefined_Instruction_handler
swint_ROM_add:	.word	vPortYieldProcessor
preab_ROM_add:	.word	AT91F_Prefetch_Abort_handler
datab_ROM_add:	.word	AT91F_Data_Abort_handler
rsrvd_ROM_add:	.word	AT91F_Reserved_handler
	
/* end of ROM vector table */
/*------------------------------------------------------------------------------*/

	.section	.reset
	.text
	.global _startup
	.func   _startup
_startup:

	.align 0
.RAM_TOP:	.word	Top_Stack

InitReset:
/*------------------------------------------------------------------------------
/*- Low level Init (PMC, AIC, ? ....) by C function AT91F_LowLevelInit
/*------------------------------------------------------------------------------*/
	.extern   AT91F_LowLevelInit
	/*- minumum C initialization */
	/*- call  AT91F_LowLevelInit( void) */

	ldr		r13,.RAM_TOP /* temporary stack in internal RAM */
	/*--Call Low level init function in ABSOLUTE through the Interworking */
	ldr		r0,=AT91F_LowLevelInit
	mov		lr, pc
	bx		r0
	
/*------------------------------------------------------------------------------
//*- Stack Sizes Definition
//*------------------------
//*- Interrupt Stack requires 2 words x 8 priority level x 4 bytes when using
//*- the vectoring. This assume that the IRQ management.
//*- The Interrupt Stack must be adjusted depending on the interrupt handlers.
//*- Fast Interrupt not requires stack If in your application it required you must
//*- be definehere.
//*- The System stack size is not defined and is limited by the free internal
//*- SRAM.
//*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
//*- Top of Stack Definition
//*-------------------------
//*- Interrupt and Supervisor Stack are located at the top of internal memory in 
//*- order to speed the exception handling context saving and restoring.
//*- ARM_MODE_SVC (Application, C) Stack is located at the top of the external memory.
//*------------------------------------------------------------------------------*/
/* #include "AT91SAM7A3_inc.h"	  */

/* (3 x 8 x 4) */
	.equ UND_STACK_SIZE, 0x00000004
    .equ ABT_STACK_SIZE, 0x00000004
    .equ FIQ_STACK_SIZE, 0x00000004
    .equ IRQ_STACK_SIZE, 0X00000060
    .equ SVC_STACK_SIZE, 0x00000054

    .equ ARM_MODE_USR, 0x10 /* User Mode */
    .equ ARM_MODE_FIQ, 0x11 /* FIQ Mode */
    .equ ARM_MODE_IRQ, 0x12 /* IRQ Mode */
    .equ ARM_MODE_SVC, 0x13 /* Supervisor Mode */
    .equ ARM_MODE_ABT, 0x17 /* Abort Mode */
    .equ ARM_MODE_UND, 0x1B /* Undefined Mode */
    .equ ARM_MODE_SYS, 0x1F /* System Mode */

    .equ I_BIT, 0x80
    .equ F_BIT, 0x40

	.equ AIC_IVR,         (256) 
	.equ AIC_FVR,         (260) 
	.equ AIC_EOICR,       (304)
	.equ AT91C_BASE_AIC,  (0xFFFFF000)

/*------------------------------------------------------------------------------
//*- Setup the stack for each mode
//*-------------------------------*/
	mov		r0,r13
    msr		CPSR_c, #ARM_MODE_UND | I_BIT | F_BIT /* Undefined Instruction Mode */
    mov		sp, r0
    sub		r0, r0, #UND_STACK_SIZE
    
    msr		CPSR_c, #ARM_MODE_ABT | I_BIT | F_BIT /* Abort Mode */
    mov		sp, r0
    sub		r0, r0, #ABT_STACK_SIZE
    
    msr		CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT /* FIQ Mode */
    mov		sp, r0
   	ldr		r8, =AT91C_BASE_AIC
    sub		r0, r0, #FIQ_STACK_SIZE
    
    msr		CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT /* IRQ Mode */
    mov		sp, r0
    sub		r0, r0, #IRQ_STACK_SIZE
    
    msr		CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT /* Supervisor Mode */
    mov		sp, r0
    sub		r0, r0, #SVC_STACK_SIZE
    
    msr		CPSR_c, #ARM_MODE_SYS | I_BIT | F_BIT /* System Mode */
    mov		sp, r0

	/* We want to start in supervisor mode.  Operation will switch to system
	mode when the first task starts. */
	msr		CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT

# Relocate .data section (Copy from ROM to RAM)
	LDR		R1, =_etext
    LDR		R2, =_data
    LDR		R3, =_edata
LoopRel:
	CMP		R2, R3
	LDRLO	R0, [R1], #4
	STRLO	R0, [R2], #4
	BLO		LoopRel

# Clear .bss section (Zero init)
	MOV     R0, #0
	LDR     R1, =__bss_start__
	LDR     R2, =__bss_end__
LoopZI:
	CMP     R1, R2
	STRLO   R0, [R1], #4
	BLO     LoopZI

# Remap 
    .equ    MC_BASE,0xFFFFFF00  /* MC Base Address */
    .equ    MC_RCR, 0x00        /* MC_RCR Offset */

    LDR     R0, =MC_BASE
    MOV     R1, #1
    STR     R1, [R0, #MC_RCR]   /* Remap */

	ldr		lr,=exit
	ldr		r0,=main
    bx		r0

/* 
   *** Remap ***
   ROM_RUN: exception vectors for RAM have been already copied 
     to 0x00200000 by the .data copy-loop 
   RAM_RUN: exception vectors are already placed at 0x0020000 by
     linker settings
*/

	.size   _startup, . - _startup
	.endfunc
	
/* get end of flash aligned to next 256 byte block */	
	.global freeFlash
	.thumb_func
	.func	freeFlash
	.align	2
freeFlash:
    ldr		r0, =_edata		/* end of data to initialize */
    ldr		r1, =_data		/* start of data to initialize */
	sub		r0, r0, r1		/* length of initialized data block */
    ldr		r1, =_etext		/* first byte of initilization data in flash */
	add		r0, r0, r1 		/* first byte of free flash */
	add		r0, r0, #255	/* r0 = r3 + 255 */
	lsr		r0,#8			/* r0 = r0 >> 8 */
	lsl		r0,#8			/* r0 = r0 << 8 */
	bx		lr				/* return, r0 contains first free 256 byte block of flash */
	
	.size   freeFlash, . - freeFlash
	.endfunc
	
/* library exit function causes sbrk/read/write etc to be linked in, even if not used */
	.global exit
	.func   exit
exit:
	b    .
	.size   exit, . - exit
	.endfunc
	

	.end
