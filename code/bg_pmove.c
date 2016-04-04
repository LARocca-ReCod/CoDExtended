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

#include "server.h"
#include "bg_public.h"

#define LOBYTE(x)   (*((unsigned char*)&(x)))   // low byte
#define LOWORD(x)   (*((short*)&(x)))   // low word
#define LODWORD(x)  (*((int*)&(x)))  // low dword
#define HIBYTE(x)   (*((unsigned char*)&(x)+1))
#define HIWORD(x)   (*((short*)&(x)+1))
#define HIDWORD(x)  (*((int*)&(x)+1))
#define BYTEn(x, n)   (*((unsigned char*)&(x)+n))
#define WORDn(x, n)   (*((short*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

#define OVERCLIP 1.001

pmove_t *pm;

static int (*BG_GetNumWeapons)();
static int (*BG_GetInfoForWeapon)(int);
static int (*BG_AnimScriptEvent)(playerState_t*,int,int,int);
static void (*PM_AddEvent)(int);

static void (*PM_SetWeaponReloadAddAmmoDelay)();
static void (*PM_SetReloadingState2)();

#define QUICK_RELOAD_FRACTION 3

static void PM_Weapon( void ) {

	
}

static void PM_SetReloadingState() {

}

static void PM_BeginWeaponReload() { //works for kar98 sniper? rest not? mmhm
	
}

static int PM_CheckJump() {
	return 0;
}

static int PM_Weapon_CheckForRechamber(int time) {
	return 0;
}


/*
==================
PM_ClipVelocity

Slide off of the impacting surface
==================
*/
void PM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce ) {
	float	backoff;
	float	change;
	int		i;
	
	backoff = DotProduct (in, normal);
	
	if ( backoff < 0 ) {
		backoff *= overbounce;
	} else {
		backoff /= overbounce;
	}

	for ( i=0 ; i<3 ; i++ ) {
		change = normal[i]*backoff;
		out[i] = in[i] - change;
	}
}

void _PM_ClearAimDownSightFlag() {

}

void _PM_UpdateAimDownSightFlag() {

}

void BG_Link() {

}