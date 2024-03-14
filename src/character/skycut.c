/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "skycut.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Clucky character structure
enum
{
	Skycut_ArcMain_Idle0,
	Skycut_ArcMain_Idle1,
	Skycut_ArcMain_Idle2,
	Skycut_ArcMain_Idle3,
	Skycut_ArcMain_Idle4,
	Skycut_ArcMain_Idle5,
	Skycut_ArcMain_Idle6,
	Skycut_ArcMain_Idle7,
	Skycut_ArcMain_Idle8,
	Skycut_ArcMain_Idle9,
	Skycut_ArcMain_Idle10,
	Skycut_ArcMain_Idle11,
	Skycut_ArcMain_Idle12,
	Skycut_ArcMain_Idle13,
	Skycut_ArcMain_Idle14,
	Skycut_ArcMain_Idle15,
	
	Skycut_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Skycut_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Skycut;

//Clucky character definitions
static const CharFrame char_Skycut_frame[] = {
	{Skycut_ArcMain_Idle0, {  0,   0, 160, 170}, { 42, 130}}, //0
	{Skycut_ArcMain_Idle1, {  0,   0, 160, 170}, { 42, 130}}, //1
	{Skycut_ArcMain_Idle2, {  0,   0, 160, 170}, { 42, 130}}, //2
	{Skycut_ArcMain_Idle3, {  0,   0, 160, 170}, { 42, 130}}, //3
	{Skycut_ArcMain_Idle4, {  0,   0, 160, 170}, { 42, 130}}, //4
	{Skycut_ArcMain_Idle5, {  0,   0, 160, 170}, { 42, 130}}, //5
	{Skycut_ArcMain_Idle6, {  0,   0, 160, 170}, { 42, 130}}, //6
	{Skycut_ArcMain_Idle7, {  0,   0, 160, 170}, { 42, 130}}, //7
	{Skycut_ArcMain_Idle8, {  0,   0, 160, 170}, { 42, 130}}, //8
	{Skycut_ArcMain_Idle9, {  0,   0, 160, 170}, { 42, 130}}, //9
	{Skycut_ArcMain_Idle10, {  0,   0, 160, 170}, { 42, 130}}, //10
	{Skycut_ArcMain_Idle11, {  0,   0, 160, 170}, { 42, 130}}, //11
	{Skycut_ArcMain_Idle12, {  0,   0, 160, 170}, { 42, 130}}, //12
	{Skycut_ArcMain_Idle13, {  0,   0, 160, 170}, { 42, 130}}, //13
	{Skycut_ArcMain_Idle14, {  0,   0, 160, 170}, { 42, 130}}, //14
	{Skycut_ArcMain_Idle15, {  0,   0, 160, 170}, { 42, 130}}, //15
};

static const Animation char_Skycut_anim[CharAnim_Max] = {
	{1, (const u8[]){ 13, 14, 15, ASCR_BACK, 3}}, //CharAnim_Idle
	{2, (const u8[]){ 1, 2, 3, 4, 5, 6, ASCR_BACK, 6}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7, 8, 9, 10, 11, 12, ASCR_BACK, 6}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){ 0, 0, 0, ASCR_BACK, 1}},        //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},        //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Clucky character functions
void Char_Skycut_SetFrame(void *user, u8 frame)
{
	Char_Skycut *this = (Char_Skycut*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Skycut_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Skycut_Tick(Character *character)
{
	Char_Skycut *this = (Char_Skycut*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Skycut_SetFrame);
	Character_Draw(character, &this->tex, &char_Skycut_frame[this->frame]);
	if(stage.cutdia==0)
	{
		if(stage.song_step <-1)
			character->set_anim(character, CharAnim_Up);
		else if(stage.song_step ==-1)
			character->set_anim(character, CharAnim_Left);
		else if(stage.song_step ==3)
			character->set_anim(character, CharAnim_Down);
		else if(stage.song_step ==8)
			character->set_anim(character, CharAnim_Idle);
	}
	else
	{
		character->set_anim(character, CharAnim_Up);
	}
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		{
		    switch (stage.song_step)
			{
			    case -2:
			    {
					this->character.focus_x = FIXED_DEC(55,1);
					this->character.focus_y = FIXED_DEC(-90,1);
					this->character.focus_zoom = FIXED_DEC(13,10);
			    };
			}
		}
}

void Char_Skycut_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Skycut_Free(Character *character)
{
	Char_Skycut *this = (Char_Skycut*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Skycut_New(fixed_t x, fixed_t y)
{
	//Allocate clucky object
	Char_Skycut *this = Mem_Alloc(sizeof(Char_Skycut));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Skycut_New] Failed to allocate skycut object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Skycut_Tick;
	this->character.set_anim = Char_Skycut_SetAnim;
	this->character.free = Char_Skycut_Free;
	
	Animatable_Init(&this->character.animatable, char_Skycut_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 2; 
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SKYCUT.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", 
		"idle1.tim",
		"idle2.tim",
		"idle3.tim", 
		"idle4.tim", 
		"idle5.tim", 
		"idle6.tim", 
		"idle7.tim",
		"idle8.tim",
		"idle9.tim",
		"idle10.tim",
		"idle11.tim",
		"idle12.tim",
		"idle13.tim",
		"idle14.tim",
		"idle15.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
