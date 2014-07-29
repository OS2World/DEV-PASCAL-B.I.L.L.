/*
 *		C . A . P .   I N T E R P R E T E R
 *
 *		C O R E   P R O C E D U R E S
 *
 *		St‚phane Charette @ C.A.P. Services
 *
 *		Last modified:  St‚phane Charette, 1995 October 29
 *
 *****************************************************************************
 *
 *		Project:	BILL
 *		Group:	interpreter
 *		File:		stackman\stackman.c
 *		Version:	0.1.4
 *
 *		This file contains all of the source code that makes up the parser
 *		used with the interpreter BILL.
 */



/*
 *	Versions:
 *
 *	0.0.1	- design of structure and implementation, St‚phane Charette, 94Apr12
 *	0.1.0	- first working version, SC, 94Apr12
 *	0.1.1	- worked out bugs in stack and code addressing, SC, 94Apr12-13
 *	0.1.2	- added trap for "divide by zero", SC, 94Apr13
 *	0.1.3	- added opcode for odd function, SC, 94Apr14
 *	0.1.4	- ported to OS/2, SC, 94Apr23-27
 *	0.1.4 - changed text formatting, SC, 95Oct29
 */
 #define _STACKMAN_VERSION "Stack machine v0.1.4, St‚phane Charette, 95October29\n"



/*
 *	Includes
 */
	#include "..\pcode\pcode_id.h"
	#include <stdio.h>
	#include <stdlib.h>



/*
 *	Global vars
 */
	FILE *inputfileptr;
	signed int CS[ 10000 ];		// code segment
	signed int SS[ 1000 ];		// stack segment
	signed int IP;					// instruction pointer
	signed int SP;					// stack pointer



/*
 *	Prototyping
 */
	int RunStackMachine( void );
	int LoadProgramme( void );



/*
 *	FUNCTION:  main
 */
int RunStackMachine( )
{
	printf( "B.I.L.L. interpreter v0.1.2\nDesigned and implemented by St‚phane Charette\n"
			"Compiled for OS/2 v3.0 using IBM's C++ compiler v2.1.\nLoading code..." );
	LoadProgramme( );
	printf( "...finished!\nStarting execution.\n\n" );

	IP = 0;
	SP = 0;

	while( CS[IP] != HLT )
	{
		switch( CS[IP] )
		{
			case NEG:
			{
				SS[SP-1] = - SS[SP-1];
				break;
			}
			case ADD:
			{
				SS[SP-2] = SS[SP-2] + SS[SP-1];
				SP--;
				break;
			}
			case SUB:
			{
				SS[SP-2] = SS[SP-2] - SS[SP-1];
				SP--;
				break;
			}
			case MUL:
			{
				SS[SP-2] = SS[SP-2] * SS[SP-1];
				SP--;
				break;
			}
			case DVD:
			{
				if( SS[SP-1] == 0 )
				{
					printf( "Hey buddy!  Wake up!  I may be good, but not that good.\n"
							"Since when do you think I can divide by zero?  Jeez...\n\a" );
							exit( 1 );
				}
				else
				{
					div_t tmpnum = div( SS[SP-2], SS[SP-1] );
					SS[SP-2] = tmpnum.quot;
					SP--;
				}
				break;
			}
			case EQL:
			{
				SS[SP-2] = SS[SP-2] == SS[SP-1];
				SP--;
				break;
			}
			case NEQ:
			{
				SS[SP-2] = SS[SP-2] != SS[SP-1];
				SP--;
				break;
			}
			case GTR:
			{
				SS[SP-2] = SS[SP-2] > SS[SP-1];
				SP--;
				break;
			}
			case LSS:
			{
				SS[SP-2] = SS[SP-2] < SS[SP-1];
				SP--;
				break;
			}
			case LEQ:
			{
				SS[SP-2] = SS[SP-2] <= SS[SP-1];
				SP--;
				break;
			}
			case GEQ:
			{
				SS[SP-2] = SS[SP-2] >= SS[SP-1];
				SP--;
				break;
			}
			case STK:
			{
				// print the stack here...  NOT IMPLEMENTED
				printf( "\nPRINT THE STACK!\nI don't know what you're trying to"
						"do to me, but I don't like it - so I'm ignoring you!\a" );
				break;
			}
			case PRN:
			{
				printf( "%i", SS[SP-1] );
				SP--;
				break;
			}
			case PRS:
			{
				signed int i = SP - SS[SP-1] - 2;
				while( ++i < ( SP - 1 ) )
				{
					printf( "%c", (unsigned char)( SS[i] ) );
				}
				SP = SP - SS[SP-1] - 1;
				break;
			}
			case NLN:
			{
				printf( "\n" );
				break;
			}
			case INN:
			{
				scanf( "%i", &(SS[SS[SP-1]]) );
				SP--;
				break;
			}
			case INT:
			{
				SP = SP + CS[IP+1];
				IP++;
				break;
			}
			case LDI:
			case LDA:
			{
				SS[SP] = CS[IP+1];
				SP++;
				IP++;
				break;
			}
			case LDV:
			{
				SS[SP-1] = SS[SS[SP-1]];
				break;
			}
			case STO:
			{
				SS[SS[SP-2]] = SS[SP-1];
				SP -= 2;
				break;
			}
			case BRN:
			{
				IP = CS[IP+1];
				break;
			}
			case BZE:
			{
				if( SS[SP-1] == 0 ) IP = ( CS[IP+1] - 2 );
				SP--;
				IP++;
				break;
			}
			case NOP:
			{
				break;
			}
			case OD:
			{
				if( ( SS[SP-1] % 2 ) == 0 )
				{
					SS[SP-1] = 0;
				}
				else
				{
					SS[SP-1] = 1;
				}
				break;
			}
			default:
			{
				printf( "\nUnrecognized opcode (%i). I think you're doing it\n"
						"on purpose to make me mad.  Find me some real code!\n\a", CS[IP] );
				exit( 2 );
				break;
			}
		}	// end of switch statement
		IP++;
	}	// end of while loop

	printf( "\nProgramme execution finished.\n" );

	return;
}



/*
 *	FUNCTION:  LoadProgramme
 */
int LoadProgramme( )
{
	inputfileptr = fopen( "pcode.lst", "r" );
	if( ! inputfileptr )
	{
		printf( "\n\nSorry buddy...  Can't find the opcodes!  Where did\n"
				"you put the pcode output file?\n\a" );
		exit( 1 );
	}
	else
	{
		signed int i = 0;
		signed int count = 0;

		while( ! feof( inputfileptr ) )
		{
			signed int code;
			signed int result;

			result = fscanf( inputfileptr, "%i\n", &code );
			if( result != 1 )
			{
				printf( "\n\nSorry buddy...  Had problems reading the list\n"
						"of opcodes.  Are you sure this file is ok?\n\a" );
				exit( 1 );
			}

			count ++;

			// store the values into the programme buffer
			CS[i++] = code;

			switch( code )
			{
				case INT:
				case LDI:
				case LDA:
				case BRN:
				case BZE:
				{	// second code required for these opcodes
					result = fscanf( inputfileptr, "%i\n", &code );
					if( result != 1 )
					{
						printf( "\n\nHum...something is really wrong.  I attempted to read\n"
								"an opcode parameter, but it somehow messed up on me...\n\a" );
						exit( 1 );
					}
					CS[i++] = code;
					count++;
				}
			}
		}
		printf( "(read %i opcodes)",count );
	}

	fclose( inputfileptr );
	return 0;
}

