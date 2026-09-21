/*
Copyright (C) 2011 Victor Luchits

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

#include "ui_precompiled.h"
#include "kernel/ui_common.h"
#include "kernel/ui_main.h"

#include "as/asui.h"
#include "as/asui_local.h"

typedef enum
{
	MM_LOGIN_STATE_LOGGED_OUT,
	MM_LOGIN_STATE_IN_PROGRESS,
	MM_LOGIN_STATE_LOGGED_IN
} mmstate_t;

namespace ASUI {

// dummy funcdef
static void ASMatchMaker_EventListenerCallback( Event *event )
{
}

class ASMatchMaker
{
public:
	ASMatchMaker() {}
	~ASMatchMaker() { }

	bool login( const asstring_t &user, const asstring_t &password )
	{
		return false;
	}

	bool logout( void )
	{
		return false;
	}

	int getState( void ) const
	{
		return MM_LOGIN_STATE_LOGGED_OUT;
	}

	asstring_t *getUser( void ) const
	{
		return ASSTR( "" );
	}

	asstring_t *getProfileURL( bool rml ) const
	{
		return ASSTR( "" );
	}

	asstring_t *getBaseWebURL() const
	{
		return ASSTR( "" );
	}

	asstring_t *getLastError( void ) const
	{
		return ASSTR( "" );
	}

	void addEventListener( const asstring_t &event, asIScriptFunction *func ) {
	}

	void removeEventListener( const asstring_t &event, asIScriptFunction *func ) {
	}
};

}
ASBIND_TYPE( ASUI::ASMatchMaker, Matchmaker );

namespace ASUI {

// ====================================================================

static ASMatchMaker *asMM;

/// This makes AS aware of this class so other classes may reference
/// it in their properties and methods
void PrebindMatchMaker( ASInterface *as )
{
	ASBind::Class<ASMatchMaker, ASBind::class_singleref>( as->getEngine() );
}

void BindMatchMaker( ASInterface *as )
{
	ASBind::Global( as->getEngine() )
		// setTimeout and setInterval callback funcdefs
		.funcdef( &ASMatchMaker_EventListenerCallback, "MMEventListenerCallback" )
	;

	ASBind::Enum( as->getEngine(), "eMatchmakerState" )
		( "MM_LOGIN_STATE_LOGGED_OUT", MM_LOGIN_STATE_LOGGED_OUT )
		( "MM_LOGIN_STATE_IN_PROGRESS", MM_LOGIN_STATE_IN_PROGRESS )
		( "MM_LOGIN_STATE_LOGGED_IN", MM_LOGIN_STATE_LOGGED_IN )
	;

	ASBind::GetClass<ASMatchMaker>( as->getEngine() )
		.method( &ASMatchMaker::login, "login" )
		.method( &ASMatchMaker::logout, "logout" )
		.method( &ASMatchMaker::getState, "get_state" )
		.method( &ASMatchMaker::getLastError, "get_lastError" )
		.method( &ASMatchMaker::getUser, "get_user" )
		.method( &ASMatchMaker::getProfileURL, "profileURL" )
		.method( &ASMatchMaker::getBaseWebURL, "baseWebURL" )

		.method2( &ASMatchMaker::addEventListener, "void addEventListener( const String &event, MMEventListenerCallback @callback )" )
		.method2( &ASMatchMaker::removeEventListener, "void removeEventListener( const String &event, MMEventListenerCallback @callback )" )
	;
}

void BindMatchMakerGlobal( ASInterface *as )
{
	assert( asMM == NULL );

	// set the AS module for scheduler
	asMM = __new__( ASMatchMaker )();

	ASBind::Global( as->getEngine() )
		// global variable
		.var( asMM, "matchmaker" )
	;
}

void UnbindMatchMaker( void )
{
	__delete__( asMM );
	asMM = NULL;
}

}
