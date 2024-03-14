/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "titlerch.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

#include "speaker.h"


//GF Weeb character structure
enum
{
	Titlerch_ArcMain_Idle0,
	Titlerch_ArcMain_Idle1,
	Titlerch_ArcMain_Idle2,
	Titlerch_ArcMain_Idle3,
	Titlerch_ArcMain_Idle4,
	Titlerch_ArcMain_Idle5,
	Titlerch_ArcMain_Idle6,
	
	Titlerch_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Titlerch_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Titlerch;

//GF character definitions
static const CharFrame char_Titlerch_frame[] = {
	{Titlerch_ArcMain_Idle0, {  0,   0, 148, 142}, { 41, 189}}, //0 idle 1
	{Titlerch_ArcMain_Idle1, {  0,   0, 148, 142}, { 41, 189}}, //1 idle 2
	{Titlerch_ArcMain_Idle2, {  0,   0, 148, 142}, { 41, 189}}, //2 idle 3
	{Titlerch_ArcMain_Idle3, {  0,   0, 148, 142}, { 41, 189}}, //3 idle 4
	{Titlerch_ArcMain_Idle4, {  0,   0, 148, 142}, { 41, 189}}, //4
	{Titlerch_ArcMain_Idle5, {  0,   0, 148, 142}, { 41, 189}}, //5
	{Titlerch_ArcMain_Idle6, {  0,   0, 148, 142}, { 41, 189}}, //6

	
};

static const Animation char_Titlerch_anim[CharAnim_Max] = {
	{1, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, ASCR_BACK, 0}}, //CharAnim_Idle
};

//GF Weeb character functions
void Char_Titlerch_SetFrame(void *user, u8 frame)
{
	Char_Titlerch *this = (Char_Titlerch*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Titlerch_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Titlerch_Tick(Character *character)
{
	Char_Titlerch *this = (Char_Titlerch*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Get parallax
	fixed_t parallax;
	parallax = FIXED_DEC(85,100);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Titlerch_SetFrame);
	Character_DrawParallax(character, &this->tex, &char_Titlerch_frame[this->frame], parallax);
}

void Char_Titlerch_SetAnim(Character *character, u8 anim)
{
	//Set animation
	if (anim != CharAnim_Idle && anim != CharAnim_Left && anim != CharAnim_Right)
		return;
	Animatable_SetAnim(&character->animatable, anim);
}

void Char_Titlerch_Free(Character *character)
{
	Char_Titlerch *this = (Char_Titlerch*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Titlerch_New(fixed_t x, fixed_t y)
{
	//Allocate gf weeb object
	Char_Titlerch *this = Mem_Alloc(sizeof(Char_Titlerch));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Titlerch_New] Failed to allocate gf weeb object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Titlerch_Tick;
	this->character.set_anim = Char_Titlerch_SetAnim;
	this->character.free = Char_Titlerch_Free;
	
	Animatable_Init(&this->character.animatable, char_Titlerch_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(16,1);
	this->character.focus_y = FIXED_DEC(-50,1);
	this->character.focus_zoom = FIXED_DEC(13,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\TITLERCH.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",  //Titlerch_ArcMain_idle0
		"idle1.tim",  //Titlerch_ArcMain_idle1
		"idle2.tim",  //Titlerch_ArcMain_idle2
		"idle3.tim",  //Titlerch_ArcMain_idle3
		"idle4.tim",  //Titlerch_ArcMain_idle4
		"idle5.tim",  //Titlerch_ArcMain_idle5
		"idle6.tim",  //Titlerch_ArcMain_idle6
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
