What this is:

B.I.L.L. is a simple Pascal interpreter which I wrote for my compiler class
at university.  Note that this interpreter does *NOT* support the full Turbo
Pascal set (as opposed to Virtual Pascal and Speed Pascal which do).
Regardless of the shortcomings, I thought it may come in handy for someone
who might want a tokenizer and didn't want to start from scratch, etc...

===========================================================================

HOW TO USE THESE FILES

The directories are as follows:

1) bill -> contains the file that has main()

2) exitcode -> has the functions which deal with error messages

3) global -> global definitions used throughout the project, such as the level
             of debugging output to include when compiling, etc

4) lexical -> tokenizer (configured for Pascal, but could be easily changed)

5) parser ->parser for a limited subset of standard Pascal

6) pcode -> routines to handle to generation of pcode

7) stackman -> stack machine which executes the pcode (this module was
               created as an independent unit so it could be made as a
               "run-time library" used to execute Pascal pcode files)

8) symtable -> symble-table routines (used with CONST and VAR)

===========================================================================

That's about it.  Send me mail for more info!
St‚phane Charette, 95Oct29.

===========================================================================

St‚phane Charette
95 Oxford, Apt. 102
Lennoxville, Qu‚bec
J1M 2G3
(819) 820-7522

ch930116@arus.ubishops.ca
  -or-
root@cyniska.ubishops.ca

(one of the above address, phone number or email addresses should work...)

