#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include "../client/client.h"

#include <time.h>

#define NS_PER_MS 1000000ULL
#define NS_PER_SEC 1000000000ULL

unsigned sys_frame_time;

uint64_t Sys_GetTimeNS( void ) {
    return SDL_GetTicksNS();
}

void Sys_SleepNS( uint64_t ns ) {
	SDL_DelayPrecise( ns );
}

void Sys_Sleep( unsigned int ms )
{
	SDL_Delay( ms );
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

	SDL_SetHint( SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "1" );

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
