// for testing purposes
//#define WP_STANDALONE

#ifdef WP_STANDALONE
    #include <errno.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
    #include <float.h>

    typedef enum { false, true } bool;
    typedef float vec_t;
    typedef vec_t vec3_t[ 3 ];

    #define VectorSubtract( a,b,c )   ( ( c )[0] = ( a )[0] - ( b )[0],( c )[1] = ( a )[1] - ( b )[1],( c )[2] = ( a )[2] - ( b )[2] )

    vec_t _VectorLength( const vec3_t v ) {
        return sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] );
    }

    vec_t _Distance( const vec3_t p1, const vec3_t p2 ) {
        vec3_t v;

        VectorSubtract( p2, p1, v );
        return _VectorLength( v );
    }
#else
    #include "script.h"
    #include <errno.h>
#endif

typedef enum {
    // always first
    WP_HEAD = -1,

    // keep up to date with gsc code
    WP_DEFAULT,
    WP_SPAWN,
    WP_SPAWNEND,
    WP_SPAWNDROP,
    WP_LADDERBASE,
    WP_LADDEREND,
    WP_JUMPSTART,
    WP_JUMPEND,
    WP_FALL,
    WP_CROUCH,
    WP_PRONE,

    // always last
    WP_NONE 
} wp_type;

char *wp_tostr( int type ) {
    switch ( type ) {
        case WP_HEAD:       return "wp_head";

        case WP_DEFAULT:    return "wp_default";
        case WP_SPAWN:      return "wp_spawn";
        case WP_SPAWNEND:   return "wp_spawnend";
        case WP_SPAWNDROP:  return "wp_spawndrop";
        case WP_LADDERBASE: return "wp_ladderbase";
        case WP_LADDEREND:  return "wp_ladderend";
        case WP_JUMPSTART:  return "wp_jumpstart";
        case WP_JUMPEND:    return "wp_jumpend";
        case WP_CROUCH:     return "wp_crouch";
        case WP_FALL:       return "wp_fall";
        case WP_PRONE:      return "wp_prone";

        case WP_NONE:       
        default:            return "none";
    }
}

bool isValidStr( char *str ) {
    int i = 0;
    bool valid = false;

    for ( ; i < WP_NONE; i++ ) {
        if ( !strcmp( wp_tostr( i ), str ) )
            valid = true;
    }

    return valid;
}

typedef struct wp_node_s {
    int id;
    int type;
    vec3_t position;
    struct wp_node_s *next;
    struct wp_node_s *prev;
} wp_node;

wp_node *wpMaster;

// --------------------------------------
bool wp_init( char * );
void wp_freeAll( wp_node ** );
void wp_print( wp_node ** );
wp_node *wp_newNode( int, int, vec3_t );
void wp_insertAtHead( wp_node **, int, int, vec3_t );
void wp_insertAtTail( wp_node **, int, int, vec3_t );
// --------------------------------------

// --------------------------------------
// zomextended
// --------------------------------------
#ifndef WP_STANDALONE
bool wp_initialised = false;

//
// wp_init()
// should be called before any other wp calls
// loads the waypoints file
// returns: true if successful, false otherwise
//
void GScr_wp_init( int entityIndex ) {
    if ( !paramCheck( 1, VT_STRING ) ) {
        Scr_AddBool( false );
        return;
    }

    char *map = Scr_GetString( 0 );
    bool success = wp_init( map );

    if ( success )
        wp_initialised = true;

    Scr_AddBool( success );
}

//
// wp_deinit()
// should be called before exitlevel() in order to
// properly free all the malloc'd bytes
// returns: n/a
//
void GScr_wp_deinit( int entityIndex ) {
    wp_initialised = false;

    wp_freeAll( &wpMaster );
}

//
// wp_getByID( id )
// returns: array with wp info from <id> or undefined
//
void GScr_wp_getByID( int entityIndex ) {
    if ( !wp_initialised ) {
        printf( "code tried to call wp_getclosest() before wp_init()\n" );
        return;
    }

    if ( !paramCheck( 1, VT_INT ) ) {
        Scr_AddUndefined();
        return;
    }

    int id = Scr_GetInt( 0 );

    wp_node *wp = NULL;
    wp_node *head = wpMaster;
    while ( head->next != NULL ) {
        if ( head->id == id ) {
            wp = head;
            break;
        }

        head = head->next;
    }

    if ( wp == NULL ) {
        Scr_AddUndefined();
        return;
    }

    Scr_MakeArray();
    Scr_AddVector( wp->position );          Scr_AddArrayStringIndexed( scr_const->position );
    Scr_AddInt( wp->id );                   Scr_AddArrayStringIndexed( xscr_const.id );
    Scr_AddString( wp_tostr( wp->type ) );  Scr_AddArrayStringIndexed( xscr_const.type );
}

//
// wp_getClosest( location )
// returns: closest waypoint to <location> or undefined
//
void GScr_wp_getClosest( int entityIndex ) {
    if ( !wp_initialised ) {
        printf( "code tried to call wp_getclosest() before wp_init()\n" );
        return;
    }

    if ( !paramCheck( 1, VT_VECTOR ) ) {
        Scr_AddUndefined();
        return;
    }

    vec3_t v;
    Scr_GetVector( 0, v );

    float maxdist = __FLT_MAX__;
    wp_node *closest = NULL;
    wp_node *head = wpMaster;
    while ( head->next != NULL ) {
        if ( _Distance( head->position, v ) < maxdist ) {
            maxdist = _Distance( head->position, v );
            closest = head;
        }

        head = head->next;
    }

    if ( closest == NULL ) {
        Scr_AddUndefined();
        return;
    }

    Scr_AddInt( closest->id );

    Scr_MakeArray();
    Scr_AddVector( closest->position );         Scr_AddArrayStringIndexed( scr_const->position );
    Scr_AddInt( closest->id );                  Scr_AddArrayStringIndexed( xscr_const.id );
    Scr_AddString( wp_tostr( closest->type ) ); Scr_AddArrayStringIndexed( xscr_const.type );
}

//
// wp_getXClosest( location, X )
// returns closest <X> number of waypoints to <location>
//
void GScr_wp_getXClosest( int entityIndex ) {
    if ( !wp_initialised ) {
        printf( "code tried to call wp_getclosest() before wp_init()\n" );
        return;
    }

    if ( !paramCheck( 1, VT_VECTOR ) ) {
        Scr_AddUndefined();
        return;
    }

    int count = 5;
    if ( Scr_GetNumParam() == 2 ) {
        if ( paramCheck( 2, VT_VECTOR, VT_INT ) ) {
            count = Scr_GetInt( 1 );

            if ( count > 20 )
                count = 20;

            if ( count < 1 )
                count = 1;
        }
    }

    vec3_t v;
    Scr_GetVector( 0, v );

    float maxdist = __FLT_MAX__;

    wp_node *nodes = NULL;
    wp_node *nodesHead = NULL;
    wp_node *head = wpMaster;
    while ( head->next != NULL ) {
        if ( _Distance( head->position, v ) < maxdist ) {
            maxdist = _Distance( head->position, v );
            wp_insertAtHead( &nodes, head->id, head->type, head->position );
        }

        head = head->next;
    }

    Scr_MakeArray();

    nodesHead = nodes;
    int depth = 0;
    while ( nodesHead->next != NULL ) {
        if ( depth >= count )
            break;

        Scr_AddInt( nodesHead->id );
        Scr_AddArray();

        nodesHead = nodesHead->next;
        depth++;
    }

    wp_freeAll( &nodes );
}
#endif

// --------------------------------------
// standalone
// --------------------------------------
#ifdef WP_STANDALONE
wp_node *wp_getClosest( wp_node **headRef, vec3_t v ) {
    float maxdist = __FLT_MAX__;
    wp_node *closest = NULL;
    wp_node *head = *headRef;
    while ( head->next != NULL ) {
        if ( _Distance( head->position, v ) < maxdist ) {
            maxdist = _Distance( head->position, v );
            closest = head;
        }

        head = head->next;
    }

    return closest;
}

wp_node *wp_getXClosest( wp_node **headRef, vec3_t v, int count ) {
    float maxdist = __FLT_MAX__;

    wp_node *nodes = NULL;
    wp_node *head = *headRef;
    while ( head->next != NULL ) {
        if ( _Distance( head->position, v ) < maxdist ) {
            maxdist = _Distance( head->position, v );
            wp_insertAtHead( &nodes, head->id, head->type, head->position );
        }

        head = head->next;
    }

    return nodes;
}

int main( void ) {
    if ( wp_init( "mp_chateau.wp" ) ) {
        printf( "success\n" );
    } else {
        return -1;
    }

    //wp_print( &wpMaster );

    vec3_t pos = { 0, 0, 0 };
    wp_node *v = wp_getClosest( &wpMaster, pos );
    if ( v != NULL ) {
        printf( "found closest: %d of type %s at (%f, %f, %f)\n", v->id, wp_tostr( v->type ), v->position[ 0 ], v->position[ 1 ], v->position[ 2 ] );
    }

    wp_node *nodes = wp_getXClosest( &wpMaster, pos, 10 );
    if ( nodes != NULL ) {
        //wp_print( &nodes );
    }
    wp_freeAll( &nodes );

    wp_freeAll( &wpMaster );

    return 0;
}
#endif

// --------------------------------------
bool wp_init( char* mapname ) {
    FILE *f = fopen( mapname, "r" );
    if ( f == NULL ) {
        printf( "wp_init() : %s\n", strerror( errno ) );
        return false;
    }

    char *line = NULL;
    ssize_t read;
    size_t len;
    int linenum = 0;

    while ( ( read = getline( &line, &len, f ) ) != -1 ) {
        // lines are semicolon delimited
        char *pch = strtok( line, ";" );
        if ( pch == NULL ) {
            printf( "wp_init() : bad syntax in \"%s\", line #%d -- skipping\n", mapname, linenum );
            goto cnt;
        }

        // first bit is the number
        int id = atoi( pch );

        // second bit is type
        pch = strtok( NULL, ";" );
        if ( pch == NULL ) {
            printf( "wp_init() : bad syntax in \"%s\", line #%d -- skipping\n", mapname, linenum );
            goto cnt;
        }

        int type = atoi( pch );
        if ( type < WP_DEFAULT || type > WP_NONE ) {
            printf( "wp_init() : invalid type %d, line #%d -- skipping\n", type, linenum );
            goto cnt;
        }

        // last bit should be vector
        pch = strtok( NULL, "\n" );
        if ( pch == NULL ) {
            printf( "wp_init() : bad formatting in \"%s\", line #%d -- skipping\n", mapname, linenum );
            goto cnt;
        }
        char *vec = pch;

        // vector must have ( ) and a ,
        if ( strchr( vec, '(' ) == NULL || strchr( vec, ')' ) == NULL || strchr( vec, ',' ) == NULL ) {
            printf( "wp_init() : invalid vector \"%s\", line #%d -- skipping\n", vec, linenum );
            goto cnt;
        }

        // strip starting and ending parantheses
        int lastpara = strchr( vec, ')' ) - vec;
        char *nvec = malloc( lastpara + 1 );
        strncpy( nvec, vec + 1, lastpara );
        nvec[ lastpara - 1 ] = '\0';

        // break apart
        char *vpch = strtok( nvec, "," );
        if ( vpch == NULL ) {
            printf( "wp_init() : invalid vector \"%s\", line #%d -- skipping\n", vec, linenum );
            goto cntvec;
        }

        float x = roundf( atof( vpch ) );

        vpch = strtok( NULL, "," );
        if ( vpch == NULL ) {
            printf( "wp_init() : invalid vector \"%s\", line #%d -- skipping\n", vec, linenum );
            goto cntvec;
        }

        float y = roundf( atof( vpch ) );

        vpch = strtok( NULL, "," );
        if ( vpch == NULL ) {
            printf( "wp_init() : invalid vector \"%s\", line #%d -- skipping\n", vec, linenum );
            goto cntvec;
        }

        float z = roundf( atof( vpch ) );

        // to vec3_t
        vec3_t position = { x, y, z };

        // build node
        wp_insertAtTail( &wpMaster, id, type, position );

    cntvec:
        free( nvec );

    cnt:
        linenum++;
    }

    free( line );
    fclose( f );

    printf( "wp_init() : successfully loaded \"%s\"\n", mapname );

    return true;
}

void wp_freeAll( wp_node **headRef ) {
    wp_node *top = *headRef;
    while ( top->next != NULL ) top = top->next;

    while ( top->prev != NULL ) {
        top = top->prev;
        free( top->next ); 
    }

    free( *headRef );
}

void wp_print( wp_node **headRef ) {
    wp_node *node = *headRef;

    printf( "waypoints: \n" );

    while ( node != NULL ) {
        printf( "%d, %s, (%f, %f, %f)\n", node->id, wp_tostr( node->type ), node->position[ 0 ], node->position[ 1 ], node->position[ 2 ] );
        node = node->next;
    }
}

wp_node *wp_newNode( int id, int type, vec3_t position ) {
    wp_node *node = (wp_node *)malloc( sizeof( wp_node ) );

    if ( node == NULL ) {
        printf( "wp_newNode() : problem malloc'ing\n" );
        return NULL;
    }

    node->id = id;
    node->type = type;
    //memcpy( node->position, position, sizeof( position ) );
    node->position[ 0 ] = position[ 0 ];
    node->position[ 1 ] = position[ 1 ];
    node->position[ 2 ] = position[ 2 ];
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void wp_insertAtHead( wp_node **headRef, int id, int type, vec3_t position ) {
    wp_node *node = wp_newNode( id, type, position );
    if ( *headRef == NULL ) {
        *headRef = node;
        return;
    }

    wpMaster->prev = node;
    node->next = *headRef;
    *headRef = node;
}

void wp_insertAtTail( wp_node **headRef, int id, int type, vec3_t position ) {
    wp_node *node = wp_newNode( id, type, position );
    if ( *headRef == NULL ) {
        *headRef = node;
        return;
    }

    wp_node *tmp = *headRef;
    while ( tmp->next != NULL ) tmp = tmp->next;
    tmp->next = node;
    node->prev = tmp;
}
