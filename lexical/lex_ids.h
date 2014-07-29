/*
 *		C . A . P .   L E X I C A L   A N A L Y Z E R
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
 *		Group:	lexical analyzer
 *		File:		lexical\lex_ids.h
 *
 *		This file contains all of the token and lexeme identifiers used with
 *		the lexical analyzer portion of the interpreter BILL.
 */



#ifndef _LEX_IDS_H_GLOBAL

	#define _LEX_IDS_H_GLOBAL

	/*
	 *		Includes
	 */



	/*
	 *		Defines (Tokens)
	 */
		#define TOK_NO_ID				0	// no id recognized
		#define TOK_ERROR_ID			1	// error occurred while getting token
		#define TOK_END_OF_FILE_ID	2	// end of file was reached
		#define TOK_PROGRAM_ID		3	// program
		#define TOK_CONST_ID			4	// const
		#define TOK_VAR_ID			5	// var
		#define TOK_BEGIN_ID			6	// begin
		#define TOK_END_ID			7	// end
		#define TOK_IF_ID				8	// if
		#define TOK_THEN_ID			9	// then
		#define TOK_WHILE_ID			10	// while
		#define TOK_DO_ID				11	// do
		#define TOK_WRITE_ID			12	// write
		#define TOK_READ_ID			13	// read
		#define TOK_ODD_ID			14	// odd
		#define TOK_ASSIGNMENT_ID	15	// assignment ":="
		#define TOK_IDENTIFIER_ID	16	// "identifier" (variable)
		#define TOK_STRING_ID		17	// "identifier" (string)
//		#define TOK_OP_ID				18	// "operator" (see lexemes)
		#define TOK_RELOP_ID			19	// "relational operator" (see lexemes)
		#define TOK_PLUS_ID			20	// plus "+"
		#define TOK_MINUS_ID			21	// minus "-"
		#define TOK_MULT_ID			22	// multiply "*"
		#define TOK_DIV_ID			23	// divide "/"
		#define TOK_PO_ID				24	// parenteses open "("
		#define TOK_PC_ID				25	// parenteses close ")"
		#define TOK_SEMICOLON_ID	26	// semicolon ";"
		#define TOK_COMMA_ID			27	// comma ","
		#define TOK_PERIOD_ID		28	// period "."



	/*
	 *		Defines (Lexemes)
	 */
		#define LEX_NO_ID				0	// no lexeme recognized
		#define LEX_EQ_ID				1	// equal "="
		#define LEX_NE_ID				2	// not equal "<>"
		#define LEX_LE_ID				3	// less than or equal to "<="
		#define LEX_LT_ID				4	// less than "<"
		#define LEX_GE_ID				5	// greater than or equal to ">="
		#define LEX_GT_ID				6	// greater than ">"
		#define LEX_VALUE_ID			13	// "VarID" is a numeric or "Token[]" is a string
												// If a variable	->	value is in Lex_State.VarID
												// If a string		->	characters are in Lex_State.Token[]
		#define LEX_VAR_ID			14	// "VarID" is a table entry
												//						->	Lex_State.VarID is the table entry



#endif

