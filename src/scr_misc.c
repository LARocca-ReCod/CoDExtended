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

void GScr_errno( int entityIndex ) {
    Scr_AddInt( _gscr_errno );
}

void GScr_errnostr( int entityIndex ) {
    if ( !paramCheck( 1, VT_INT ) ) {
        Scr_AddString( "" );
        return;
    }

    int err = Scr_GetInt( 0 );
    char *strerr = xstrdup( strerror( err ) );
    Scr_AddString( strerr );
    free( strerr );
}

void GScr_typeof( int entityIndex ) {
    int type = Scr_GetType( 0 );
    Scr_AddString( Scr_GetVariableType( type ) );
}

char* Scr_GetVariableType( int type ) {
    switch(type) {
        default:
        case VT_UNDEFINED: {
            return "undefined";
        }
        break;
        case VT_INT: {
            return "integer";
        } 
        break;
        case VT_FLOAT: {
            return "float";
        } 
        break;
        case VT_STRING: {
            return "string";
        } 
        break;
        case VT_VECTOR: {
            return "vector";
        } 
        break;
        case VT_LOCALIZED_STRING: {
            return "localized-string";
        } 
        break;
        case VT_OBJECT: {
            return "object";
        } 
        break;
        case VT_ARRAY: {
            return "array";
        } 
        break;
    }
}

void GScr_printconsole(int entityIndex) {
    const char* txt = Scr_GetString(0);
    printf(txt);
}

void GScr_salt_password(int a1) {
    char* password = Scr_GetString(0);
    char* salt = Scr_GetString(1);
    Scr_AddString(get_pass_hash(password, salt));
}

void GScr_SendServerCommand(int a1) {
    char* cmd = Scr_GetString(0);
    SV_SendServerCommand(NULL, 1, cmd);
}

void GScr_Trace(int a1) {
    trace_t tr;
    vec3_t start, end, mins, maxs;
    Scr_GetVector(0, start);
    Scr_GetVector(1, mins);
    Scr_GetVector(2, maxs);
    Scr_GetVector(3, end);
    int ignore = Scr_GetInt(4);
    int mask = Scr_GetInt(5);
    int locational = 0;
    int staticmodels = 0;
    if(Scr_GetNumParam() > 6)
        locational = Scr_GetInt(6);
    if(Scr_GetNumParam() > 7)
        staticmodels = Scr_GetInt(7);
    
    void (*trace)(void*,float*,float*,float*,float*,int ignore,int contentmask,int locational,char *priorityMap,int staticmodels);
    *(int*)&trace = 0x80916F4;
    
    trace(&tr,start,mins,maxs,end,-1,mask,locational,NULL,staticmodels);
    Scr_MakeArray();
    
    Scr_AddVector(tr.endpos); Scr_AddArrayStringIndexed(scr_const->position);
    if((tr.entityNum - 1022) > 1)
    Scr_AddInt(tr.entityNum);//Scr_AddEntity(&g_entities[tr.entityNum]); //scr_addentity crashed sometime cba
    else
    Scr_AddUndefined();
    Scr_AddArrayStringIndexed(scr_const->entity);
    
    Scr_AddFloat(tr.fraction); Scr_AddArrayStringIndexed(scr_const->fraction);
    Scr_AddVector(tr.normal); Scr_AddArrayStringIndexed(scr_const->normal);
    Scr_AddInt(tr.contents); Scr_AddArrayStringIndexed(xscr_const.contents);
    Scr_AddInt(tr.surfaceFlags); Scr_AddArrayStringIndexed(scr_const->surfacetype);
    Scr_AddInt(tr.textureName); Scr_AddArrayStringIndexed(xscr_const.texturename);
}

void GScr_Cmd_Argc(int a1) {
    Scr_AddInt(Cmd_Argc());
}

void GScr_Cmd_Argv(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    Scr_AddString(Cmd_Argv(idx));
}

void GScr_trap_Argv(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    char buf[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(idx, buf, sizeof(buf));
    Scr_AddString(buf);
}

void GScr_ConcatArgs(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    Scr_AddString(ConcatArgs(idx));
}

void GScr_md5(int a1) {
    char* str = Scr_GetString(0);
    Scr_AddString(get_md5(str));
}

void GScr_getChat(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    char* chat = ConcatArgs(idx);
    if(strlen(chat) == 0 || (chat[0] == 0x15 && strlen(chat)==1)) {
        Scr_AddString("");
        return;
    }
    if(chat[0] == 0x15)
        Scr_AddString(&chat[1]);
    else
        Scr_AddString(chat);
}

bool scr_return = 0;
void GScr_return(int a1) {
    scr_return = 1;
}

bool Scr_Continue() {
    bool ret = scr_return;
    scr_return = 0;
    return(!ret);
}

void GScr_getDateStamp( int entityIndex ) {
    time_t t;
    struct tm *tm_info;
    time( &t );
    tm_info = localtime( &t );

    if ( paramCheck( 1, VT_STRING ) ) {
        char buff[ 128 ];
        strftime( buff, 128, Scr_GetString( 0 ), tm_info );
        Scr_AddString( buff );
        return;
    }

    char buff[ 32 ];
    strftime( buff, 32, "%A, %d %B", tm_info );
    Scr_AddString( buff );
}

void GScr_frametime( int entityIndex ) {
    cvar_t *fps = Cvar_Get( "sv_fps", 20, 256 );
    float framelength = ( 1.0f / (float)fps->integer );

    Scr_AddFloat( framelength );
}

void GScr_isDefinedAny( int entityIndex ) {
    bool defined = false;

    int i = 0;
    for ( ; i < Scr_GetNumParam(); i++ ) {
        if ( Scr_GetType( i ) != VT_UNDEFINED ) {
            defined = true;
            break;
        }
    }

    Scr_AddBool( defined );
}

void GScr_isDefinedAll( int entityIndex ) {
    bool defined = true;

    int i = 0;
    for ( ; i < Scr_GetNumParam(); i++ ) {
        if ( Scr_GetType( i ) == VT_UNDEFINED ) {
            defined = false;
            break;
        }
    }

    Scr_AddBool( defined );
}
