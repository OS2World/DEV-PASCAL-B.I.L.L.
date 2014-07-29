program Testing;

var
first, second, third;

const
fourth=4;
fifth=5;

begin

	write( 'Testing ODD function' );
	if odd( 1 ) then
	write( 'OK' );
	if odd( 2 ) then
	write( 'ODD ERROR' );
	write;

	write( 'Testing WHILE function (5 loops)' );
	first := 0;
	while first < fifth do
		begin
			first := first + 1;
			write( ( fifth - first ), ' loops left...' );
		end;

	write;

	write( 'Testing variable assignment' );
	write( 'Please enter a value:' );
	read( first );
	second := first;
	write( 'The value you entered is ', second );
	write;

	write( 'Testing arithmatic functions' );

	write( ' - additions and subtractions' );
	first := ( 2 + 2 ) - 4;
	if first <> 0 then write( '   first set ERROR (', first, ')' );
		if first = 0 then write( '   first set ok' );
	first := 2 + ( 2 - 4 );
	if first <> 0 then write( '   second set ERROR (', first, ')' );
	if first = 0 then write( '   second set ok' );

	write( ' - multiplication and division' );
	first := ( 2 * 4 ) / 2;
	if first <> 4 then write( '   first set ERROR (', first, ')' );
	if first = 4 then write( '   first set ok' );
	first := 2 * ( 4 / 2 );
	if first <> 4 then write( '   second set ERROR (', first, ')' );
	if first = 4 then write( '   second set ok' );

	write( ' - all four operators' );
	first := 2 + 2 * 2 - 4 / 2;
	if first <> 4 then write( '   first set ERROR (', first, ')' );
	if first = 4 then write( '   first set ok' );
	first := ( 2 + 2 ) * 2 - 4 / 2;
	if first <> 6 then write( '   second set ERROR (', first, ')' );
	if first = 6 then write( '   second set ok' );
end.
