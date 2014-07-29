The following are bugs and errors known to exist in B.I.L.L.:

	1.	Negative symbol not handled correctly if
		used as unary operator.

	2.	The programme name is not recorded in the
		symbol table.  Thus, a variable/constant
		declared to use the same name will not
		generate an error.

	3.	If B.I.L.L. is run on a write-protected drive,
		an access violation will occur due to the
		ouput file having an uninitialized pointer.

	4.	The "IF" statement requires that the condition
		be in parentheses while standard Pascal does
		not require this.


Not yet implemented:

	1.	Procedures and functions

	2.	Basic types (float, string, char)

	3.	Case statement

	4.	ELSE keyword for IF statement

Hope you like it!!!  Remember - it's only a beginning!!!

Stephane Denis Charette
1994, April 29.
