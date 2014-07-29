/*
 *		C . A . P .   P C O D E   G E N E R A T O R
 *
 *		G L O B A L   I N C L U D E   F I L E ,   I D E N T I F I E R S
 *
 *		St‚phane Charette @ C.A.P. Services
 *
 *		Last modified:  St‚phane Charette, 1995 October 29
 *
 *****************************************************************************
 *
 *		Project:	BILL
 *		Group:	pcode
 *		File:		pcode\pcode_id.h
 *
 *		This file contains all of the ids in the pcode generator used with
 *		the interpreter BILL.
 */



#ifndef _PCODE_IDS_H_GLOBAL

	#define _PCODE_IDS_H_GLOBAL

	/*
	 *		Defines
	 */
		#define		NEG		1			// negate top of stack
		#define		ADD		2			// add next-to-top to top-of-stack, leave result as top-of-stack
		#define		SUB		3			// substact top-of-stack from next-to-top, leave result as top-of-stack
		#define		MUL		4			// multiply next-to-top by top-of-stack, leave result as top-of-stack
		#define		DVD		5			// divide next-to-top by top-of-stack, leave result as top-of-stack
		#define		EQL		6			// leave 1 as top-of-stack if next-to-top == top-of-stack, 0 otherwise
		#define		NEQ		7			// leave 1 as top-of-stack if next-to-top <> top-of-stack, 0 otherwise
		#define		GTR		8			// leave 1 as top-of-stack if next-to-top > top-of-stack, 0 otherwise
		#define		LSS		9			// leave 1 as top-of-stack if next-to-top < top-of-stack, 0 otherwise
		#define		LEQ		10			// leave 1 as top-of-stack if next-to-top <= top-of-stack, 0 otherwise
		#define		GEQ		11			// leave 1 as top-of-stack if next-to-top => top-of-stack, 0 otherwise
		#define		STK		12			// dump stack to printer
		#define		PRN		13			// print top-of-stack as an integer, pop stack
		#define		PRS		14			// print the top-of-stack elements below top of stack as ASCII, pop top-of-stack + 1
		#define		NLN		15			// print carriage-return-line-feed sequence
		#define		INN		16			// read integer value, store on stack element whose address is top-of-stack, pop stack
		#define		INT		17			// increment stack pointer by A (where A is constant)
		#define		LDI		18			// push integer value A onto stack
		#define		LDA		19			// push address of A onto stack
		#define		LDV		20			// replace top of stack by element whose address is currently top-of-stack
		#define		STO		21			// store top-of-stack in memory at the address given as next-to-top, pop two elements
		#define		HLT		22			// halt
		#define		BRN		23			// unconditional branch to instruction A
		#define		BZE		24			// branch to instruction A if top-of-stack is zero, pop stack
		#define		NOP		25			// no operation
		#define		OD			26			// leave 1 if top-of-stack is odd, otherwise 0



#endif

