#include <SDL3/SDL.h>
#include "../client/client.h"

unsigned sys_frame_time;

void Sys_Sleep( unsigned int millis )
{
	SDL_Delay( millis );
}

void Sys_Error( const char *format, ... )
{
	va_list argptr;
	char msg[1024];

	va_start( argptr, format );
	Q_vsnprintfz( msg, sizeof( msg ), format, argptr );
	va_end( argptr );

	SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, APPLICATION, msg, NULL );

	exit( 1 );
}

/*
* Sys_Init
*/
void Sys_Init( void )
{
}

/*
* Sys_InitDynvars
*/
void Sys_InitDynvars( void )
{
}

/*
* Sys_Quit
*/
void Sys_Quit( void )
{
	Qcommon_Shutdown();

	exit( 0 );
}

/*
* Sys_AcquireWakeLock
*/
void *Sys_AcquireWakeLock( void )
{
	return NULL;
}

/*
* Sys_ReleaseWakeLock
*/
void Sys_ReleaseWakeLock( void *wl )
{
}

/*
* Sys_AppActivate
*/
void Sys_AppActivate( void )
{
}

/*
* Sys_SendKeyEvents
*/
void Sys_SendKeyEvents( void )
{
	// grab frame time
	sys_frame_time = Sys_Milliseconds();
}

char *Sys_ConsoleInput( void )
{
	return NULL;
}

void Sys_ConsoleOutput( char *string )
{
}

/*****************************************************************************/

int main( int argc, char **argv )
{
	unsigned int oldtime, newtime, time;

	if( !SDL_SetAppMetadata( APPLICATION_UTF8, APP_VERSION_STR, APPLICATION ) )
		Sys_Error( "SDL_SetAppMetadata: %s", SDL_GetError() );

	// needed to enumerate video modes
	if( !SDL_Init( SDL_INIT_VIDEO ) )
		Sys_Error( "SDL_Init: %s", SDL_GetError() );

	Qcommon_Init( argc, argv );

	oldtime = Sys_Milliseconds();
	while( true ) {
		// find time spent rendering last frame
		do {
			newtime = Sys_Milliseconds();
			time = newtime - oldtime;
			if( time > 0 )
				break;
			Sys_Sleep( 0 );
		} while( 1 );
		oldtime = newtime;

		Qcommon_Frame( time );
	}

	SDL_Quit();
}
