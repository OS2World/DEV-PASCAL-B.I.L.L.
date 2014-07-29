/*
 *		C . A . P .   S Y M B O L   T A B L E
 *
 *		G L O B A L   I N C L U D E   F I L E
 *
 *		St‚phane Charette @ C.A.P. Services
 *
 *		Last modified:  Stephane Charette, 1995 October 29
 *
 *****************************************************************************
 *
 *		Project:	BILL
 *		Group:	symbol table
 *		File:		symtable\symtable.h
 *
 *		This file contains all of the global definitions used with the symbol
 *		table portion of the interpreter BILL.
 */



#ifndef _SYMTABLE_H_GLOBAL

	#define _SYMTABLE_H_GLOBAL

	/*
	 *		Includes
	 */
		#include "..\global\global.h"
		#include "..\symtable\symt_ids.h"



	/*
	 *		Defines
	 */
		#define SYMTABLE_MAX_VAR_LEN	31		// maximum length allowed for variable names
		#define SYMTABLE_MAX_ENTRY		200	// maximum number of entries allowed in the symbol table



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
		ERR InitSymTable( void );										// initialize the symbol table
		ERR ResetSymTable( void );										// reset the symbol table
		ERR NewTableEntry( SHORT *index, UCHAR name[] );		// add a new table entry to the symbol table
		ERR GetTableEntry( SHORT *index, UCHAR name[] );		// get the index of a table entry
		ERR TableMakeConst( UCHAR name[] );							// make the specified entry a constant
		ERR TableIsConst( BOOL *isconst, UCHAR name[] );		// check to see if the entry is a constant
		#if( _SYMTABLE_DEBUG )
		ERR InitSymTableDebug( BOOL screen, BOOL source );		// initialize lexical debug module
		ERR ResetSymTableDebug( void );								// reset the lexical debug module
		#endif



#endif

