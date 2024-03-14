/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "gfm.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"


//GFM character structure
enum
{
	GFM_ArcMain_Idle0,
	GFM_ArcMain_Idle1,
	GFM_ArcMain_Idle2,
	GFM_ArcMain_Idle3,
	GFM_ArcMain_Idle4,
	
	GFM_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main, arc_scene;
	IO_Data arc_ptr[GFM_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
	
} Char_GFM;

//GFM character definitions
static const CharFrame char_gfm_frame[] = {
	{GFM_ArcMain_Idle0, {  0,   0,  124, 176}, { 37,  72}}, //0
	{GFM_ArcMain_Idle0, {124,   0,  124, 176}, { 38,  72}}, //1
	{GFM_ArcMain_Idle1, {  0,   0,  124, 176}, { 37,  72}}, //2
	{GFM_ArcMain_Idle1, {124,   0,  124, 176}, { 38,  72}}, //3
	{GFM_ArcMain_Idle2, {  0,   0,  124, 176}, { 37,  72}}, //4
	{GFM_ArcMain_Idle2, {124,   0,  124, 176}, { 38,  72}}, //5
	{GFM_ArcMain_Idle3, {  0,   0,  124, 176}, { 37,  72}}, //6
	{GFM_ArcMain_Idle3, {124,   0,  124, 176}, { 38,  72}}, //7
	
	{GFM_ArcMain_Idle4, {  0,   0,  124, 176}, { 37,  72}}, //8
	{GFM_ArcMain_Idle4, {124,   0,  124, 176}, { 38,  72}}, //9
};

static const Animation char_gfm_anim[CharAnim_Max] = {
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                        //CharAnim_Idle
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                           //CharAnim_Left
	{1, (const u8[]){ 0,  0,  1,  1,  2,  2,  3, 3, ASCR_BACK, 1}}, //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                           //CharAnim_Down
	{1, (const u8[]){ 8, 8, 9, 9, 8, 8, 9, 9, ASCR_BACK, 1}}, //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                           //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                           //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                           //CharAnim_Right
	{1, (const u8[]){ 4,  4,  5,  5,  6,  6,  7, 7, ASCR_BACK, 1}}, //CharAnim_RightAlt
};

//GFM character functions
void Char_GFM_SetFrame(void *user, u8 frame)
{
	Char_GFM *this = (Char_GFM*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_gfm_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_GFM_Tick(Character *character)
{
	Char_GFM *this = (Char_GFM*)character;
	
		if (stage.flag & STAGE_FLAG_JUST_STEP)
		{
			
			//Perform dance
			if (stage.note_scroll >= character->sing_end && (stage.song_step % stage.gf_speed) == 0)
			{
				//Switch animation
				if (character->animatable.anim == CharAnim_LeftAlt || character->animatable.anim == CharAnim_Right)
					character->set_anim(character, CharAnim_RightAlt);
				else
					character->set_anim(character, CharAnim_LeftAlt);
			}
		}
	
	//Get parallax
	fixed_t parallax;
	parallax = FIXED_UNIT;
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_GFM_SetFrame);
	Character_DrawParallax(character, &this->tex, &char_gfm_frame[this->frame], parallax);
}

void Char_GFM_SetAnim(Character *character, u8 anim)
{
	//Set animation
	if (anim == CharAnim_Left || anim == CharAnim_Down || anim == CharAnim_Up || anim == CharAnim_Right || anim == CharAnim_UpAlt)
		character->sing_end = stage.note_scroll + FIXED_DEC(22,1); //Nearly 2 steps
	Animatable_SetAnim(&character->animatable, anim);
}

void Char_GFM_Free(Character *character)
{
	Char_GFM *this = (Char_GFM*)character;
	
	//Free art
	Mem_Free(this->arc_main);
	Mem_Free(this->arc_scene);
}

Character *Char_GFM_New(fixed_t x, fixed_t y)
{
	//Allocate gfm object
	Char_GFM *this = Mem_Alloc(sizeof(Char_GFM));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_GFM_New] Failed to allocate gfm object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_GFM_Tick;
	this->character.set_anim = Char_GFM_SetAnim;
	this->character.free = Char_GFM_Free;
	
	Animatable_Init(&this->character.animatable, char_gfm_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 0;
	
	this->character.focus_x = FIXED_DEC(2,1);
	this->character.focus_y = FIXED_DEC(-40,1);
	this->character.focus_zoom = FIXED_DEC(2,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\GFM.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //GFM_ArcMain_GFM0
		"idle1.tim", //GFM_ArcMain_GFM1
		"idle2.tim", //GFM_ArcMain_GFM2
		"idle3.tim",
		"idle4.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
