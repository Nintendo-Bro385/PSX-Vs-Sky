/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sky.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Clucky character structure
enum
{
	Sky_ArcMain_Idle0,
	Sky_ArcMain_Idle1,
	Sky_ArcMain_Idle2,
	Sky_ArcMain_Idle3,
	Sky_ArcMain_Idle4,
	Sky_ArcMain_Idle5,
	Sky_ArcMain_Idle6,
	Sky_ArcMain_Idle7,
	Sky_ArcMain_Left0,
	Sky_ArcMain_Left1,
	Sky_ArcMain_Down0,
	Sky_ArcMain_Down1,
	Sky_ArcMain_Up0,
	Sky_ArcMain_Up1,
	Sky_ArcMain_Right0,
	Sky_ArcMain_Right1,
	Sky_ArcMain_Right2,
	
	Sky_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Sky_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Sky;

//Clucky character definitions
static const CharFrame char_Sky_frame[] = {
	{Sky_ArcMain_Idle0, {  0,   0, 96, 140}, { 42, 130}}, //0 
	{Sky_ArcMain_Idle0, {  96,   0, 96, 140}, { 42, 130}}, //1 
	{Sky_ArcMain_Idle1, {  0,   0, 96, 140}, { 42, 130}}, //2 
	{Sky_ArcMain_Idle1, {  96,   0, 96, 140}, { 42, 130}}, //3 
	{Sky_ArcMain_Idle2, {  0,   0, 96, 140}, { 42, 130}}, //4 
	{Sky_ArcMain_Idle2, {  96,   0, 96, 140}, { 42, 130}}, //5 
	{Sky_ArcMain_Idle3, {  0,   0, 96, 140}, { 42, 130}}, //6 
	{Sky_ArcMain_Idle3, {  96,   0, 96, 140}, { 42, 130}}, //7 
	{Sky_ArcMain_Idle4, {  0,   0, 96, 140}, { 42, 130}}, //8 
	{Sky_ArcMain_Idle4, {  96,   0, 96, 140}, { 42, 130}}, //9 
	{Sky_ArcMain_Idle5, {  0,   0, 96, 140}, { 42, 130}}, //10 
	{Sky_ArcMain_Idle5, {  96,   0, 96, 140}, { 42, 130}}, //11 
	{Sky_ArcMain_Idle6, {  0,   0, 96, 140}, { 42, 130}}, //12 
	{Sky_ArcMain_Idle6, {  96,   0, 96, 140}, { 42, 130}}, //13 
	{Sky_ArcMain_Idle7, {  0,   0, 96, 140}, { 42, 130}}, //14 
	{Sky_ArcMain_Idle7, {  96,   0, 96, 140}, { 42, 130}}, //15
	
	{Sky_ArcMain_Left0, {  0,   0, 96, 140}, { 42, 130}}, //16
	{Sky_ArcMain_Left0, {  96,   0, 96, 140}, { 42, 130}},//17
	{Sky_ArcMain_Left1, {  0,   0, 96, 140}, { 42, 130}}, //18
	{Sky_ArcMain_Left1, {  96,   0, 96, 140}, { 42, 130}},//19
	
	{Sky_ArcMain_Down0, {  0,   0, 96, 140}, { 42, 130}}, //20
	{Sky_ArcMain_Down0, {  96,   0, 96, 140}, { 42, 130}},//21
	{Sky_ArcMain_Down1, {  0,   0, 96, 140}, { 42, 130}}, //22
	{Sky_ArcMain_Down1, {  96,   0, 96, 140}, { 42, 130}},//23
	
	{Sky_ArcMain_Up0, {  0,   0, 96, 140}, { 42, 130}}, //24
	{Sky_ArcMain_Up0, {  96,   0, 96, 140}, { 42, 130}},//25
	{Sky_ArcMain_Up1, {  0,   0, 96, 140}, { 42, 130}}, //26
	{Sky_ArcMain_Up1, {  96,   0, 96, 140}, { 42, 130}},//27
	{Sky_ArcMain_Right2, {  96,   0, 96, 140}, { 42, 130}},//28
	
	{Sky_ArcMain_Right0, {  0,   0, 96, 140}, { 42, 130}}, //29
	{Sky_ArcMain_Right0, {  96,   0, 96, 140}, { 42, 130}},//30
	{Sky_ArcMain_Right1, {  0,   0, 96, 140}, { 42, 130}}, //31
	{Sky_ArcMain_Right1, {  96,   0, 96, 140}, { 42, 130}},//32
	{Sky_ArcMain_Right2, {  0,   0, 96, 140}, { 42, 130}}, //33
};

static const Animation char_Sky_anim[CharAnim_Max] = {
	{1, (const u8[]){ 0,  1,  2,  3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, ASCR_BACK, 16}}, //CharAnim_Idle
	{2, (const u8[]){ 16,  17, 18, 19, ASCR_BACK, 2}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 20,  21, 22, 23, ASCR_BACK, 2}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 24,  25, 26, 27, 28, ASCR_BACK, 3}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){29, 30, 31, 32, 33, ASCR_BACK, 3}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Clucky character functions
void Char_Sky_SetFrame(void *user, u8 frame)
{
	Char_Sky *this = (Char_Sky*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Sky_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Sky_Tick(Character *character)
{
	Char_Sky *this = (Char_Sky*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Sky_SetFrame);
	Character_Draw(character, &this->tex, &char_Sky_frame[this->frame]);
}

void Char_Sky_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Sky_Free(Character *character)
{
	Char_Sky *this = (Char_Sky*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Sky_New(fixed_t x, fixed_t y)
{
	//Allocate clucky object
	Char_Sky *this = Mem_Alloc(sizeof(Char_Sky));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Sky_New] Failed to allocate sky object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Sky_Tick;
	this->character.set_anim = Char_Sky_SetAnim;
	this->character.free = Char_Sky_Free;
	
	Animatable_Init(&this->character.animatable, char_Sky_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 2; 
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SKY.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", 
		"idle1.tim",
		"idle2.tim",
		"idle3.tim", 
		"idle4.tim", 
		"idle5.tim", 
		"idle6.tim", 
		"idle7.tim", 
		"left0.tim",  
		"left1.tim", 
		"down0.tim",
		"down1.tim",  
		"up0.tim",  
		"up1.tim",  
		"right0.tim", 
		"right1.tim", 
		"right2.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
