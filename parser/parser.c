/*
 *		C . A . P .   P A R S E R
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
 *		Group:	parser
 *		File:		parser\parser.c
 *		Version:	0.2.3
 *
 *		This file contains all of the source code that makes up the parser
 *		used with the interpreter BILL.
 */



/*
 *	Versions:
 *
 *	0.0.1	- design of structure and implementation, St‚phane Charette, 94Mar27
 *	0.0.2	- implemented procedures for top-down recursive descent, SC, 94Mar27-Apr10
 *	0.1.0	- added simple pcode generator for basic functions, SC, 94Apr10-11
 *	0.2.0	- included full-function pcode generator, SC, 94Apr11
 *	0.2.1	- fixed various bugs occuring while generating pcode, SC, 94Apr11-13
 *	0.2.2	- fixed odd functionality, SC, 94Apr14
 *	0.2.2	- fixed while functionality, SC, 94Apr14-23
 *	0.2.3	- ported to OS/2, SC, 94Apr23-27
 *	0.2.3 - reformatted text, SC, 95Oct29
 */
 #define _PARSER_VERSION "Parser v0.2.2, St‚phane Charette, 95October29\n"



/*
 *	Includes
 */
	#include "..\parser\parser.hi"		// internal parser include file



/*
 *	FUNCTION:  InitParser
 */
ERR InitParser( UCHAR filename[] )
{
	ERR RC = ERR_OK;

	#if( _PARSER_DEBUG )
	Parser_Debug.ScreenOutput = false;	// pre-initialize debug value
	Parser_Debug.FileOutput = false;		// pre-initialize debug value
	Parser_Debug.Indent = 0;				// indentation for parse tree
	#endif

	Parser_State.Error = ERR_OK;			// internal symbol table error

	// is the path len valid?
	if( strlen( filename ) > PARSER_MAX_PATH_LEN ) ParserError( ERR_PARSER_PATH_TOO_LONG );	//	is the path valid?
	strcpy( Parser_State.Filename, filename );													//	save the path of file

	// initialize the lexical analyzer
	InitLex( filename );
	#if( _LEX_DEBUG )
	InitLexDebug( true, true, true, true );
	#endif

	// initialize the symbol table
	InitSymTable( );
	#if( _SYMTABLE_DEBUG )
	InitSymTableDebug( false, true );
	#endif

	// initialize the pcoder
	StartPCodeGenerator( );

	return RC;
}



/*
 *	FUNCTION:  ResetParser
 */
ERR ResetParser( )
{
	return ERR_OK;
}



/*
 *	FUNCTION:  ParserError
 */
void ParserError( ERR error_code )
{
	Parser_State.Error = error_code;

	printf( "\n\nError encountered in parser:  %i\n", Parser_State.Error );
	printf( "Token in question:  \"%s\".\n\a", Lex_State.Token );
	#if( _SYMTABLE_DEBUG )
	ResetSymTableDebug( );					// shutdown the debug procedures
	#endif

	ResetParser( );							// perform shutdown procedures

	ExitCode( error_code );					// return to operating system
}



/*
 *	FUNCTION:  BeginParser
 */
ERR BeginParser( )
{
	ERR RC = ERR_OK;

	Parser_Program( );

	// write out the generated pcode
	WritePCodeBuffer( );

	return RC;
}



/*
 *	FUNCTION:  Parser_Program
 */
void Parser_Program( )
{
	ERR RC;

	// get the first token - hopefully the keyword "program"
	_AcceptNext( TOK_PROGRAM_ID, ERR_PARSER_PROGRAM_EXPCTD );

	// get the next token - hopefully an identifier
	_AcceptNext( TOK_IDENTIFIER_ID, ERR_PARSER_IDENT_EXPCTD );

	// get the next token - hopefully a semi-colon
	_AcceptNext( TOK_SEMICOLON_ID, ERR_PARSER_SEMICOLON_EXPCTD );

	Parser_Block( );				// get the "block" - the rest of the programme

	// look at the current token - hopefully a period
	_AcceptNext( TOK_PERIOD_ID, ERR_PARSER_PERIOD_EXPCTD );

	// programme was recognized!
	AddPCode( HLT );

	#if( _PARSER_DEBUG )
	ParserDebugWrite( "\nFile parsed correctly.\n" );
	#endif

	// if any code exists after the last period, it will be completely ignored...

	return;
}



/*
 *	FUNCTION:  Parser_Block
 */
void Parser_Block( )
{
	ERR RC;

	_GetNextToken;

	while( ( Lex_State.TokenID == TOK_CONST_ID ) || ( Lex_State.TokenID == TOK_VAR_ID ) )
	{
		// look at the current token - maybe the keyword "const"
		if( Lex_State.TokenID == TOK_CONST_ID )	// is there a "const" section to this programme?
		{														// ...yes there is
			Parser_Constdeclaration( );				// ...evaluate const declarations
		}

		// look at the current token - maybe the keyword "var"
		if( Lex_State.TokenID == TOK_VAR_ID )		// is there a "var" section to this programme?
		{														// ...yes there is
			Parser_Vardeclaration( );					// ...evaluate var declerations
		}
	}

	// the next token should be the "begin" keyword
	Parser_Statementpart( );	// get the "statements" - the actual programme

	// now that the programme is done, there should only be an "end." left
	_AcceptCurrent( TOK_END_ID, ERR_PARSER_END_EXPCTD );

	return;
}


	
/*
 *	FUNCTION:  Parser_Constdeclaration
 */
void Parser_Constdeclaration( )
{
	ERR RC;
	UCHAR constname[LEX_MAX_TOKEN_LEN+1];
	SHORT constindex;
	USHORT number_of_consts = 0;
	USHORT address_of_consts;

	address_of_consts = AddDPCode( INT, 0 );	// increment stack for constant declarations

	// we've recognized "const", so now lets read the identifiers/values
	_GetNextToken;								// get the const name
	do
	{
		number_of_consts ++;					// number of constants that we have found so far...

		if( ( Lex_State.TokenID != TOK_IDENTIFIER_ID ) || ( Lex_State.LexemeID != LEX_VAR_ID ) )
			ParserError( ERR_PARSER_IDENT_EXPCTD );

		strcpy( constname, Lex_State.Token );	// store the name for when we'll update it
		// register this with the symbol table
		RC = NewTableEntry( &constindex, constname );
		if( RC ) ParserError( RC );
		if( constindex == SYM_DUPLICATE_ID ) ParserError( ERR_PARSER_REDEFINITION );

		// make current declaration into a constant
		RC = TableMakeConst( constname );
		if( RC ) ParserError( RC );

		// get the "=" symbol
		_GetNextToken;							// get the "=" symbol
		if( ( Lex_State.TokenID != TOK_RELOP_ID ) || ( Lex_State.LexemeID != LEX_EQ_ID ) )
			ParserError( ERR_PARSER_EQUAL_EXPCTD );

		// get the value for this token
		_GetNextToken;							// get the value to associate with this name
		if( ( Lex_State.TokenID != TOK_IDENTIFIER_ID ) || ( Lex_State.LexemeID != LEX_VALUE_ID ) )
			ParserError( ERR_PARSER_VALUE_EXPCTD );

		// assign the value to the constant
		AddDPCode( LDI, constindex );
		AddDPCode( LDI, Lex_State.VarID );
		AddPCode( STO );

		// get the semicolon following this declaration
		_AcceptNext( TOK_SEMICOLON_ID, ERR_PARSER_SEMICOLON_EXPCTD );

		// get the next token - could be another constant declaration or something new
		_GetNextToken;
	}
	while( ( Lex_State.TokenID == TOK_IDENTIFIER_ID ) && ( Lex_State.LexemeID == LEX_VAR_ID ) );

	// make sure that the correct number of spaces are reserved on stack
	UpdatePCode( address_of_consts, number_of_consts );

	return;
}



/*
 *	FUNCTION:  Parser_Vardeclaration
 */
void Parser_Vardeclaration( )
{
	ERR RC;
	SHORT varindex;
	USHORT number_of_vars = 0;
	USHORT address_of_vars;

	address_of_vars = AddDPCode( INT, 0 );	// increment stack for variable declarations


	// we've recognized "var", so now lets read the identifiers/values

	do
	{
		number_of_vars ++;					// number of vars that we have found so far...

		// get the next identifier
		_GetNextToken;							// get the var name

		if( ( Lex_State.TokenID != TOK_IDENTIFIER_ID ) || ( Lex_State.LexemeID != LEX_VAR_ID ) )
			ParserError( ERR_PARSER_IDENT_EXPCTD );

		// register this with the symbol table
		RC = NewTableEntry( &varindex, Lex_State.Token );
		if( RC ) ParserError( RC );
		if( varindex == SYM_DUPLICATE_ID ) ParserError( ERR_PARSER_REDEFINITION );

		// get the token following this declaration
		_GetNextToken;							// get the comma (maybe) following the declaration
	}
	while( Lex_State.TokenID == TOK_COMMA_ID );

	// since we didn't just read a comma, it better be a semicolon
	_AcceptCurrent( TOK_SEMICOLON_ID, ERR_PARSER_SEMICOLON_EXPCTD );

	// make sure that the correct number of spaces are reserved on stack
	UpdatePCode( address_of_vars, number_of_vars );

	// get the next token - just to be ready for whatever comes after this var declaration
	_GetNextToken;								// get whatever comes next

	return;
}



/*
 *	FUNCTION:  Parser_Statementpart
 */
void Parser_Statementpart( )
{
	ERR RC = ERR_OK;
	_AcceptCurrent( TOK_BEGIN_ID, ERR_PARSER_BEGIN_EXPCTD );
	Parser_Statementsequence( );
	_AcceptCurrent( TOK_END_ID, ERR_PARSER_END_EXPCTD );
	return;
}



/*
 *	FUNCTION:  Parser_Statementsequence
 */
void Parser_Statementsequence( )
{
	ERR RC = ERR_OK;

	Parser_Statement( );
	while( Lex_State.TokenID == TOK_SEMICOLON_ID )
	{
		_AcceptCurrent( TOK_SEMICOLON_ID, ERR_PARSER_SEMICOLON_EXPCTD );
		Parser_Statement( );
	}
	return;
}



/*
 *  FUNCTION:  Parser_Statement
 */
void Parser_Statement( )
{
	ERR RC = ERR_OK;

	_GetNextToken;								// get the next token

	switch( Lex_State.TokenID )
	{
		case TOK_BEGIN_ID:					// BEGIN statement
		{
			Parser_Statementpart( );
			_GetNextToken;						// be prepared for what comes next
			break;
		}
		case TOK_IDENTIFIER_ID:				// assignment
		{
			Parser_Assignment( );
			break;
		}
		case TOK_IF_ID:						// IF statement
		{
			Parser_Ifstatement( );
			break;
		}
		case TOK_WHILE_ID:					// WHILE statement
		{
			Parser_Whilestatement( );
			break;
		}
		case TOK_WRITE_ID:					// WRITE statement
		{
			Parser_Writestatement( );
			break;
		}
		case TOK_READ_ID:						// READ statement
		{
			Parser_Readstatement( );
			_GetNextToken;						// be prepared for what comes next
			break;
		}
	}

	return;
}



/*
 *	FUNCTION:  Parser_Assignment
 */
void Parser_Assignment( )
{
	ERR RC = ERR_OK;
	BOOL isconst;
	SHORT index;

	_AcceptCurrent( TOK_IDENTIFIER_ID, ERR_PARSER_IDENT_EXPCTD );

	// is this identifier a constant?  (does it even exist?)
	RC = TableIsConst( &isconst, Lex_State.Token );
	if( RC ) ParserError( RC );
	if( isconst ) ParserError( ERR_PARSER_CONST_REASSIGN );

	// get the index for this identifier
	RC = GetTableEntry( &index, Lex_State.Token );
	if( RC ) ParserError( RC );

	// note the address (index) of this assignment
	AddDPCode( LDI, index );

	_AcceptNext( TOK_ASSIGNMENT_ID, ERR_PARSER_ASSIGN_EXPCTD );

	// find out what this assignment is worth
	_GetNextToken;
	Parser_Expression( );

	// perform the assignment
	AddPCode( STO );

	return;
}



/*
 *	FUNCTION:  Parser_Expression
 */
void Parser_Expression( )
{
	ERR RC = ERR_OK;

	switch( Lex_State.TokenID )
	{
		case TOK_PLUS_ID:			// expression begins with "+"
		{
			_GetNextToken;
			Parser_Term( );
			AddPCode( ADD );
			break;
		}
		case TOK_MINUS_ID:			// expression begins with "-"
		{
			_GetNextToken;
			Parser_Term( );
			AddPCode( NEG );
			AddPCode( ADD );
			break;
		}
		default:					// expression does not start with "+" or "-"
		{
			Parser_Term( );
		}
	}

	if( ( Lex_State.TokenID == TOK_PLUS_ID ) || ( Lex_State.TokenID == TOK_MINUS_ID ) ) Parser_Expression( );

	return;
}



/*
 *	FUNCTION:  Parser_Term
 */
void Parser_Term( )
{
	ERR RC = ERR_OK;

	Parser_Factor( );

	while( ( Lex_State.TokenID == TOK_MULT_ID ) || ( Lex_State.TokenID == TOK_DIV_ID ) )
	{
		switch( Lex_State.TokenID )
		{
			case TOK_MULT_ID:
			{
				_AcceptCurrent( TOK_MULT_ID, ERR_PARSER_GEN );
				_GetNextToken;
				Parser_Factor( );
				AddPCode( MUL );	// multiply
				break;
			}
			case TOK_DIV_ID:
			{
				_AcceptCurrent( TOK_DIV_ID, ERR_PARSER_GEN );
				_GetNextToken;
				Parser_Factor( );
				AddPCode( DVD );	// divide
				break;
			}
		}
	}

	return;
}



/*
 *	FUNCTION:  Parser_Factor
 */
void Parser_Factor( )
{
	ERR RC = ERR_OK;

	switch( Lex_State.TokenID )
	{
		case TOK_IDENTIFIER_ID:
		{
			switch( Lex_State.LexemeID )
			{
				case LEX_VALUE_ID:	// token is a numeric value
				{
					AddDPCode( LDI, Lex_State.VarID );
					break;
				}
				case LEX_VAR_ID:		// token is a variable
				{
					SHORT index;

					RC = GetTableEntry( &index, Lex_State.Token );
					if( RC ) ParserError( RC );
					AddDPCode( LDI, index );
					AddPCode( LDV );
					break;
				}
			}
			break;
		}
		case TOK_PO_ID:				// parentheses open
		{
			_AcceptCurrent( TOK_PO_ID, ERR_PARSER_GEN );
			_GetNextToken;
			Parser_Expression( );
			_AcceptCurrent( TOK_PC_ID, ERR_PARSER_PARENTC_EXPCTD );
			break;
		}
	}
	// just to be ready for whatever comes next
	_GetNextToken;
	return;
}



/*
 *	FUNCTION:  Parser_Ifstatement
 */
void Parser_Ifstatement( )
{
	ERR RC = ERR_OK;
	USHORT address;

	_AcceptCurrent( TOK_IF_ID, ERR_PARSER_GEN );
	Parser_Condition( );
	address = AddDPCode( BZE, 0 );
	_AcceptCurrent( TOK_THEN_ID, ERR_PARSER_THEN_EXPCTD );
	Parser_Statement( );

	UpdatePCode( address, (SHORT)GetCurrentIndex( ) );

	return;
}



/*
 *	FUNCTION:  Parser_Whilestatement
 */
void Parser_Whilestatement( )
{
	ERR RC = ERR_OK;
	USHORT newaddress;
	USHORT oldaddress;

	_AcceptCurrent( TOK_WHILE_ID, ERR_PARSER_GEN );
	oldaddress = GetCurrentIndex( );
	Parser_Condition( );
	newaddress = AddDPCode( BZE, 0 );
	_AcceptCurrent( TOK_DO_ID, ERR_PARSER_DO_EXPCTD );
	Parser_Statement( );
	AddDPCode( BRN, (SHORT)(oldaddress - 1) );

	UpdatePCode( newaddress, (SHORT)GetCurrentIndex( ) );

	return;
}



/*
 *	FUNCTION:  Parser_Writestatement
 */
void Parser_Writestatement( )
{
	ERR RC = ERR_OK;

	_AcceptCurrent( TOK_WRITE_ID, ERR_PARSER_GEN );
	_GetNextToken;
	if( Lex_State.TokenID == TOK_PO_ID )
	{
		_AcceptCurrent( TOK_PO_ID, ERR_PARSER_GEN );
		Parser_Writelist( );
		_AcceptCurrent( TOK_PC_ID, ERR_PARSER_PARENTC_EXPCTD );
		_GetNextToken;
	}

	AddPCode( NLN );	// print a new line (carriage-return/line-feed) after all the printing is done

	return;
}



/*
 *	FUNCTION:  Parser_Readstatement
 */
void Parser_Readstatement( )
{
	ERR RC = ERR_OK;

	_AcceptCurrent( TOK_READ_ID, ERR_PARSER_GEN );
	_AcceptNext( TOK_PO_ID, ERR_PARSER_PARENTO_EXPCTD );
	Parser_Identsequence( );
	_AcceptCurrent( TOK_PC_ID, ERR_PARSER_PARENTC_EXPCTD );

	return;
}



/*
 *	FUNCTION:  Parser_Identsequence
 */
void Parser_Identsequence( )
{
	ERR RC = ERR_OK;
	BOOL isconst;
	SHORT index;

	do
	{
		_AcceptNext( TOK_IDENTIFIER_ID, ERR_PARSER_IDENT_EXPCTD );
		// is this identifier a constant?  (does it even exist?)
		RC = TableIsConst( &isconst, Lex_State.Token );
		if( RC ) ParserError( RC );
		if( isconst ) ParserError( ERR_PARSER_CONST_REASSIGN );

		// get the index for this identifier
		RC = GetTableEntry( &index, Lex_State.Token );
		if( RC ) ParserError( RC );

		// note the address (index) for this assignment
		AddDPCode( LDI, index );

		// read in the value
		AddPCode( INN );

		_GetNextToken;				// maybe a comma?
	}
	while( Lex_State.TokenID == TOK_COMMA_ID );

	return;
}



/*
 *	FUNCTION:  Parser_Condition
 */
void Parser_Condition( )
{
	ERR RC = ERR_OK;

	_GetNextToken;

	if( Lex_State.TokenID == TOK_ODD_ID )
	{
		_AcceptCurrent( TOK_ODD_ID, ERR_PARSER_GEN );
		_AcceptNext( TOK_PO_ID, ERR_PARSER_PARENTO_EXPCTD );
		_GetNextToken;
		Parser_Expression( );
		_AcceptCurrent( TOK_PC_ID, ERR_PARSER_PARENTC_EXPCTD );
		AddPCode( OD );
		_GetNextToken;
	}
	else
	{
		Parser_Expression( );
		_AcceptCurrent( TOK_RELOP_ID, ERR_PARSER_RELOP_EXPCTD );
		switch( Lex_State.LexemeID )
		{
			case LEX_EQ_ID:
			{
				_GetNextToken;
				Parser_Expression( );
				AddPCode( EQL );
				break;
			}
			case LEX_NE_ID:
			{
				_GetNextToken;
				Parser_Expression( );
				AddPCode( NEQ );
				break;
			}
			case LEX_LE_ID:
			{
				_GetNextToken;
				Parser_Expression( );
				AddPCode( LEQ );
				break;
			}
			case LEX_LT_ID:
			{
				_GetNextToken;
				Parser_Expression( );
				AddPCode( LSS );
				break;
			}
			case LEX_GE_ID:
			{
				_GetNextToken;
				Parser_Expression( );
				AddPCode( GEQ );
				break;
			}
			case LEX_GT_ID:
			{
				_GetNextToken;
				Parser_Expression( );
				AddPCode( GTR );
				break;
			}
		}
	}

	return;
}



/*
 *	FUNCTION:  Parser_Writelist
 */
void Parser_Writelist( )
{
	ERR RC = ERR_OK;
	_GetNextToken;

	do
	{
		if( Lex_State.TokenID == TOK_COMMA_ID )
		{
			_AcceptCurrent( TOK_COMMA_ID, ERR_PARSER_GEN );
			_GetNextToken;
		}

		switch( Lex_State.TokenID )
		{
			case TOK_STRING_ID:
			{
				SHORT stringlen = 0;
				while( Lex_State.Token[stringlen] != '\0' )
				{
					AddDPCode( LDI, (SHORT)Lex_State.Token[stringlen] );
					stringlen ++;
				}
				AddDPCode( LDI, stringlen );
				AddPCode( PRS );			// print the string
				_GetNextToken;
				break;
			}
			case TOK_IDENTIFIER_ID:
			{
				switch( Lex_State.LexemeID )
				{
					case LEX_VALUE_ID:	// token is a numeric value
					{
						AddDPCode( LDI, Lex_State.VarID );
						break;
					}
					case LEX_VAR_ID:		// token is a variable
					{
						SHORT index;
						// get the index for this identifier
						RC = GetTableEntry( &index, Lex_State.Token );
						if( RC ) ParserError( RC );
						// note the address (index) for this assignment
						AddDPCode( LDI, index );
						AddPCode( LDV );	// dereference the address
						break;
					}
				}
				AddPCode( PRN );			// print the bugger
				_GetNextToken;
				break;
			}
			default:
			{
				Parser_Expression( );
				AddPCode( PRN );			// print the value
				break;
			}
		}
	}
	while( Lex_State.TokenID == TOK_COMMA_ID );

	return;
}



#if( _PARSER_DEBUG )
/*
 *	FUNCTION:  InitParserDebug
 */
ERR InitParserDebug( BOOL screen, BOOL source )
{
	ERR RC = ERR_OK;

	Parser_Debug.ScreenOutput = screen;
	Parser_Debug.FileOutput = source;

	if( Parser_Debug.FileOutput )
	{
		if( ! ( ParserDebugFilePtr = fopen( PARSER_DEBUG_FILENAME, "wt" ) ) ) ParserError( ERR_PARSER_DEBUG_FILE_ERROR );
	}

	ParserDebugWrite( "\n%s\n", _PARSER_VERSION );
	if( Parser_Debug.ScreenOutput ) ParserDebugWrite( "=> screen output enabled.\n" );
		else ParserDebugWrite( "=> screen output disabled.\n" );
	if( Parser_Debug.FileOutput ) ParserDebugWrite( "=> output to file \"%s\" enabled.\n", PARSER_DEBUG_FILENAME );
		else ParserDebugWrite( "=> output to file disabled.\n" );

	return RC;
}
#endif



#if( _PARSER_DEBUG )
/*
 *	FUNCTION:  ResetParserDebug
 */
ERR ResetParserDebug( )
{
	ERR RC = ERR_OK;

	if( Parser_State.Error ) ParserDebugWrite( "\n\nError encountered in parser:  %i\n\a", Parser_State.Error );

	if( Parser_Debug.FileOutput ) fclose( ParserDebugFilePtr );

	return RC;
}
#endif



#if( _PARSER_DEBUG )
/*
 *	FUNCTION:	ParserDebugWrite
 */
ERR ParserDebugWrite( UCHAR *text, ... )
{
	ERR RC = ERR_OK;

	va_list ptr;					/* va is a variable argument type implemented in newer revisions */
	va_start( ptr, text );		/* of ANSI C.  Support library includes vfprintf, vprintf, vsprintf */

	// if screen debug output is enabled...
	if( Parser_Debug.ScreenOutput ) vprintf( text, ptr );								// ...then output to screen

	// if file debug output is enabled...
	if( Parser_Debug.FileOutput ) vfprintf( ParserDebugFilePtr, text, ptr );	// ...then output to file

	va_end( ptr );

	return RC;
}
#endif

