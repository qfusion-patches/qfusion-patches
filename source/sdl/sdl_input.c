#include <SDL3/SDL.h>
#include "../client/client.h"

cvar_t *in_grabinconsole;

extern cvar_t *vid_xpos;
extern cvar_t *vid_ypos;

extern SDL_Window *sdl_window;

static bool input_inited = false;
static bool mouse_active = false;
static bool input_focus = false;
static bool mouse_relative = false;

static float mx = 0, my = 0;

void IN_Commands( void )
{
}

/**
 * Function which is called whenever the mouse is moved.
 * @param ev the SDL event object containing the mouse position et all
 */
static void mouse_motion_event( SDL_MouseMotionEvent *event )
{
	if( !mouse_active ) {
		return;
	}

	mx += event->xrel;
	my += event->yrel;
}

/**
 * Function which is called whenever a mouse button is pressed or released.
 * @param ev the SDL event object containing the button number et all
 * @param state either true if it is a keydown event or false otherwise
 */
static void mouse_button_event( SDL_MouseButtonEvent *event, bool state )
{
	Uint8 button = event->button;

	if( !mouse_active ) {
		return;
	}

	if( button <= 3 ) {
		switch( button ) {
			case SDL_BUTTON_LEFT:
				Key_MouseEvent( K_MOUSE1, state, Sys_Milliseconds() );
				break;
			case SDL_BUTTON_MIDDLE:
				Key_MouseEvent( K_MOUSE3, state, Sys_Milliseconds() );
				break;
			case SDL_BUTTON_RIGHT:
				Key_MouseEvent( K_MOUSE2, state, Sys_Milliseconds() );
				break;
		}
	} else if( button <= 8 ) {
		// The engine only supports up to 8 buttons plus the mousewheel.

		// Switch place of MOUSE4-5 with MOUSE6-7
		if( button == 4 || button == 5 )
			button += 2;
		else if( button == 6 || button == 7 )
			button -= 2;

		Key_MouseEvent( K_MOUSE1 + button - 1, state, Sys_Milliseconds() );
	} else
		Com_Printf( "sdl_input.c: Unsupported mouse button (button = %u)\n", button );
}

static void mouse_wheel_event( SDL_MouseWheelEvent *event )
{
	if( !mouse_active ) {
		return;
	}

	int key = event->y > 0 ? K_MWHEELUP : K_MWHEELDOWN;
	unsigned sys_msg_time = Sys_Milliseconds();

	Key_Event( key, true, sys_msg_time );
	Key_Event( key, false, sys_msg_time );
}

static unsigned TranslateSDLKeycode( const SDL_KeyboardEvent *event )
{
	switch( event->key ) {
		case SDLK_TAB:          return K_TAB;        break;
		case SDLK_RETURN:       return K_ENTER;      break;
		case SDLK_ESCAPE:       return K_ESCAPE;     break;
		case SDLK_SPACE:        return K_SPACE;      break;
		case SDLK_CAPSLOCK:     return K_CAPSLOCK;   break;
		case SDLK_SCROLLLOCK:   return K_SCROLLLOCK; break;
		case SDLK_NUMLOCKCLEAR: return K_NUMLOCK;    break;
		case SDLK_LALT:         return K_LALT;       break;
		case SDLK_RALT:         return K_RALT;       break;
		case SDLK_LCTRL:        return K_LCTRL;      break;
		case SDLK_RCTRL:        return K_RCTRL;      break;
		case SDLK_LSHIFT:       return K_LSHIFT;     break;
		case SDLK_RSHIFT:       return K_LSHIFT;     break;
		case SDLK_BACKSPACE:    return K_BACKSPACE;  break;
		case SDLK_UP:           return K_UPARROW;    break;
		case SDLK_DOWN:         return K_DOWNARROW;  break;
		case SDLK_LEFT:         return K_LEFTARROW;  break;
		case SDLK_RIGHT:        return K_RIGHTARROW; break;

		default: break;
	}

	switch( event->scancode )
	{
		case SDL_SCANCODE_F1:           return K_F1;      break;
		case SDL_SCANCODE_F2:           return K_F2;      break;
		case SDL_SCANCODE_F3:           return K_F3;      break;
		case SDL_SCANCODE_F4:           return K_F4;      break;
		case SDL_SCANCODE_F5:           return K_F5;      break;
		case SDL_SCANCODE_F6:           return K_F6;      break;
		case SDL_SCANCODE_F7:           return K_F7;      break;
		case SDL_SCANCODE_F8:           return K_F8;      break;
		case SDL_SCANCODE_F9:           return K_F9;      break;
		case SDL_SCANCODE_F10:          return K_F10;     break;
		case SDL_SCANCODE_F11:          return K_F11;     break;
		case SDL_SCANCODE_F12:          return K_F12;     break;
		case SDL_SCANCODE_F13:          return K_F13;     break;
		case SDL_SCANCODE_F14:          return K_F14;     break;
		case SDL_SCANCODE_F15:          return K_F15;     break;
		case SDL_SCANCODE_INSERT:       return K_INS;     break;
		case SDL_SCANCODE_DELETE:       return K_DEL;     break;
		case SDL_SCANCODE_PAGEUP:       return K_PGUP;    break;
		case SDL_SCANCODE_PAGEDOWN:     return K_PGDN;    break;
		case SDL_SCANCODE_HOME:         return K_HOME;    break;
		case SDL_SCANCODE_END:          return K_END;     break;
		case SDL_SCANCODE_GRAVE:        return '~';       break;
		case SDL_SCANCODE_LGUI:
		case SDL_SCANCODE_RGUI:         return K_COMMAND; break;

		case SDL_SCANCODE_A:         return 'a';         break;
		case SDL_SCANCODE_B:         return 'b';         break;
		case SDL_SCANCODE_C:         return 'c';         break;
		case SDL_SCANCODE_D:         return 'd';         break;
		case SDL_SCANCODE_E:         return 'e';         break;
		case SDL_SCANCODE_F:         return 'f';         break;
		case SDL_SCANCODE_G:         return 'g';         break;
		case SDL_SCANCODE_H:         return 'h';         break;
		case SDL_SCANCODE_I:         return 'i';         break;
		case SDL_SCANCODE_J:         return 'j';         break;
		case SDL_SCANCODE_K:         return 'k';         break;
		case SDL_SCANCODE_L:         return 'l';         break;
		case SDL_SCANCODE_M:         return 'm';         break;
		case SDL_SCANCODE_N:         return 'n';         break;
		case SDL_SCANCODE_O:         return 'o';         break;
		case SDL_SCANCODE_P:         return 'p';         break;
		case SDL_SCANCODE_Q:         return 'q';         break;
		case SDL_SCANCODE_R:         return 'r';         break;
		case SDL_SCANCODE_S:         return 's';         break;
		case SDL_SCANCODE_T:         return 't';         break;
		case SDL_SCANCODE_U:         return 'u';         break;
		case SDL_SCANCODE_V:         return 'v';         break;
		case SDL_SCANCODE_W:         return 'w';         break;
		case SDL_SCANCODE_X:         return 'x';         break;
		case SDL_SCANCODE_Y:         return 'y';         break;
		case SDL_SCANCODE_Z:         return 'z';         break;

		case SDL_SCANCODE_1:         return '1';         break;
		case SDL_SCANCODE_2:         return '2';         break;
		case SDL_SCANCODE_3:         return '3';         break;
		case SDL_SCANCODE_4:         return '4';         break;
		case SDL_SCANCODE_5:         return '5';         break;
		case SDL_SCANCODE_6:         return '6';         break;
		case SDL_SCANCODE_7:         return '7';         break;
		case SDL_SCANCODE_8:         return '8';         break;
		case SDL_SCANCODE_9:         return '9';         break;
		case SDL_SCANCODE_0:         return '0';         break;

		case SDL_SCANCODE_MINUS:        return '-';  break;
		case SDL_SCANCODE_EQUALS:       return '=';  break;
		case SDL_SCANCODE_BACKSLASH:    return '\\'; break;
		case SDL_SCANCODE_COMMA:        return ',';  break;
		case SDL_SCANCODE_PERIOD:       return '.';  break;
		case SDL_SCANCODE_SLASH:        return '/';  break;
		case SDL_SCANCODE_LEFTBRACKET:  return '[';  break;
		case SDL_SCANCODE_RIGHTBRACKET: return ']';  break;
		case SDL_SCANCODE_SEMICOLON:    return ';';  break;
		case SDL_SCANCODE_APOSTROPHE:   return '\''; break;

		case SDL_SCANCODE_KP_0:        return KP_INS;        break;
		case SDL_SCANCODE_KP_1:        return KP_END;        break;
		case SDL_SCANCODE_KP_2:        return KP_DOWNARROW;  break;
		case SDL_SCANCODE_KP_3:        return KP_PGDN;       break;
		case SDL_SCANCODE_KP_4:        return KP_LEFTARROW;  break;
		case SDL_SCANCODE_KP_5:        return KP_5;          break;
		case SDL_SCANCODE_KP_6:        return KP_RIGHTARROW; break;
		case SDL_SCANCODE_KP_7:        return KP_HOME;       break;
		case SDL_SCANCODE_KP_8:        return KP_UPARROW;    break;
		case SDL_SCANCODE_KP_9:        return KP_PGUP;       break;
		case SDL_SCANCODE_KP_ENTER:    return KP_ENTER;      break;
		case SDL_SCANCODE_KP_PERIOD:   return KP_DEL;        break;
		case SDL_SCANCODE_KP_PLUS:     return KP_PLUS;       break;
		case SDL_SCANCODE_KP_MINUS:    return KP_MINUS;      break;
		case SDL_SCANCODE_KP_DIVIDE:   return KP_SLASH;      break;
		case SDL_SCANCODE_KP_MULTIPLY: return KP_STAR;       break;
		case SDL_SCANCODE_KP_EQUALS:   return KP_EQUAL;      break;

		default: break;
	}

	return 0;
}

// XXX: This doesn't comprehensively filter out all non-printables, but the
// only source of events is the keyboard, so it's fine for it to not be
// comprehensive.
//
// This is a hack as ideally we would use SDL's text input system, but the
// engine provides no way for us to get notified about entering a "typing
// state." It should be extended to to do this.
static void text_input( unsigned key, const SDL_KeyboardEvent *event ) {
	SDL_Keymod mod = event->mod;

	if( mod & SDL_KMOD_CTRL || mod & SDL_KMOD_GUI )
		return;

	if( mod & SDL_KMOD_ALT && !(mod & SDL_KMOD_MODE) )
		return;

	SDL_Keycode keycode = SDL_GetKeyFromScancode( event->scancode, event->mod, false );

	if( keycode & ( SDLK_EXTENDED_MASK | SDLK_SCANCODE_MASK ) )
		return;

	if( keycode == 0 )
		return;

	// C0 control codes
	if( keycode < 0x20 || keycode == 0x7F )
		return;

	// C1 control codes
	if( keycode >= 0x80 && keycode <= 0x9F )
		return;

	// make sure it's UCS-2 without surrogates
	if( keycode > 0xFFFF || ( keycode >= 0xD800 && keycode <= 0xDFFF ) )
		return;

	Key_CharEvent( key, (wchar_t)keycode );
}

/**
 * Function which is called whenever a key is pressed or released.
 * @param event the SDL event object containing the keysym et all
 * @param state either true if it is a keydown event or false otherwise
 */
static void key_event( const SDL_KeyboardEvent *event, bool down )
{
	unsigned key = TranslateSDLKeycode( event );

	if( key > 255 )
		key = 0;

	Key_Event( key, down, Sys_Milliseconds() );

	if( down )
		text_input( key, event );
}

/*****************************************************************************/

static void AppActivate( bool active )
{
	SCR_PauseCinematic( !active );
	CL_SoundModule_Activate( active );
	VID_AppActivate( active, false );
}

static void IN_HandleEvents( void )
{
	SDL_Event event;
	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
			case SDL_EVENT_KEY_DOWN:
				if( event.key.scancode == SDL_SCANCODE_C && event.key.mod & SDL_KMOD_CTRL )
				{
					Key_CharEvent( KC_CTRLC, KC_CTRLC );
					break;
				}

				if( event.key.scancode == SDL_SCANCODE_V && event.key.mod & SDL_KMOD_CTRL )
				{
					Key_CharEvent( KC_CTRLV, KC_CTRLV );
					break;
				}

				key_event( &event.key, true );
				break;

			case SDL_EVENT_KEY_UP:
				key_event( &event.key, false );
				break;

			case SDL_EVENT_MOUSE_MOTION:
				mouse_motion_event( &event.motion );
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				mouse_button_event( &event.button, true );
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				mouse_button_event( &event.button, false );
				break;

			case SDL_EVENT_MOUSE_WHEEL:
				mouse_wheel_event( &event.wheel );
				break;

			case SDL_EVENT_QUIT:
				Cbuf_ExecuteText( EXEC_NOW, "quit" );
				break;

			case SDL_EVENT_WINDOW_SHOWN:
				AppActivate( true );
				break;
			case SDL_EVENT_WINDOW_HIDDEN:
				AppActivate( false );
				break;
			case SDL_EVENT_WINDOW_FOCUS_GAINED:
				input_focus = true;
				break;
			case SDL_EVENT_WINDOW_FOCUS_LOST:
				input_focus = false;
				break;
			case SDL_EVENT_WINDOW_MOVED:
				// TODO: move this somewhere else
				Cvar_SetValue( "vid_xpos", event.window.data1 );
				Cvar_SetValue( "vid_ypos", event.window.data2 );
				vid_xpos->modified = false;
				vid_ypos->modified = false;
				break;
		}
	}
}

/**
 * Skips relative mouse movement for current frame.
 * We need to ignore the movement event generated when
 * mouse cursor is warped to window centre for the first time.
 */
static void IN_SkipRelativeMouseMove( void )
{
	if( mouse_relative ) {
		SDL_GetRelativeMouseState( NULL, NULL );
	}
}

static void IN_WarpMouseToCenter( int *pcenter_x, int *pcenter_y )
{
	int center_x, center_y;

	SDL_GetWindowSize( sdl_window, &center_x, &center_y );

	center_x /= 2;
	center_y /= 2;

	SDL_WarpMouseInWindow( sdl_window, center_x, center_y );

	if( pcenter_x ) {
		*pcenter_x = center_x;
	}
	if( pcenter_y ) {
		*pcenter_y = center_y;
	}
}

void IN_MouseMove( usercmd_t *cmd )
{
	if ( !mx && !my )
		return;

	if( mouse_active ) {
		CL_MouseMove( cmd, mx, my );
	}

	mx = my = 0;
}

/*
* IN_GetThumbsticks
*/
void IN_GetThumbsticks( vec4_t sticks )
{
	Vector4Set( sticks, 0.0f, 0.0f, 0.0f, 0.0f );
}

/* #if SDL_VERSION_ATLEAST(2, 0, 2) */
/**/
/* 	{ */
/* 		cvar_t *m_raw = Cvar_Get( "m_raw", "1", CVAR_ARCHIVE ); */
/* 		SDL_SetHint( SDL_HINT_MOUSE_RELATIVE_MODE_WARP, m_raw->integer ? "0" : "1" ); */
/* 	} */
/* #endif */

void IN_Init()
{
	if( input_inited )
		return;

	input_inited = true;

	in_grabinconsole = Cvar_Get( "in_grabinconsole", "0", CVAR_ARCHIVE );

	mouse_relative = SDL_SetWindowRelativeMouseMode( sdl_window, true );

	mouse_active = true;
}

/**
 * Shutdown input subsystem.
 */
void IN_Shutdown()
{
	if( !input_inited )
		return;

	input_inited = false;
	SDL_SetWindowRelativeMouseMode( sdl_window, false );
}

/**
 * Restart the input subsystem.
 */
void IN_Restart( void )
{
	IN_Shutdown();
	IN_Init();
}

/**
 * This function is called for every frame and gives us some time to poll
 * for events that occured at our input devices.
 */
void IN_Frame()
{
	if( !input_inited )
		return;

	if( !input_focus || (!Cvar_Value( "vid_fullscreen" ) && cls.key_dest == key_console && !in_grabinconsole->integer) ) {
		if( mouse_active ) {
			mouse_relative = !SDL_SetWindowRelativeMouseMode( sdl_window, false );
		}
		mouse_active = false;
	} else {
		if( !mouse_active ) {
			mouse_relative = SDL_SetWindowRelativeMouseMode( sdl_window, true );
		}
		mouse_active = true;
	}

	IN_HandleEvents();
}

/**
 * The input devices supported by the system.
 */
unsigned int IN_SupportedDevices( void )
{
	return IN_DEVICE_KEYBOARD | IN_DEVICE_MOUSE;
}

/**
 * Stub for showing an on-screen keyboard.
 */
void IN_ShowSoftKeyboard( bool show )
{
}

/**
 * Stubs for the IME until it's implemented through SDL and/or Cocoa.
 */
void IN_IME_Enable( bool enable )
{
}

size_t IN_IME_GetComposition( char *str, size_t strSize, size_t *cursorPos, size_t *convStart, size_t *convLen )
{
	if( str && strSize )
		str[0] = '\0';
	if( cursorPos )
		*cursorPos = 0;
	if( convStart )
		*convStart = 0;
	if( convLen )
		*convLen = 0;
	return 0;
}

unsigned int IN_IME_GetCandidates( char * const *cands, size_t candSize, unsigned int maxCands, int *selected, int *firstKey )
{
	if( selected )
		*selected = -1;
	if( firstKey )
		*firstKey = 1;
	return 0;
}

/**
 * IN_GetInputLanguage
 */
void IN_GetInputLanguage( char *dest, size_t size )
{
	if( size )
		dest[0] = '\0';
}
