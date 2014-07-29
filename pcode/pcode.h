/*
 *		C . A . P .   P C O D E   G E N E R A T O R
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
 *		Group:	pcode
 *		File:		pcode\pcode.h
 *
 *		This file contains all of the global definitions used with the pcode
 *		generator of the interpreter BILL.
 */


#ifndef _PCODE_H_GLOBAL

	#define _PCODE_H_GLOBAL

	/*
	 *		Includes
	 */
		#include "..\global\global.h"



	/*
	 *		Defines
	 */
		#define PCODE_MAX_LEN	10000



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
		ERR StartPCodeGenerator( void );
		USHORT AddPCode( SHORT code );
		USHORT AddDPCode( SHORT code1, SHORT code2 );
		USHORT UpdatePCode( USHORT address, SHORT newcode );
		USHORT GetCurrentIndex( void );
		ERR WritePCodeBuffer( void );



#endif

