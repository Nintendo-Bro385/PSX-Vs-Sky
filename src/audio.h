/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_AUDIO_H
#define PSXF_GUARD_AUDIO_H

#include "psx.h"

//XA enumerations
typedef enum
{
	XA_Menu,   //MENU.XA
	XA_Week1A, //WEEK1A.XA
	XA_Week1B, //WEEK1B.XA
	XA_Dia, //DIA.XA
	XA_End, //END.XA
	
	XA_Max,
} XA_File;

typedef enum
{
	//MENU.XA
	XA_GettinFreaky, //Gettin' Freaky
	XA_GameOver,     //Game Over
	XA_Freeky,  //Tutorial
	XA_Ludum,  //Tutorial
	//WEEK1A.XA
	XA_Wifeforever, //Get rich
	XA_Sky,   //Short sale
	//WEEK1B.XA
	XA_Manifest, //Bankrupt
	XA_Clucked, //Clucked
	//DIA.XA
	XA_Shift,
	XA_Fest,
	XA_Cut,
	XA_Blank,
	//END.XA
	XA_Gend,
	XA_Bend,
	
	XA_TrackMax,
} XA_Track;

//Audio functions
void Audio_Init(void);
void Audio_Quit(void);
void Audio_PlayXA_Track(XA_Track track, u8 volume, u8 channel, boolean loop);
void Audio_SeekXA_Track(XA_Track track);
void Audio_PauseXA(void);
void Audio_ResumeXA(void);
void Audio_StopXA(void);
void Audio_ChannelXA(u8 channel);
s32 Audio_TellXA_Sector(void);
s32 Audio_TellXA_Milli(void);
boolean Audio_PlayingXA(void);
void Audio_WaitPlayXA(void);
void Audio_ProcessXA(void);

#endif
