
#include <stdlib.h>
#include <stdio.h>

#include <drmaa_utils/common.h>


void
raise( int err_code )
{
	fsd_exc_raise_code( err_code );
	printf( "after raise\n" );
}


void
runner( void (*function)(void) )
{
	TRY
	 {
		printf( "runner: before\n" );
		function();
		printf( "runner: after\n" );
	 }
	EXCEPT_DEFAULT
	 {
		printf( "runner: except: %s\n", fsd_exc_get()->_message );
	 }
	END_TRY
}


void test_0(void)
{
	TRY
	 { printf( "try\n" ); }
	EXCEPT( FSD_ERRNO_INVALID_VALUE )
	 { printf( "except\n" ); }
	END_TRY
	printf( "after try\n" );
}


void test_1(void)
{
	TRY
	 { printf( "try\n" ); }
	EXCEPT_DEFAULT
	 { printf( "except\n" ); }
	ELSE
	 { printf( "else\n" ); }
	FINALLY
	 { printf( "finally\n" ); }
	END_TRY
}


void test_2(void)
{
	TRY
	 {
		printf( "try\n" );
		raise( FSD_ERRNO_INVALID_VALUE );
	 }
	EXCEPT_DEFAULT
	 {
		printf( "except: %s\n", fsd_exc_get()->_message );
	 }
	FINALLY
	 {
		printf( "finally\n" );
	 }
	END_TRY
}


void test_3(void)
{
	TRY
	 {
		TRY
		 {
			printf( "inner try\n" );
			raise( FSD_ERRNO_INVALID_VALUE );
		 }
		FINALLY
		 {
			printf( "inner finally\n" );
		 }
		END_TRY
		printf( "after\n" );
	 }
	EXCEPT_DEFAULT
	 {
		printf( "except: %s\n", fsd_exc_get()->_message );
		fsd_exc_reraise();
	 }
	ELSE
	 { printf( "else\n" ); }
	FINALLY
	 {
		printf( "finally\n" );
	 }
	END_TRY
}


void test_4(void)
{
	TRY
	 {
		raise( FSD_ERRNO_INVALID_VALUE );
	 }
	EXCEPT( FSD_ERRNO_INVALID_VALUE )
	 { printf( "except: %s\n", fsd_exc_get()->_message ); }
	EXCEPT_DEFAULT
	 { printf( "except_default: %s\n", fsd_exc_get()->_message ); }
	FINALLY
	 {
		printf( "finally\n" );
		raise( FSD_ERRNO_INTERNAL_ERROR );
	 }
	END_TRY
}


int
main( int argc, char *argv[] )
{
	int i;
	for( i=1;  i<argc;  i++ )
	 {
		int test_no = atoi(argv[i]);
		switch( test_no )
		 {
			case 0:  test_0();  break;
			case 1:  test_1();  break;
			case 2:  test_2();  break;
			case 3:  runner( test_3 );  break;
			case 4:  runner( test_4 );  break;
			default:
				fprintf( stderr, "invalid test number: %s\n", argv[i] );
				return 1;
		 }
	 }
	return 0;
}

