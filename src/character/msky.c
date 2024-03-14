/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "msky.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Msky character structure
enum
{
	Msky_ArcMain_Menusky,
	
	Msky_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Msky_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Msky;

//Msky character definitions
static const CharFrame char_msky_frame[] = {
	{Msky_ArcMain_Menusky, {  0,   0,  60,  99}, { 53,  92}}, //idle0 0
	{Msky_ArcMain_Menusky, { 60,   0,  60,  99}, { 53,  92}}, //idle1 1
	{Msky_ArcMain_Menusky, {120,   0,  60,  99}, { 53,  92}}, //idle2 2
	{Msky_ArcMain_Menusky, {180,   0,  60,  99}, { 53,  92}}, //idle3 3
};

static const Animation char_msky_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0, 0, 1, 1, 2, 2, 3, 3, ASCR_BACK, 8}}, //CharAnim_Idle
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},          //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},        //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Msky character functions
void Char_Msky_SetFrame(void *user, u8 frame)
{
	Char_Msky *this = (Char_Msky*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_msky_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Msky_Tick(Character *character)
{
	Char_Msky *this = (Char_Msky*)character;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{	
		//Perform dance
		if (stage.note_scroll >= character->sing_end && (stage.song_step % stage.gf_speed) == 0)
			character->set_anim(character, CharAnim_Idle);
		
	}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Msky_SetFrame);
	Character_Draw(character, &this->tex, &char_msky_frame[this->frame]);
}

void Char_Msky_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Msky_Free(Character *character)
{
	Char_Msky *this = (Char_Msky*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Msky_New(fixed_t x, fixed_t y)
{
	//Allocate msky object
	Char_Msky *this = Mem_Alloc(sizeof(Char_Msky));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Msky_New] Failed to allocate msky object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Msky_Tick;
	this->character.set_anim = Char_Msky_SetAnim;
	this->character.free = Char_Msky_Free;
	
	Animatable_Init(&this->character.animatable, char_msky_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 11;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MSKY.ARC;1");
	
	const char **pathp = (const char *[]){
		"menusky.tim", //Msky_ArcMain_Idle
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
