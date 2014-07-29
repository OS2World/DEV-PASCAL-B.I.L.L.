/*
 *		C . A . P .   L E X I C A L   A N A L Y Z E R
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
 *		Group:	lexical analyzer
 *		File:		lexical\lexical.h
 *
 *		This file contains all of the global definitions used with the lexical
 *		portion of the interpreter BILL.
 */



#ifndef _LEXICAL_H_GLOBAL

	#define _LEXICAL_H_GLOBAL

	/*
	 *		Includes
	 */
		#include "..\global\global.h"
		#include "..\lexical\lex_ids.h"



	/*
	 *		Defines
	 */
		#define LEX_MAX_PATH_LEN	_MAX_LENGTH_OF_PATH
		#define LEX_MAX_TOKEN_LEN	128



	/*
	 *		Types & structures
	 */
		struct LEXICAL_STATE_STRUCT
		{
			BOOL		InComment;		// are we inside a comment?
			BOOL		InString;		// are we inside a string?
			ULONG		LineNumber;		// current line number
			USHORT	CharOffset;		// character offset into current line
			ERR		Error;			// last error encountered
			USHORT	TokenID;			// id of current token
			USHORT	LexemeID;		// id of lexeme (if applicable)
			SHORT		VarID;			// value of identifier, or table entry
			UCHAR		Token[ LEX_MAX_TOKEN_LEN + 1 ];		// current token
		};
		typedef struct LEXICAL_STATE_STRUCT LEXICAL_STATE;



	/*
	 *		Macros
	 */



	/*
	 *		Global (external) variables
	 */
		extern LEXICAL_STATE Lex_State;



	/*
	 *		Global (external) prototyping
	 */
		ERR InitLex( UCHAR path[] );	// initialize all variables for lex
		ERR ResetLex( void );			// reset all variables used with lex
		ERR GetNextToken( void );		// read buffer and return next token
		#if( _LEX_DEBUG )
		ERR InitLexDebug( BOOL screen, BOOL source, BOOL token, BOOL mix );	// initialize lexical debug module
		ERR ResetLexDebug( void );		// reset the lexical debug module
		#endif



#endif

