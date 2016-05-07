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

/*
	//TODO
		move from script.c to here
*/

#include "script.h"

/*
=============
FILE FUNCTIONS
=============
*/

void GScr_fopen( int entityIndex ) {
    if ( !paramCheck( 2, VT_STRING, VT_STRING ) ) {
        Scr_AddInt( -1 );
        return;
    }

    char* name = Scr_GetString( 0 );
    char* mode = Scr_GetString( 1 );

    FILE* f = fopen( name, mode );
    if ( f == NULL ) {
        Scr_AddInt( -1 );
        _gscr_errno = errno;

        return;
    }
    
    Scr_AddInt( (int)( f ) );
}

void GScr_fexists( int entityIndex ) {
    if ( !paramCheck( 1, VT_STRING ) ) {
        Scr_AddBool( false );
        return;
    }

    char* name = Scr_GetString( 0 );

    FILE* f;
    if ( ( f = fopen( name, "r" ) ) ) {
        fclose( f );

        Scr_AddBool( true );
    } else {
        Scr_AddBool( false );
    }
}

//
// reads entire file into string
//
void GScr_freadfile( int entityIndex ) {
    if ( !paramCheck( 1, VT_INT ) ) {
        Scr_AddUndefined();
        return;
    }

    char *buf = NULL;
    size_t size = 0;
   
    FILE *f = (FILE *)( Scr_GetInt( 0 ) );
    if ( f == NULL ) {
        Scr_AddUndefined();
        return;
    }

    fseek( f, 0, SEEK_END );
    size = ftell( f );
    rewind( f );
    buf = (char *) malloc( size );
    fread( buf, size, 1, f );
    Scr_AddString( buf );
    free( buf );
}

//
// reads a line up to \n
//
void GScr_freadline( int entityIndex ) {
    if ( !paramCheck( 1, VT_INT ) ) {
        Scr_AddUndefined();
        return;
    }

    char *line = NULL;
    ssize_t read;
    size_t len;

    FILE *f = (FILE *)( Scr_GetInt( 0 ) );
    if ( f == NULL ) {
        Scr_AddUndefined();
        return;
    }

    if ( ( read = getline( &line, len, f ) ) == -1 ) {
        _gscr_errno = errno;
        Scr_AddUndefined();
        return;
    }

    Scr_AddString( line );
    free( line );
}

//
// reads only certain bits
// continues where last left off
//
void GScr_fread( int entityIndex ) {
    if ( !paramCheck( 2, VT_INT, VT_INT ) ) {
        Scr_AddUndefined();
        return;
    }

    char *buf = NULL;
    size_t size = 0;
    int len = Scr_GetInt( 0 );
   
    FILE *f = (FILE *)( Scr_GetInt( 1 ) );
    if ( f == NULL ) {
        Scr_AddUndefined();
        return;
    }

    int cur = ftell( f );
    fseek( f, 0, SEEK_END );
    size = ftell( f ) - cur;

    if ( len > size )
        len = size;

    rewind( f );

    fseek( f, cur, SEEK_SET );
    buf = (char *) malloc( len );
    fread( buf, len, 1, f );
    Scr_AddString( buf );
    free( buf ); 
}

void GScr_fwrite( int entityIndex ) {
    if ( !paramCheck( 2, VT_STRING, VT_INT ) ) {
        Scr_AddBool( false );
        return;
    }

    char* text = Scr_GetString( 0 );
    FILE *f = (FILE *)( Scr_GetInt( 1 ) );

    if ( f == NULL ) {
        Scr_AddBool( false );
    } else {
        fprintf( f, text );

        Scr_AddBool( true );
    }
}

void GScr_fsize( int entityIndex ) {
    if ( !paramCheck( 1, VT_STRING ) ) {
        Scr_AddInt( -1 );
        return;
    }

    char* name = Scr_GetString( 0 );
    size_t len;
    FILE *f = fopen( name, "r" );
    if ( f == NULL ) {
        _gscr_errno = errno;
        Scr_AddInt( -1 );
        return;
    }

    fseek( f, 0, SEEK_END );
    len = ftell( f );
    rewind( f );
    Scr_AddInt( (int)len );
}

void GScr_fclose( int entityIndex ) {
    if ( !paramCheck( 1, VT_INT ) ) {
        Scr_AddBool( false );
        return;
    }

    FILE *f = (FILE*)( Scr_GetInt( 0 ) );
    if ( f ) {
        fclose( f );

        Scr_AddBool( true );
    } else {
        Scr_AddBool( false );
    }
}
