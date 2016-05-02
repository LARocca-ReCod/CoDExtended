/*
	This file is part of CoDExtended.

    CoDExtended is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoDExtended is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "script.h"

/*
=============
MATH
=============
*/

void MScr_randomInt( int entityIndex ) {
    if ( !paramCheck( 1, VT_INT ) ) {
        Scr_AddInt( rand() % RAND_MAX );
        return;
    }

    int max = Scr_GetInt( 0 );
/*
    int limit = RAND_MAX - ( RAND_MAX % max );

    int r;
    while ( ( r = rand() ) >= limit );
*/
    Scr_AddInt( genrand_int32() % max );
}

void MScr_randomIntRange( int entityIndex ) {
    if ( !paramCheck( 2, VT_INT, VT_INT ) ) {
        Scr_AddInt( rand() % RAND_MAX );
        return;
    }

    int min = Scr_GetInt( 0 );
    int max = Scr_GetInt( 1 );

    // e.g
    // min = 5
    // max = 10
    // ( rand() % (10 - 5) ) + 5
    // so 4 + 5 = 9
    // 0 + 5 = 5
    // etc.
    Scr_AddInt( ( genrand_int32() % ( max - min ) ) + min );
}
/*
void MScr_randomFloat( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( ( (float)rand() / (float)RAND_MAX + 1.0f ) );
        return;
    }

    float max = Scr_GetFloat( 0 );
    Scr_AddFloat( ( (float)rand() / (float)RAND_MAX + 1.0f ) * max );
}*/

void MScr_ceil( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 ); 
    Scr_AddFloat( ceil( f ) );
}

void MScr_floor( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 );
    Scr_AddFloat( floor( f ) );
}

void MScr_round( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 );
    Scr_AddFloat( round( f ) );
}

void MScr_sqrt( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 );
    Scr_AddFloat( sqrt( f ) );
}

void MScr_cos( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 );
    Scr_AddFloat( cos( f ) );
}

void MScr_sin( int entityIndex ) {
    if ( !paramCheck( 1, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 );
    Scr_AddFloat( sin( f ) );
}

void MScr_pow( int entityIndex ) {
    if ( !paramCheck( 2, VT_FLOAT, VT_FLOAT ) ) {
        Scr_AddFloat( 0.0 );
        return;
    }

    float f = Scr_GetFloat( 0 );
    float p = Scr_GetFloat( 1 );
    Scr_AddFloat( pow( f, p ) );
}
