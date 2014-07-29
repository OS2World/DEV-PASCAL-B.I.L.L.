//
//		NOTE:  THIS IS A QUICK HACK AT A main() FOR THE BILL PROJECT!
//



/*
 *	Includes
 */
	#include "..\parser\parser.h"
	#include "..\stackman\stackman.h"
	#include <conio.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>



/*
 *	FUNCTION:  main
 */
USHORT main( )
{
	char InputFileStr[ 512 ];

	ERR RC = ERR_OK;

	printf( "\n\nB.I.L.L. -- Bishop's Innovative Lovely Language\n"
			"PARSER & PCode generator for B.I.L.L.\n"
			"St‚phane Charette, 1994April27, 1995Oct29\n"
			"Compiled for OS/2 v3.0 using IBM's C++ compiler v2.1" );

	do
	{
		printf( "\n\nWhich file to use as input?\n" );
		scanf( "%511s", InputFileStr );

		RC = InitParser( InputFileStr );
		#if( _PARSER_DEBUG )
		RC = InitParserDebug( FALSE, TRUE );
		#endif

		BeginParser( );

		#if( _PARSER_DEBUG )
		ResetParserDebug( );
		#endif
		ResetParser( );

		printf( "\n\nPCode generated ok.\n" );

		printf( "\nRun the interpreter?\n" );
		if( strchr( "Yy", getch( ) ) )
		{
			printf( "yes\n\n\n\n\n" );
			RunStackMachine( );
		}
		else
		{
			printf( "no\n" );
		}

		printf( "\nRestart B.I.L.L.?\n" );

	} while( strchr( "Yy", getch( ) ) );

	printf( "no\n" );

	return RC;
}
