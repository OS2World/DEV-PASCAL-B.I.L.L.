/*
 *		C . A . P .   P C O D E   G E N E R A T O R
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
 *		Group:	pcode
 *		File:		pcode\pcode.c
 *		Version:	0.1.1
 *
 *		This file contains all of the source code that makes up the pcode
 *		generator used with the interpreter BILL.
 */



/*
 *	Versions:
 *
 *	0.0.1	- design of structure and implementation, St‚phane Charette, 94Apr10
 *	0.1.0	- first working version, SC, 94Apr10
 *	0.1.1	- fixed bug while outputing file, SC, 94Apr11
 * 0.1.1 - changed text formatting, SC, 95Oct29
 */
 #define _PCODE_VERSION "Parser v0.1.1, St‚phane Charette, 95October29\n"



/*
 *	Includes
 */
	#include "..\pcode\pcode.hi"		// internal pcode include file



/*
 *	FUNCTION:  StartPCodeGenerator
 */
ERR StartPCodeGenerator( )
	{
		PCode_State.CurrentIndex = 0;	// start at the beginning of the buffer

		return ERR_OK;
	}



/*
 *	FUNCTION:  PCodeError
 */
void PCodeError( )
{
	printf(	"\nOh dear!  It seems you have run into the most foul of the worst type of errors\n"
				"that have come to pass across this wonderful Earth of ours.  You can either\n"
				"call it quits now, or attempt to find out what the f**k happened to B.I.L.L.\n"
				"Should you be interested, this error is the result of the pcode buffer being\n"
				"filled up.  Thus, all you have to do is increase the size of the buffer!\n" );
	printf(	"\nBy the way - THIS IS A FATAL ERROR.  B.I.L.L. is now officially dead!\n" );
	exit( 1 );
}



/*
 *	FUNCTION:  AddPCode
 */
USHORT AddPCode( SHORT code )
{
	if( PCode_State.CurrentIndex == PCODE_MAX_LEN ) PCodeError( );

	PCodeBuffer[PCode_State.CurrentIndex] = code;

	return PCode_State.CurrentIndex++;
}



/*
 *	FUNCTION:  AddDPCode
 */
USHORT AddDPCode( SHORT code1, SHORT code2 )
{
	AddPCode( code1 );

	return AddPCode( code2 );
}



/*
 *	FUNCTION:  UpdatePCode
 */
USHORT UpdatePCode( USHORT address, SHORT newcode )
{
	PCodeBuffer[address] = newcode;

	return address;
}



/*
 *	FUNCTION:  GetCurrentIndex
 */
USHORT GetCurrentIndex( )
{
	return PCode_State.CurrentIndex;
}



/*
 *	FUNCTION:  WritePCodeBuffer
 */
ERR WritePCodeBuffer( )
{
	USHORT i = 0;					// tmp index counter
	FILE *PCFilePtr;				// file pointer

	PCFilePtr = fopen( "PCODE.LST", "wb" );	// open the pcode output file

	while( i < PCode_State.CurrentIndex ) fprintf( PCFilePtr, "%i\n", PCodeBuffer[i++] );

	fclose( PCFilePtr );			// close the pcode output file

	return ERR_OK;
}

