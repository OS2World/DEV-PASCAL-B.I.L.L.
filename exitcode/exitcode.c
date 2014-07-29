/*
 *		C . A . P .   " B I L L "   I N T E R P R E T E R
 *
 *		E R R O R   D I S P L A Y ,   C O R E   P R O C E D U R E S
 *
 *		St‚phane Charette @ C.A.P. Services
 *
 *		Last modified:  St‚phane Charette, 1995 October 29
 *
 *****************************************************************************
 *
 *		Project:	BILL
 *		Group:	error display
 *		File:		exitcode\exitcode.c
 *		Version:	0.0.3
 *
 *		This file contains all of the source code that makes up the exit and
 *		error code verification portion of the interpreter BILL.
 */



/*
 *	Versions:
 *
 *	0.0.1	- design of structure and implementation, St‚phane Charette, 94Mar28
 *	0.0.2	- added shutdown of all subsystems, Stephane Charette, 94Apr10
 *	0.0.3	- ported to OS/2, SC, 94Apr23-27
 * 0.0.3 - changed text formatting, 95Oct29
 */
 #define _EXITCODE_VERSION "Error display v0.0.3, St‚phane Charette, 95Oct29\n"


/*
 *	Includes
 */
	#include "..\exitcode\exitcode.hi"



/*
 *	FUNCTION:  ExitCode
 */
void ExitCode( ERR code )
{
	if( code )						// if an error occurred then...
	{
		#if( _LEX_DEBUG )
		ResetLexDebug( );			// ...reset the lexical debug info
		#endif

		#if( _SYMTABLE_DEBUG )
		ResetSymTableDebug( );	// ...reset the symbol table debug info
		#endif

		#if( _PARSER_DEBUG )
		ResetParserDebug( );		// ...reset the parser debug info
		#endif
	}

	if( code >= 3000 ) printf( "\n%s\n", _ExitCodeParser[code-3000] );
	else
		if( code >= 2000 ) printf( "\n%s\n", _ExitCodeSymTable[code-2000] );
		else
			if( code >= 1000 ) printf( "\n%s\n", _ExitCodeLex[code-1000] );
			else
				printf( "\n%s\n", _ExitCodeGen[code] );

	exit( code );
}

