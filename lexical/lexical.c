/*
 *		C . A . P .   L E X I C A L   A N A L Y Z E R
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
 *		Group:	lexical analyzer
 *		File:		lexical\lexical.c
 *		Version:	0.1.4
 *
 *		This file contains all of the source code that makes up the lexical
 *		analyzer portion of the interpreter BILL.
 */



/*
 *	Versions:
 *
 *	0.0.1	- design of structure and implementation, St‚phane Charette, 94Feb26-Mar1
 *	0.1.0	- first working version, SC, 94Mar2
 *	0.1.1	- added debug information, SC, 94Mar2-3
 *	0.1.2	- added set of illegal characters, SC, 94Mar24
 *	0.1.2	- added more comments, optimized debug, SC, 94Mar27-Apr10
 *	0.1.3	- fixed bug while detecting comments, SC, 94Mar13
 *	0.1.3	- fixed bug while detecting not-equal "<>" symbol, SC, 94Mar14
 *	0.1.4	- ported to OS/2, SC, 94Apr23-27
 * 0.1.4 - changed text formatting, SC, 95Oct29
 */
 #define _LEX_VERSION "Lexical analyzer v0.1.4, St‚phane Charette, 95Oct29\n"


/*
 *	Includes
 */
	#include "..\lexical\lexical.hi"		// internal lexical analyzer include file



/*
 *	FUNCTION:  InitLex
 */
ERR InitLex( UCHAR path[] )
{
	ERR RC = ERR_OK;

	#if( _LEX_DEBUG )
	Lex_Debug.ScreenOutput = FALSE;		// pre-initialize debug value
	Lex_Debug.SourceOutput = FALSE;		// pre-initialize debug value
	Lex_Debug.TokenOutput = FALSE;		// pre-initialize debug value
	Lex_Debug.MixOutput = FALSE;			// pre-initialize debug value
	#endif

	Lex_State.InComment = FALSE;			// are we in a comment?
	Lex_State.InString = FALSE;			// are we in a string?
	Lex_State.LineNumber = 0;				// current line number
	Lex_State.CharOffset = 0;				// character offset into current line
	Lex_State.Error = ERR_OK;				// last error
	Lex_State.TokenID = TOK_NO_ID; 		// ID of found token
	Lex_State.LexemeID = LEX_NO_ID;		// Actual lexeme if ID is generic
	Lex_State.VarID = 0;						// variable reference
	strcpy( Lex_State.Token, "" );		// Actual token

	// open the source file
	if( strlen( path ) > LEX_MAX_PATH_LEN ) LexError( ERR_LEX_PATH_TOO_LONG );						//	is the path valid?
	strcpy( FileName, path );																		//	save the path of file
	if( ( FilePtr = fopen( FileName, "r" ) ) == NULL ) LexError( ERR_LEX_CANNOT_OPEN_FILE );		//	open the file

	// initialize the look-ahead buffer
	strcpy( Buffer, " \n" );			// set the previous and current character to <SPACE><NEWLINE>
	if( ! fread( &( Buffer[ 2 ] ), 1, LEX_MAX_BUFFER_LEN - 2, FilePtr ) ) LexError( ERR_LEX_CANNOT_READ_FILE );	// read file

	return RC;
}



/*
 *	FUNCTION:  ResetLex
 */
ERR ResetLex( )
{
	ERR RC = ERR_OK;

	if( fclose( FilePtr ) ) RC = ERR_LEX_CANNOT_CLOSE_FILE;

	return RC;
}



/*
 *	FUNCTION:  LexError
 */
void LexError( ERR error_code )
{
	Lex_State.Error = error_code;

	printf( "\nError encountered in lex:  %i\n\a", Lex_State.Error );

	ResetLex( );								// perform shutdown procedures

	ExitCode( error_code );					// return to operating system
}



/*
 *	FUNCTION:  GetNextCharacter
 */
ERR GetNextCharacter( )
{
	ERR RC = ERR_OK;

	strncpy( Buffer, &( Buffer[ 1 ] ), ( LEX_MAX_BUFFER_LEN - 1 ) );		// shift the buffer down by one

	fread( &( Buffer[ LEX_MAX_BUFFER_LEN - 1 ] ), 1, 1, FilePtr );			// read the next character

	// find out if an EOF occurred
	if( feof( FilePtr ) )
	{
		// could not read character, so use a space instead
		Buffer[ LEX_MAX_BUFFER_LEN - 1 ] = ' ';

		RC = Lex_State.Error = ERR_LEX_END_OF_FILE;	// indicate that the eof was reached
	}

	// find out if a file error occurred
	if( ferror( FilePtr ) )
	{
		// could not read character, so use a space instead
		Buffer[ LEX_MAX_BUFFER_LEN - 1 ] = ' ';

		RC = Lex_State.Error = ERR_LEX_CANNOT_READ_FILE;	// indicate that an actual file read error occurred
	}

	if( ( Buffer[ 0 ] == '\r' ) || ( Buffer[ 0 ] == '\n' ) )
	{										// if the previous character read was an EOL character...
		Lex_State.LineNumber ++;	// ...then increase the line counter by one,
		Lex_State.CharOffset = 1;	// ...and reset the character counter to the start of the line
		#if( _LEX_DEBUG )
		LexDebugWrite( "\n(%3i) ", Lex_State.LineNumber );
		#endif
	}
	else
	{										// ...else...
		Lex_State.CharOffset ++;	// ...increase the character offset into this line
		#if( _LEX_DEBUG )
		if( Lex_Debug.ScreenOutput ) printf( "%c", Buffer[ 0 ] );
		if( Lex_Debug.SourceOutput ) fprintf( LexDebugSourceFilePtr, "%c", Buffer[ 0 ] );
		if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, "%c", Buffer[ 0 ] );
		#endif
	}

	return RC;
}



/*
 *	FUNCTION:  GetNextToken
 */
ERR GetNextToken( )
{
	ERR RC = ERR_OK;

	Lex_State.TokenID = TOK_NO_ID;
	Lex_State.LexemeID = LEX_NO_ID;
	Lex_State.VarID = SYM_NO_ID;
	strcpy( Lex_State.Token, "" );

	// skip the white space - if any - at the current position in Buffer
	if( isspace( Buffer[ 1 ] ) ) RC = SkipWhiteSpace( );

	while( Lex_State.TokenID == TOK_NO_ID )
	{
		if( ( ( ( Lex_State.InString == FALSE ) && ( Lex_State.InComment == FALSE ) ) &&
			( strchr( SET_OF_TOKEN_DELIMITERS, Buffer[ 1 ] ) || strchr( SET_OF_ILLEGAL_CHARS, Buffer[ 1 ] ) ) ) ||
			( ( Lex_State.InComment == TRUE ) && ( Buffer[ 1 ] == '}' ) ) ||
			( ( Lex_State.InString == TRUE ) && ( Buffer[ 1 ] == '\'' ) ) )
		{
			// token delimiter found - now try and recognize the token

			// is this a single character delimiter, or does this indicate the end of a previous token?
			if( Lex_State.Token[ 0 ] == '\0' )
			{
				// this indicates we've just read a single character delimiter
				switch( Buffer[ 1 ] )
				{
					case '{':	// don't do anything with these - the next step will
					case '}':	// take care of handling them correctly
						break;
					case ';':
						Lex_State.TokenID = TOK_SEMICOLON_ID;	// semicolon recognized
						strcpy( Lex_State.Token, ";" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case ',':
						Lex_State.TokenID = TOK_COMMA_ID;		// comma recognized
						strcpy( Lex_State.Token, "," );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case '.':
						Lex_State.TokenID = TOK_PERIOD_ID;		// period recognized
						strcpy( Lex_State.Token, "." );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						if( RC = ERR_LEX_END_OF_FILE ) RC = ERR_OK;	// give the guy a break...  :)
						break;
					case '+':
						Lex_State.TokenID = TOK_PLUS_ID;			// plus recognized
						strcpy( Lex_State.Token, "+" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case '-':
						Lex_State.TokenID = TOK_MINUS_ID;		// minus recognized
						strcpy( Lex_State.Token, "-" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case '*':
						Lex_State.TokenID = TOK_MULT_ID;			// multiply recognized
						strcpy( Lex_State.Token, "*" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case '/':
						Lex_State.TokenID = TOK_DIV_ID;			// divide recognized
						strcpy( Lex_State.Token, "/" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case '(':
						Lex_State.TokenID = TOK_PO_ID;			// parenteses open recognized
						strcpy( Lex_State.Token, "(" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case ')':
						Lex_State.TokenID = TOK_PC_ID;			// parenteses close recognized
						strcpy( Lex_State.Token, ")" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					case '\'':
						break;											// pass on the single quotes
					case '<':
						if( Buffer[ 2 ] == '=' )
						{
							Lex_State.TokenID = TOK_RELOP_ID;	// relational operator recognized
							Lex_State.LexemeID = LEX_LE_ID;		// less_than_or_equal_to lexeme recognized
							strcpy( Lex_State.Token, "<=" );		// make a copy of the token
							// move ahead two characters since this is a two-char token
							if( ( RC = GetNextCharacter( ) ) == ERR_OK ) RC = GetNextCharacter( );
						}
						else
						{
							if( Buffer[ 2 ] == '>' )
							{
								Lex_State.TokenID = TOK_RELOP_ID;
								Lex_State.LexemeID = LEX_NE_ID;
								strcpy( Lex_State.Token, "<>" );
								if( ( RC = GetNextCharacter( ) ) == ERR_OK ) RC = GetNextCharacter( );
							}
							else
							{
								Lex_State.TokenID = TOK_RELOP_ID;	// relational operator recognized
								Lex_State.LexemeID = LEX_LT_ID;	// less_than lexeme recognized
								strcpy( Lex_State.Token, "<" );	// make a copy of the token
								RC = GetNextCharacter( );			// get next character
							}
						}
						break;
					case '>':
						if( Buffer[ 2 ] == '=' )
						{
							Lex_State.TokenID = TOK_RELOP_ID;	// relational operator recognized
							Lex_State.LexemeID = LEX_GE_ID;		// greater_than_or_equal_to lexeme recognized
							strcpy( Lex_State.Token, ">=" );		// make a copy of the token
							// move ahead two characters since this is a two-char token
							if( ( RC = GetNextCharacter( ) ) == ERR_OK ) RC = GetNextCharacter( );
						}
						else
						{
							Lex_State.TokenID = TOK_RELOP_ID;	// relational operator recognized
							Lex_State.LexemeID = LEX_GT_ID;		// greater_than lexeme recognized
							strcpy( Lex_State.Token, ">" );		// make a copy of the token
							RC = GetNextCharacter( );				// get next character
						}
						break;
					case ':':
						if( Buffer[ 2 ] == '=' )
						{
							Lex_State.TokenID = TOK_ASSIGNMENT_ID;	// assignment recognized
							strcpy( Lex_State.Token, ":=" ); 		// make a copy of the token
							// move ahead two characters since this is a two-char token
							if( ( RC = GetNextCharacter( ) ) == ERR_OK ) RC = GetNextCharacter( );
						}
						else
						{
							RC = Lex_State.Error = ERR_LEX_UNRECOGNIZED_CHAR;
							Lex_State.TokenID = TOK_ERROR_ID;
							strcpy( Lex_State.Token, ":" );		// make a copy of the token
						}
						break;
					case '=':
						Lex_State.TokenID = TOK_RELOP_ID;		// relational operator recognized
						Lex_State.LexemeID = LEX_EQ_ID;			// equal lexeme recognized
						strcpy( Lex_State.Token, "=" );			// make a copy of the token
						RC = GetNextCharacter( );					// get next character
						break;
					default:
						RC = Lex_State.Error = ERR_LEX_UNRECOGNIZED_CHAR;	// set an error flag
						Lex_State.TokenID = TOK_ERROR_ID;		// set an error flag
						Lex_State.Token[ 0 ] = Buffer[ 1 ];		// make a copy of the unrecognized character
						Lex_State.Token[ 1 ] = '\0';
						break;
				}
			}

			// was our token one of the previous single-character tokens?
			if( Lex_State.TokenID == TOK_NO_ID )
			{	// no...
				// ...then see if it's a special delimiter, such as ' (quote) or {} (curly braces)
				switch( Buffer[ 1 ] )
				{
					case '\'':											// string delimiter found...
						if( Lex_State.InString )
						{
							Lex_State.InString = FALSE;			// ...clear string flag
							Lex_State.TokenID = TOK_STRING_ID;	// set the token to "string"
							Lex_State.LexemeID = LEX_VALUE_ID;	// set the lexeme to "string"
							GetNextCharacter( );						// ...get next character
							break;
						}
						else
						{
							Lex_State.InString = TRUE;				// ...set string flag
							GetNextCharacter( );						// ...get next character
							break;
						}
					case '{':											// start of comment block found...
						Lex_State.InComment ++;						// ...increase comment block counter
						break;
					case '}':											// end of comment block found...
						Lex_State.InComment --;						// ...decrease comment block counter
						if( Lex_State.InComment < 0 ) LexError( ERR_LEX_CLOSE_COMMENT_BLOCK );	// comment close without open
						// skip the white space - if any - after the end of the comment
						if( isspace( Buffer[ 2 ] ) )
						{
							RC = GetNextCharacter( );
							SkipWhiteSpace( );
						}
						break;
					default:
						// since it wasn't a special token, now try and recongnize it as a reserved word
						if( strcmp( Lex_State.Token, "program" ) == 0 ) Lex_State.TokenID = TOK_PROGRAM_ID;
						if( strcmp( Lex_State.Token, "const" ) == 0 ) Lex_State.TokenID = TOK_CONST_ID;
						if( strcmp( Lex_State.Token, "var" ) == 0 ) Lex_State.TokenID = TOK_VAR_ID;
						if( strcmp( Lex_State.Token, "begin" ) == 0 ) Lex_State.TokenID = TOK_BEGIN_ID;
						if( strcmp( Lex_State.Token, "end" ) == 0 ) Lex_State.TokenID = TOK_END_ID;
						if( strcmp( Lex_State.Token, "if" ) == 0 ) Lex_State.TokenID = TOK_IF_ID;
						if( strcmp( Lex_State.Token, "then" ) == 0 ) Lex_State.TokenID = TOK_THEN_ID;
						if( strcmp( Lex_State.Token, "while" ) == 0 ) Lex_State.TokenID = TOK_WHILE_ID;
						if( strcmp( Lex_State.Token, "do" ) == 0 ) Lex_State.TokenID = TOK_DO_ID;
						if( strcmp( Lex_State.Token, "write" ) == 0 ) Lex_State.TokenID = TOK_WRITE_ID;
						if( strcmp( Lex_State.Token, "read" ) == 0 ) Lex_State.TokenID = TOK_READ_ID;
						if( strcmp( Lex_State.Token, "odd" ) == 0 ) Lex_State.TokenID = TOK_ODD_ID;
						if( Lex_State.TokenID == TOK_NO_ID )
						{
							// token was not recognized as a reserved work
							if( isdigit( Lex_State.Token[ 0 ] ) )
							{
								// token recognized as a numerical value
								Lex_State.TokenID = TOK_IDENTIFIER_ID;
								Lex_State.LexemeID = LEX_VALUE_ID;
								sscanf( Lex_State.Token, "%i", &( Lex_State.VarID ) );
							}
							else
							{
								// token must be a variable name
								Lex_State.TokenID = TOK_IDENTIFIER_ID;
								Lex_State.LexemeID = LEX_VAR_ID;
								Lex_State.VarID = SYM_NO_ID;
							}
						}
						break;
				}
			}
		}
		else		// ...no delimiter found yet...
		{
			if( ! Lex_State.InComment )							// if we aren't in the middle of a comment, then...
			{
				UCHAR tmpbuffer[ 2 ] = " ";						// temporary buffer
				tmpbuffer[ 0 ] = Buffer[ 1 ];						// hold current character in temporary buffer

				// convert to lowercase if not currently analyzing string
				if( ! Lex_State.InString ) tmpbuffer[ 0 ] = (UCHAR)tolower( tmpbuffer[ 0 ] );

				// append the current character to the token
				if( strlen( Lex_State.Token ) == LEX_MAX_TOKEN_LEN )
				{
					// token is too long to fit in Lex_State.Token[] - generate error
					Lex_State.Error = ERR_LEX_TOKEN_TOO_LONG;
					break;
				}
				else
				{
					// add the new character to Lex_State.Token[]
					strcat( Lex_State.Token, tmpbuffer );
				}

			}

			GetNextCharacter( );				// get a new character

			if( ( Lex_State.Error == ERR_LEX_END_OF_FILE ) &&
				( ( Lex_State.InComment && ( Buffer[ 1 ] != '}' ) ) ||
					( Lex_State.InString && ( Buffer[ 1 ] != '\'' ) ) ) )
			{
				// this most likely means unbalanced comments or strings
				Lex_State.Error = ERR_LEX_UNEXPECTED_EOF;
				break;
			}
		}
	}

	// we've just found a token - return to calling programme

	#if( _LEX_DEBUG )
	if( Lex_State.TokenID == TOK_ERROR_ID )
	{	// if there's an error, print the offending character
		if( Lex_Debug.ScreenOutput ) printf( "%c", Buffer[ 1 ] );
		if( Lex_Debug.SourceOutput ) fprintf( LexDebugSourceFilePtr, "%c", Buffer[ 1 ] );
		if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, "%c", Buffer[ 1 ] );
	}
	if( ( Lex_Debug.TokenOutput ) || ( Lex_Debug.ScreenOutput ) )
	{
		//if( Lex_Debug.ScreenOutput ) printf( "[T%i", Lex_State.TokenID );
		if( Lex_Debug.TokenOutput ) fprintf( LexDebugTokenFilePtr, "[T%i", Lex_State.TokenID );
		if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, "[T%i", Lex_State.TokenID );

		if( Lex_State.TokenID == TOK_IDENTIFIER_ID )
		{	// token is a variable
			if( Lex_State.LexemeID == LEX_VAR_ID )
			{	// token is a symbol table entry
				//if( Lex_Debug.ScreenOutput ) printf( ":L%i:S%i]", Lex_State.LexemeID, Lex_State.VarID );
				if( Lex_Debug.TokenOutput ) fprintf( LexDebugTokenFilePtr, ":L%i:S%i]", Lex_State.LexemeID, Lex_State.VarID );
				if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, ":L%i:S%i]", Lex_State.LexemeID, Lex_State.VarID );
			}
			else
			{	// token is a numeric value
				//if( Lex_Debug.ScreenOutput ) printf( ":L%i:Val%i]", Lex_State.LexemeID, Lex_State.VarID );
				if( Lex_Debug.TokenOutput ) fprintf( LexDebugTokenFilePtr, ":L%i:Val%i]", Lex_State.LexemeID, Lex_State.VarID );
				if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, ":L%i:Val%i]", Lex_State.LexemeID, Lex_State.VarID );
			}
		}
		else
		{	// token is a string, reserved word or operator
			if( Lex_State.LexemeID )
			{	// token is an operator
				//if( Lex_Debug.ScreenOutput ) printf( ":L%i]", Lex_State.LexemeID );
				if( Lex_Debug.TokenOutput ) fprintf( LexDebugTokenFilePtr, ":L%i]", Lex_State.LexemeID );
				if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, ":L%i]", Lex_State.LexemeID );
			}
			else
			{	// token is a reserved word
				//if( Lex_Debug.ScreenOutput ) printf( "]" );
				if( Lex_Debug.TokenOutput ) fprintf( LexDebugTokenFilePtr, "]" );
				if( Lex_Debug.MixOutput ) fprintf( LexDebugMixFilePtr, "]" );
			}
		}
	}
	#endif

	return RC;

}



/*
 *
 */
ERR SkipWhiteSpace( )
{
	ERR RC = ERR_OK;

	while( isspace( Buffer[ 1 ] ) )
	{
		// find the next non-white character
		GetNextCharacter( );
		if( Lex_State.Error )
		{
			if( Lex_State.Error != ERR_LEX_END_OF_FILE )
			{
				LexError( Lex_State.Error );
			}
			else
			{
				// eof was reached, so stop looking for non-white characters
				Lex_State.TokenID = TOK_END_OF_FILE_ID;
				break;
			}
		}
	}
	return RC;
}



#if( _LEX_DEBUG )
/*
 *	FUNCTIN:  InitLexDebug
 */
ERR InitLexDebug( BOOL screen, BOOL source, BOOL token, BOOL mix )
{
	ERR RC = ERR_OK;

	Lex_Debug.ScreenOutput = screen;
	Lex_Debug.SourceOutput = source;
	Lex_Debug.TokenOutput = token;
	Lex_Debug.MixOutput = mix;

	if( Lex_Debug.SourceOutput )
	{
		if( ! ( LexDebugSourceFilePtr = fopen( LEX_DEBUG_SOURCE_FILENAME, "wt" ) ) ) LexError( ERR_LEX_DEBUG_FILE_ERROR );
	}

	if( Lex_Debug.TokenOutput )
	{
		if( ! ( LexDebugTokenFilePtr = fopen( LEX_DEBUG_TOKEN_FILENAME, "wt" ) ) ) LexError( ERR_LEX_DEBUG_FILE_ERROR );
	}

	if( Lex_Debug.MixOutput )
	{
		if( ! ( LexDebugMixFilePtr = fopen( LEX_DEBUG_MIX_FILENAME, "wt" ) ) ) LexError( ERR_LEX_DEBUG_FILE_ERROR );
	}

	LexDebugWrite( "\n%s\n", _LEX_VERSION );
	if( Lex_Debug.ScreenOutput ) LexDebugWrite( "=> screen output enabled.\n" );
		else LexDebugWrite( "=> screen output disabled.\n" );
	if( Lex_Debug.SourceOutput ) LexDebugWrite( "=> source output to file \"%s\" enabled.\n", LEX_DEBUG_SOURCE_FILENAME );
		else LexDebugWrite( "=> source output to file disabled.\n" );
	if( Lex_Debug.TokenOutput ) LexDebugWrite( "=> token output to file \"%s\" enabled.\n", LEX_DEBUG_TOKEN_FILENAME );
		else LexDebugWrite( "=> token output to file disabled.\n" );
	if( Lex_Debug.MixOutput ) LexDebugWrite( "=> mix output to file \"%s\" enabled.\n", LEX_DEBUG_MIX_FILENAME );
		else LexDebugWrite( "=> mix output to file disabled.\n" );

	return RC;
}
#endif



#if( _LEX_DEBUG )
/*
 *	FUNCTION:  ResetLexDebug
 */
ERR ResetLexDebug( )
{
	ERR RC = ERR_OK;

	if( Lex_State.Error != ERR_LEX_END_OF_FILE )
	{
		LexDebugWrite( "\nSource file:  %s\n", FileName );
		LexDebugWrite( "Line number:  %i\n", Lex_State.LineNumber );
		LexDebugWrite( "Char offset:  %i\n", Lex_State.CharOffset );
	}
	else
	{
		LexDebugWrite( "\n\nFinished reading file %s\n", FileName );
	}

	if( Lex_Debug.SourceOutput ) fclose( LexDebugSourceFilePtr );
	if( Lex_Debug.TokenOutput ) fclose( LexDebugTokenFilePtr );
	if( Lex_Debug.MixOutput ) fclose( LexDebugMixFilePtr );

	return RC;
}
#endif



#if( _LEX_DEBUG )
/*
 *	FUNCTION:	LexDebugWrite
 */
ERR LexDebugWrite( UCHAR *text, ... )
{
	ERR RC = ERR_OK;

	va_list ptr;				/* va is a variable argument type implemented in newer revisions */
	va_start( ptr, text );		/* of ANSI C.  Support library includes vfprintf, vprintf, vsprintf */

	// if screen debug output is enabled...
	if( Lex_Debug.ScreenOutput ) vprintf( text, ptr );							// ...then output to screen

	// if file debug source output is enabled...
	if( Lex_Debug.SourceOutput ) vfprintf( LexDebugSourceFilePtr, text, ptr );	// ...then output to source file

	// if file debug token output is enabled...
	if( Lex_Debug.TokenOutput ) vfprintf( LexDebugTokenFilePtr, text, ptr );	// ...then output to token file

	// if mix debug output is enabled...
	if( Lex_Debug.MixOutput ) vfprintf( LexDebugMixFilePtr, text, ptr );		// ...then ouput to mix file

	va_end( ptr );

	return RC;
}
#endif

