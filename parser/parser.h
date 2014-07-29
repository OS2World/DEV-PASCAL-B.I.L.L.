/*
 *		C . A . P .   P A R S E R
 *
 *		G L O B A L   I N C L U D E   F I L E
 *
 *		St‚phane Charette @ C.A.P. Services
 *
 *		Last modified:  St‚phane Charette, 1995 October 29
 *
 *****************************************************************************
 *
 *		Project:	BILL
 *		Group:	parser
 *		File:		parser\parser.h
 *
 *		This file contains all of the global definitions used with the parser
 *		portion of the interpreter BILL.
 */


#ifndef _PARSER_H_GLOBAL

	#define _PARSER_H_GLOBAL

	/*
	 *		Includes
	 */
		#include "..\global\global.h"



	/*
	 *		Defines
	 */
		#define PARSER_MAX_PATH_LEN	_MAX_LENGTH_OF_PATH



	/*
	 *		Types & structures
	 */



	/*
	 *		Macros
	 */



	/*
	 *		Global (external) variables
	 */



	/*
	 *		Global (external) prototyping
	 */
		ERR InitParser( UCHAR filename[] );	// initialize the parser //
		ERR ResetParser( void );			// reset the parser
		ERR BeginParser( void );			// begin the parser recognition
		#if( _SYMTABLE_DEBUG )
		ERR InitParserDebug( BOOL screen, BOOL source );	// initialize parser debug module
		ERR ResetParserDebug( void );		// reset the parser debug module
		#endif



#endif

