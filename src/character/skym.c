/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "skym.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Clucky character structure
enum
{
	Skym_ArcMain_Idle0,
	Skym_ArcMain_Idle1,
	Skym_ArcMain_Idle2,
	Skym_ArcMain_Left0,
	Skym_ArcMain_Left1,
	Skym_ArcMain_Down0,
	Skym_ArcMain_Down1,
	Skym_ArcMain_Up0,
	Skym_ArcMain_Up1,
	Skym_ArcMain_Up2,
	Skym_ArcMain_Right0,
	Skym_ArcMain_Right1,
	Skym_ArcMain_Right2,
	
	Skym_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Skym_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Skym;

//Clucky character definitions
static const CharFrame char_Skym_frame[] = {
	{Skym_ArcMain_Idle0, {  0,  0, 128, 144}, { 42, 130}},//0
	{Skym_ArcMain_Idle0, {128,  0, 128, 140}, { 42, 130}},//1
	{Skym_ArcMain_Idle1, {  0,  0, 128, 144}, { 42, 130}},//2
	{Skym_ArcMain_Idle1, {128,  0, 128, 140}, { 42, 130}},//3
	{Skym_ArcMain_Idle2, {  0,  0, 128, 144}, { 42, 130}},//4
	{Skym_ArcMain_Idle2, {128,  0, 128, 140}, { 42, 130}},//5
	
	{Skym_ArcMain_Left0, {  0,  0, 128, 144}, { 42, 130}},//6
	{Skym_ArcMain_Left0, {128,  0, 128, 140}, { 42, 130}},//7
	{Skym_ArcMain_Left0, {  0,  0, 128, 144}, { 42, 130}},//8
	{Skym_ArcMain_Left0, {128,  0, 128, 140}, { 42, 130}},//9
	
	{Skym_ArcMain_Down0, {  0,  0, 128, 144}, { 42, 130}},//10
	{Skym_ArcMain_Down0, {128,  0, 128, 140}, { 42, 130}},//11
	{Skym_ArcMain_Down1, {  0,  0, 128, 144}, { 42, 130}},//12
	{Skym_ArcMain_Down1, {128,  0, 128, 140}, { 42, 130}},//13
	{Skym_ArcMain_Right2, {128,  0, 128, 140}, { 42, 130}},//14
	
	{Skym_ArcMain_Up0, {  0,  0, 128, 144}, { 42, 130}},//15
	{Skym_ArcMain_Up0, {128,  0, 128, 140}, { 42, 130}},//16
	{Skym_ArcMain_Up1, {  0,  0, 128, 144}, { 42, 130}},//17
	{Skym_ArcMain_Up1, {128,  0, 128, 140}, { 42, 130}},//18
	{Skym_ArcMain_Up2, {  0,  0, 128, 144}, { 42, 130}},//19
	
	{Skym_ArcMain_Right0, {  0,  0, 128, 144}, { 42, 130}},//20
	{Skym_ArcMain_Right0, {128,  0, 128, 140}, { 42, 130}},//21
	{Skym_ArcMain_Right1, {  0,  0, 128, 144}, { 42, 130}},//22
	{Skym_ArcMain_Right1, {128,  0, 128, 140}, { 42, 130}},//23
	{Skym_ArcMain_Right2, {  0,  0, 128, 144}, { 42, 130}},//24
};

static const Animation char_Skym_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, 4, 5, ASCR_BACK, 6}}, //CharAnim_Idle
	{2, (const u8[]){ 6, 7, 8, 9, ASCR_BACK, 2}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 10, 11, 12, 13, 14, ASCR_BACK, 3}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 15, 16, 17, 18, 19, ASCR_BACK, 3}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 20, 21, 22, 23, 24, ASCR_BACK, 3}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Clucky character functions
void Char_Skym_SetFrame(void *user, u8 frame)
{
	Char_Skym *this = (Char_Skym*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Skym_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Skym_Tick(Character *character)
{
	Char_Skym *this = (Char_Skym*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Skym_SetFrame);
	Character_Draw(character, &this->tex, &char_Skym_frame[this->frame]);
}

void Char_Skym_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Skym_Free(Character *character)
{
	Char_Skym *this = (Char_Skym*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Skym_New(fixed_t x, fixed_t y)
{
	//Allocate clucky object
	Char_Skym *this = Mem_Alloc(sizeof(Char_Skym));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Skym_New] Failed to allocate skym object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Skym_Tick;
	this->character.set_anim = Char_Skym_SetAnim;
	this->character.free = Char_Skym_Free;
	
	Animatable_Init(&this->character.animatable, char_Skym_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 3; 
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SKYM.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", 
		"idle1.tim",
		"idle2.tim", 
		"left0.tim",  
		"left1.tim", 
		"down0.tim",
		"down1.tim",  
		"up0.tim",  
		"up1.tim", 
		"up2.tim", 
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
