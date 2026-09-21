/*
   Copyright (C) 1997-2001 Id Software, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */

#include <SDL3/SDL.h>

#include "../ref_gl/r_local.h"
#include "sdl_glw.h"

glwstate_t glw_state;
cvar_t *vid_fullscreen;

static int GLimp_InitGL( int stencilbits );

void GLimp_SetWindowIcon( void )
{
	int *xpm_icon = glw_state.applicationIcon;
	if( !xpm_icon )
	{
		return;
	}

	SDL_Surface *surface = SDL_CreateSurfaceFrom( xpm_icon[0], xpm_icon[1], SDL_PIXELFORMAT_ARGB8888, xpm_icon, xpm_icon[0]*4);
	if ( !surface ) {
		ri.Com_Printf( "GLimp_SetWindowIcon() - failed to create surface: %s\n", SDL_GetError() );
		return;
	}

	bool set = SDL_SetWindowIcon( glw_state.sdl_window, surface );
	SDL_DestroySurface( surface );

	if ( !set ) {
		ri.Com_Printf( "GLimp_SetWindowIcon() - failed to set icon: %s\n", SDL_GetError() );
		return;
	}
}

rserr_t GLimp_SetFullscreenMode( bool fullscreen )
{
    if( SDL_SetWindowFullscreen( glw_state.sdl_window, fullscreen ) ) {
	    SDL_SyncWindow( glw_state.sdl_window );
        glConfig.fullScreen = fullscreen;
        return rserr_ok;
    }

    return rserr_invalid_fullscreen;
}

static void GLimp_CreateWindow( int x, int y, int width, int height )
{
	// ugly, but SDL2->SDL3 migration guide says so...
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty( props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, glw_state.applicationName );
	SDL_SetNumberProperty( props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width );
	SDL_SetNumberProperty( props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height );
	SDL_SetNumberProperty( props, SDL_PROP_WINDOW_CREATE_X_NUMBER, x );
	SDL_SetNumberProperty( props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y );
	SDL_SetBooleanProperty( props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true );

	glw_state.sdl_window = SDL_CreateWindowWithProperties( props );

	SDL_DestroyProperties(props);

	if( !glw_state.sdl_window )
		Sys_Error( "Couldn't create window: \"%s\"", SDL_GetError() );

	if( glw_state.wndproc ) {
		glw_state.wndproc( glw_state.sdl_window, 0, 0, 0 );
	}

	GLimp_SetWindowIcon();
}

/**
 * Set video mode.
 * @param mode number of the mode to set
 * @param fullscreen <code>true</code> for a fullscreen mode,
 *     <code>false</code> otherwise
 */
rserr_t GLimp_SetMode( int x, int y, int width, int height, bool fullscreen )
{
	const char *win_fs[] = {"W", "FS"};

	ri.Com_Printf( "Initializing OpenGL display\n" );
	ri.Com_Printf( "...setting mode:" );
	ri.Com_Printf( " %d %d %s\n", width, height, win_fs[fullscreen] );

	// destroy the existing window
	if( glw_state.sdl_window ) {
		SDL_DestroyWindow( glw_state.sdl_window );
	}

	GLimp_CreateWindow( x, y, width, height );

	// init all the gl stuff for the window
	if( !GLimp_InitGL( r_stencilbits->integer ) ) {
		ri.Com_Printf( "GLimp_SetMode() - GLimp_InitGL failed\n" );
		return rserr_invalid_mode;
	}

    glConfig.fullScreen = fullscreen ? GLimp_SetFullscreenMode( fullscreen ) == rserr_ok : false;
	glConfig.width = width;
	glConfig.height = height;

    return glConfig.fullScreen == fullscreen ? rserr_ok : rserr_invalid_fullscreen;
}

/**
 * Shutdown GLimp sub system.
 */
void GLimp_Shutdown()
{
	SDL_DestroyWindow( glw_state.sdl_window );

	free( glw_state.applicationName );
	free( glw_state.applicationIcon );

	memset( &glw_state, 0, sizeof( glw_state ) );

	glConfig.width = 0;
	glConfig.height = 0;
}

/**
 * Initialize GLimp sub system.
 * @param hinstance
 * @param wndproc
 */

int GLimp_Init( const char *applicationName, void *wndproc, const int *iconXPM )
{
	glw_state.wndproc = wndproc;
	glw_state.applicationName = strdup( applicationName );
	glw_state.applicationIcon = NULL;
	memcpy( glw_state.applicationName, applicationName, strlen( applicationName ) + 1 );

	if( iconXPM )
	{
		size_t icon_memsize = iconXPM[0] * iconXPM[1] * sizeof( int );
		glw_state.applicationIcon = malloc( icon_memsize );
		memcpy( glw_state.applicationIcon, iconXPM, icon_memsize );
	}

	return true;
}

static int GLimp_InitGL( int stencilbits )
{
	int colorBits, depthBits, stencilBits;

	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, max( 0, stencilbits ) );

	glw_state.sdl_glcontext = SDL_GL_CreateContext( glw_state.sdl_window );
	if( !glw_state.sdl_glcontext ) {
		ri.Com_Printf( "GLimp_Init() - SDL_GL_CreateContext failed: \"%s\"\n", SDL_GetError() );
		return false;
	}

	if( !SDL_GL_MakeCurrent( glw_state.sdl_window, glw_state.sdl_glcontext ) ) {
		ri.Com_Printf( "GLimp_Init() - SDL_GL_MakeCurrent failed: \"%s\"\n", SDL_GetError() );
		return false;
	}

	/*
	 ** print out PFD specifics
	 */
	SDL_GL_GetAttribute( SDL_GL_BUFFER_SIZE, &colorBits );
	SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &depthBits );
	SDL_GL_GetAttribute( SDL_GL_STENCIL_SIZE, &stencilBits );

	glConfig.stencilBits = stencilBits;

	ri.Com_Printf( "GL PFD: color(%d-bits) Z(%d-bit) stencil(%d-bits)\n", colorBits, depthBits, stencilBits );

	return true;
}

/**
 * TODO documentation
 */
void GLimp_BeginFrame( void )
{
}

/**
 * Swap the buffers and possibly do other stuff that yet needs to be
 * determined.
 */
void GLimp_EndFrame( void )
{
	SDL_GL_SwapWindow( glw_state.sdl_window );
}

// XXX: SDL3 doesn't support window gamma
bool GLimp_GetGammaRamp( size_t stride, unsigned short *psize, unsigned short *ramp )
{
	return false;
}
void GLimp_SetGammaRamp( size_t stride, unsigned short size, unsigned short *ramp )
{
}

/**
 * TODO documentation
 */
void GLimp_AppActivate( bool active, bool destroy )
{
}

/*
** GLimp_SetWindow
*/
rserr_t GLimp_SetWindow( void *wndproc, bool *surfaceChangePending )
{
	if( surfaceChangePending )
		*surfaceChangePending = false;

	return rserr_ok; // surface cannot be lost
}

/*
** GLimp_RenderingEnabled
*/
bool GLimp_RenderingEnabled( void )
{
	return true;
}

/*
** GLimp_SetSwapInterval
*/
void GLimp_SetSwapInterval( int swapInterval )
{
	SDL_GL_SetSwapInterval( swapInterval );
}

/*
** GLimp_MakeCurrent
*/
bool GLimp_MakeCurrent( void *context, void *surface )
{
	return SDL_GL_MakeCurrent( glw_state.sdl_window, (SDL_GLContext)context ) == 0;
}

/*
** GLimp_EnableMultithreadedRendering
*/
void GLimp_EnableMultithreadedRendering( bool enable )
{
}

/*
** GLimp_GetWindowSurface
*/
void *GLimp_GetWindowSurface( bool *renderable )
{
	if( renderable )
		*renderable = true;
	return NULL;
}

/*
** GLimp_UpdatePendingWindowSurface
*/
void GLimp_UpdatePendingWindowSurface( void )
{
}

/*
** GLimp_SharedContext_Create
*/
bool GLimp_SharedContext_Create( void **context, void **surface )
{
	SDL_GL_SetAttribute( SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );

	*context = (void*)SDL_GL_CreateContext( glw_state.sdl_window );
	if( surface )
		*surface = NULL;

	// SDL_GL_CreateContext makes the newly created context current
	// we don't want that, so revert to our main context
	return SDL_GL_MakeCurrent( glw_state.sdl_window, glw_state.sdl_glcontext ) == 0;
}

/*
** GLimp_SharedContext_Destroy
*/
void GLimp_SharedContext_Destroy( void *context, void *surface )
{
	SDL_GL_DestroyContext( context );
}
