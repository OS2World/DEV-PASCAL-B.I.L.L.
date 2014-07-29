/*
 *		C . A . P .   " B I L L "   I N T E R P R E T E R
 *
 *		G L O B A L   I N C L U D E   F I L E ,   E R R O R   C O D E S
 *
 *		St‚phane Charette @ C.A.P. Services
 *
 *		Last modified:  St‚phane Charette, 1995 October 29
 *
 *****************************************************************************
 *
 *		Project:	BILL
 *		Group:	none
 *		File:		global\error.h
 *
 *		This file contains all of the possible error codes returned by the
 *		BILL interpreter.
 */



#ifndef _ERROR_H_GLOBAL

	#define _ERROR_H_GLOBAL

	/*
	 *		Function return codes
	 */
		#define ERR_OK								   0	// no error
		#define ERR_GEN							   1	// general error



	/*
	 *		Lexical analyzer (1000+)
	 */
		#define ERR_LEX_GEN						1000	// general lex error
		#define ERR_LEX_PATH_TOO_LONG			1001	// path specified is too long
		#define ERR_LEX_CANNOT_OPEN_FILE		1002	// error opening file
		#define ERR_LEX_CANNOT_READ_FILE		1003	// error reading file
		#define ERR_LEX_CANNOT_CLOSE_FILE	1004	// error closing file
		#define ERR_LEX_END_OF_FILE			1005	// end of file reached (not necessarily an error)
		#define ERR_LEX_TOKEN_TOO_LONG		1006	// length of token exceeds that set by LEX_MAX_TOKEN_LEN
		#define ERR_LEX_CLOSE_COMMENT_BLOCK	1007	// comment close character found without comment open character
		#define ERR_LEX_UNEXPECTED_EOF		1008	// unbalanced string or comment caused read past eof
		#define ERR_LEX_UNRECOGNIZED_CHAR	1009	// unrecognized character in source file
		#define ERR_LEX_DEBUG_FILE_ERROR		1010	// cannot access the lexical debug output file



	/*
	 *		Symbol table (2000+)
	 */
		#define ERR_SYMT_GEN						2000	// general symbol table error
		#define ERR_SYMT_DEBUG_FILE_ERROR	2001	// cannot access the symbol table debug output file
		#define ERR_SYMT_TOO_MANY_IDS			2002	// too many table entries
		#define ERR_SYMT_UNDEFINED_ID			2003	// tried to reference an undefined ID



	/*
	 *		Parser (3000+)
	 */
		#define ERR_PARSER_GEN					3000	// general parser error
		#define ERR_PARSER_PATH_TOO_LONG		3001	// path specified is too long
		#define ERR_PARSER_DEBUG_FILE_ERROR	3002	// cannot access the parser debug output file
		#define ERR_PARSER_PROGRAM_EXPCTD	3003	// expected "program" keyword
		#define ERR_PARSER_IDENT_EXPCTD		3004	// expected identifier
		#define ERR_PARSER_SEMICOLON_EXPCTD	3005	// expected semi-colon
		#define ERR_PARSER_PERIOD_EXPCTD		3006	// expected period
		#define ERR_PARSER_VALUE_EXPCTD		3007	// expected numeric value
		#define	ERR_PARSER_REDEFINITION		3008	// variable/constant redefinition
		#define ERR_PARSER_EQUAL_EXPCTD		3009	// expected "=" symbol
		#define ERR_PARSER_BEGIN_EXPCTD		3010	// expected "begin" keyword
		#define ERR_PARSER_END_EXPCTD			3011	// expected "end" keyword
		#define ERR_PARSER_ASSIGN_EXPCTD		3012	// expected ":=" symbol
		#define ERR_PARSER_STATEMENT_EXPCTD	3013	// expected a statement
		#define ERR_PARSER_PARENTO_EXPCTD	3014	// expected open parentheses
		#define ERR_PARSER_PARENTC_EXPCTD	3015	// expected close parentheses
		#define ERR_PARSER_THEN_EXPCTD		3016	// expected "then" keyword
		#define ERR_PARSER_DO_EXPCTD			3017	// expected "do" keyword
		#define ERR_PARSER_CONST_REASSIGN	3018	// detected constant reassignment
		#define ERR_PARSER_RELOP_EXPCTD		3019	// expected relational operator



	/*
	 *		Parser Plus (4000+)
	 */



	/*
	 *		Interpreter (5000+)
	 */



	/*
	 *		Run-time library (6000+)
	 */

#endif

