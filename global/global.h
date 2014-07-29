/*
 *		C . A . P .   " B I L L "   I N T E R P R E T E R
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
 *		Group:	none
 *		File:		global\global.h
 *
 *		This file contains all of the global project declarations used with
 *		the interpreter BILL.
 */



#ifndef _GLOBAL_H_GLOBAL

	#define _GLOBAL_H_GLOBAL

	/*
	 *		Basic system definitions
	 */
		#define TRUE						1
		#define true						TRUE
		#define FALSE						0
		#define false						FALSE
		
		typedef signed char				BYTE;
		typedef unsigned char			UBYTE;
		typedef unsigned char			UCHAR;
		typedef signed int				SHORT;
		typedef unsigned int				USHORT;
		typedef signed long int			LONG;
		typedef unsigned long int		ULONG;
		typedef USHORT						ERR;
		typedef BYTE						BOOL;



	/*
	 *		System values
	 */
		#define _MAX_LENGTH_OF_PATH	512		// maximum length of path for all file references (min of 8+1+3=12)



	/*
	 *		Debug information include/exclude
	 */
		#define _LEX_DEBUG				false		// include lexical debug source code in .OBJ
		#define _SYMTABLE_DEBUG			false		// include symbol table debug source code in .OBJ
		#define _PARSER_DEBUG			false		// include parser debug source code in .OBJ



	/*
	 *		Include the list of error codes
	 */
		#include "..\global\error.h"
		#include "..\exitcode\exitcode.h"



	/*
	 *		Types and structures
	 */



	/*
	 *		Macros
	 */



#endif

