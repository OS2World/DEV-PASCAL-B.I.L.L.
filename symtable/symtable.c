/*
 *		C . A . P .   S Y M B O L   T A B L E
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
 *		Group:	symbol table
 *		File:		symtable\symtable.c
 *		Version:	0.1.5
 *
 *		This file contains all of the source code that makes up the symbol
 *		table used with the interpreter BILL.
 */



/*
 *	Versions:
 *
 *	0.0.1	- design of structure and implementation, St‚phane Charette, 94Mar23-27
 *	0.1.0	- first working version, SC, 94Mar27
 *	0.1.1	- added verification for referencing uninitialized variables, SC, 94Mar27
 *	0.1.2	- added verification for undefined variables, SC, 94Mar27-28
 *	0.1.3	- removed verification for undefined variables, SC, 94Apr9
 *	0.1.3	- added constant variable types, SC, 94Apr9
 *	0.1.4	- fixed bug while looking if entry exists, SC, 94Apr11
 *	0.1.5	- ported to OS/2, SC, 94Apr23-27
 *	0.1.5 - changed text formatting, SC, 95Oct29
 */
 #define _SYMTABLE_VERSION "Symbol table v0.1.5, St‚phane Charette, 95Oct29\n"



/*
 *	Includes
 */
	#include "..\symtable\symtable.hi"			// internal symbol table include file



/*
 *	FUNCTION:  InitSymTable
 */
ERR InitSymTable( )
{
	USHORT i;											// temporary index counter
	ERR RC = ERR_OK;

	#if( _SYMTABLE_DEBUG )
	Symbol_Table_Debug.ScreenOutput = FALSE;	// pre-initialize debug value
	Symbol_Table_Debug.FileOutput = FALSE;		// pre-initialize debug value
	#endif

	SymTableInternalError = ERR_OK;				// internal symbol table error

	// initialize the actual table
	for( i = 0; i < SYMTABLE_MAX_ENTRY; Table[i++].Used = false );

	return RC;
}



/*
 *	FUNCTION:  ResetSymTable
 */
ERR ResetSymTable( )
{
	return ERR_OK;
}



/*
 *	FUNCTION:  SymTableError
 */
void SymTableError( ERR error_code )
{
	SymTableInternalError = error_code;

	#if( _SYMTABLE_DEBUG )
	ResetSymTableDebug( );			// shutdown the debug procedures
	#else
	printf( "\nError encountered in symbol table:  %i\n\a", SymTableInternalError );
	#endif

	ResetSymTable( );					// perform shutdown procedures

	ExitCode( error_code );			// return to operating system
}



/*
 *	FUNCTION:  NewTableEntry
 ***************************************
 *	Creates a new entry in the variable	*
 *	table and assigns it the name that	*
 *	is specified as "name".  Return the	*
 *	index into the table entry using		*
 *	the variable "index".					*
 ***************************************/
ERR NewTableEntry( SHORT *index, UCHAR name[] )
{
	ERR RC = ERR_OK;
	SHORT newid = SYM_NO_ID;	// index of first available symbol in table
	USHORT i;						// tmp counter

	// assume the worst - set the index to indicate an error
	*index = SYM_NO_ID;

	// make certain the requested ID is not already in the table
	for( i = 0; i < SYMTABLE_MAX_ENTRY; i++ )
	{
		if( ( Table[i].Used == false ) && ( newid == SYM_NO_ID ) ) newid = i;	// save this position for the new variable
		if( ( Table[i].Used == true ) && ( stricmp( Table[i].Name, name ) == 0 ) ) newid = SYM_DUPLICATE_ID;	// duplicate!
	}

	if( newid == SYM_NO_ID )	// is the symbol table filled?
	{
		SymTableError( RC = ERR_SYMT_TOO_MANY_IDS );
	}
	else
	{
		if( ( newid >= 0 ) && ( newid < SYMTABLE_MAX_ENTRY ) )	// is the "available" index returned valid?
		{	// initialize the new table entry
			Table[newid].Used = true;
			strcpy( Table[newid].Name, name );
			Table[newid].Constant = false;
			*index = newid;
		}
	}

	return RC;
}



/*
 *	FUNCTION:  GetTableEntry
 ***************************************
 *	Look in the symbol table for a		*
 *	variable called "name" and return	*
 *	it's index using the variable			*
 *	"index".										*
 ***************************************/
ERR GetTableEntry( SHORT *index, UCHAR name[] )
{
	ERR RC = ERR_OK;
	USHORT i;			// tmp counter

	// assume the worst - set the index to indicate we didn't find it
	*index = SYM_NO_ID;

	// find the id in the table
	for( i = 0; i < SYMTABLE_MAX_ENTRY; i++ )
	{
		if( ( Table[i].Used == true ) && ( stricmp( Table[i].Name, name ) == 0 ) )
		{							// found it!
			*index = i;	// keep the index
			break;		// break out of the for loop
		}
	}

	if( i == SYMTABLE_MAX_ENTRY )		// is the value of i past the maximum range?
	{											// ...this would indicate that we didn't find the var
		RC = ERR_SYMT_UNDEFINED_ID;	// ...so return the error
	}

	return RC;
}



/*
 *	FUNCTION:  TableMakeConst
 ***************************************
 *	Look in the symbol table for the		*
 *	variable and change it's attributes	*
 *	so that it's recognized as a const.	*
 ***************************************/
ERR TableMakeConst( UCHAR name[] )
{
	ERR RC = ERR_OK;
	USHORT i;						// tmp counter

	// find the id in the table
	for( i = 0; i < SYMTABLE_MAX_ENTRY; i++ )
	{
		if( ( Table[i].Used == true ) && ( stricmp( Table[i].Name, name ) == 0 ) )
		{								// found it!
			Table[i].Constant = true;	// make it a constant
			break;					// break out of the for loop
		}
	}

	if( i == SYMTABLE_MAX_ENTRY )		// is the value of i past the maximum ranger?
	{											// ...this would indicate that we didn't find the var
		RC = ERR_SYMT_UNDEFINED_ID;	// ...so return the error
	}

	return RC;
}



/*
 *	FUNCTION:  TableIsConst
 ***************************************
 *	Look in the symbol table for the		*
 *	variable and see if it's declared	*
 *	as being a constant.						*
 ***************************************/
ERR TableIsConst( BOOL *isconst, UCHAR name[] )
{
	ERR RC = ERR_OK;
	USHORT i;

	// find the id in the table
	for( i = 0; i < SYMTABLE_MAX_ENTRY; i++ )
	{
		if( ( Table[i].Used == true ) && ( stricmp( Table[i].Name, name ) == 0 ) )
		{										// found it!
			*isconst = Table[i].Constant;	// get the value
			break;							// break out of the for loop
		}
	}

	if( i == SYMTABLE_MAX_ENTRY )		// is the value of i past the maximum ranger?
	{											// ...this would indicate that we didn't find the var
		RC = ERR_SYMT_UNDEFINED_ID;	// ...so return the error
	}

	return RC;
}



#if( _SYMTABLE_DEBUG )
/*
 *	FUNCTION:  InitSymTableDebug
 */
ERR InitSymTableDebug( BOOL screen, BOOL source )
{
	ERR RC = ERR_OK;

	Symbol_Table_Debug.ScreenOutput = screen;
	Symbol_Table_Debug.FileOutput = source;

	if( Symbol_Table_Debug.FileOutput )
	{
		if( ! ( SymTableDebugFilePtr = fopen( SYMTABLE_DEBUG_FILENAME, "wt" ) ) ) SymTableError( ERR_SYMT_DEBUG_FILE_ERROR );
	}

	SymTableDebugWrite( "\n%s\n", _SYMTABLE_VERSION );
	if( Symbol_Table_Debug.ScreenOutput ) SymTableDebugWrite( "=> screen output enabled.\n" );
		else SymTableDebugWrite( "=> screen output disabled.\n" );
	if( Symbol_Table_Debug.FileOutput ) SymTableDebugWrite( "=> output to file \"%s\" enabled.\n", SYMTABLE_DEBUG_FILENAME );
		else SymTableDebugWrite( "=> output to file disabled.\n" );

	return RC;
}
#endif



#if( _SYMTABLE_DEBUG )
/*
 *	FUNCTION:  ResetSymTableDebug
 */
ERR ResetSymTableDebug( )
{
	ERR RC = ERR_OK;

	if( SymTableInternalError ) SymTableDebugWrite( "\nError encountered in symbol table:  %i\n\a", SymTableInternalError );

	if( Symbol_Table_Debug.FileOutput ) fclose( SymTableDebugFilePtr );

	return RC;
}
#endif



#if( _SYMTABLE_DEBUG )
/*
 *	FUNCTION:	SymTableDebugWrite
 */
ERR SymTableDebugWrite( UCHAR *text, ... )
{
	ERR RC = ERR_OK;

	va_list ptr;				/* va is a variable argument type implemented in newer revisions */
	va_start( ptr, text );		/* of ANSI C.  Support library includes vfprintf, vprintf, vsprintf */

	// if screen debug output is enabled...
	if( Symbol_Table_Debug.ScreenOutput ) vprintf( text, ptr );							// ...then output to screen

	// if file debug output is enabled...
	if( Symbol_Table_Debug.FileOutput ) vfprintf( SymTableDebugFilePtr, text, ptr );	// ...then output to file

	va_end( ptr );

	return RC;
}
#endif

