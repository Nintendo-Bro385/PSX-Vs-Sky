/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "skya.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

boolean pissed;
//Clucky character structure
enum
{
	Skya_ArcMain_Idle0,
	Skya_ArcMain_Idle1,
	Skya_ArcMain_Idle2,
	Skya_ArcMain_Idle3,
	Skya_ArcMain_Left0,
	Skya_ArcMain_Left1,
	Skya_ArcMain_Left2,
	Skya_ArcMain_Down0,
	Skya_ArcMain_Down1,
	Skya_ArcMain_Down2,
	Skya_ArcMain_Down3,
	Skya_ArcMain_Up0,
	Skya_ArcMain_Up1,
	Skya_ArcMain_Right0,
	Skya_ArcMain_Right1,
	Skya_ArcMain_Right2,
	Skya_ArcMain_Right3,
	Skya_ArcMain_Upalt,
	
	Skya_ArcMain_PIdle0,
	Skya_ArcMain_PIdle1,
	Skya_ArcMain_PIdle2,
	Skya_ArcMain_PIdle3,
	Skya_ArcMain_PLeft0,
	Skya_ArcMain_PLeft1,
	Skya_ArcMain_PLeft2,
	Skya_ArcMain_PLeft3,
	Skya_ArcMain_PDown0,
	Skya_ArcMain_PDown1,
	Skya_ArcMain_PDown2,
	Skya_ArcMain_PDown3,
	Skya_ArcMain_PUp0,
	Skya_ArcMain_PUp1,
	Skya_ArcMain_PRight0,
	Skya_ArcMain_PRight1,
	Skya_ArcMain_PRight2,
	Skya_ArcMain_PRight3,
	Skya_ArcMain_PLefta0,
	Skya_ArcMain_PLefta1,
	Skya_ArcMain_PDowna0,
	Skya_ArcMain_PDowna1,
	Skya_ArcMain_PUpalt0,
	Skya_ArcMain_PUpalt1,
	
	Skya_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Skya_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Skya;

//Clucky character definitions
static const CharFrame char_Skya_frame[] = {
	{Skya_ArcMain_Idle0, {  0,   0, 112, 141}, { 42, 130}},//0 
	{Skya_ArcMain_Idle0, {112,   0, 112, 141}, { 42, 130}},//1 
	{Skya_ArcMain_Idle1, {  0,   0, 112, 141}, { 42, 130}},//2
	{Skya_ArcMain_Idle1, {112,   0, 112, 141}, { 42, 130}},//3 
	{Skya_ArcMain_Idle2, {  0,   0, 112, 141}, { 42, 130}},//4 
	{Skya_ArcMain_Idle2, {112,   0, 112, 141}, { 42, 130}},//5 
	{Skya_ArcMain_Idle3, {  0,   0, 112, 141}, { 42, 130}},//6 
	{Skya_ArcMain_Idle3, {112,   0, 112, 141}, { 42, 130}},//7
	
	{Skya_ArcMain_Left0, {  0,   0, 112, 141}, { 42, 130}},//8
	{Skya_ArcMain_Left0, {112,   0, 112, 141}, { 42, 130}},//9
	{Skya_ArcMain_Left1, {  0,   0, 112, 141}, { 42, 130}},//10
	{Skya_ArcMain_Left1, {112,   0, 112, 141}, { 42, 130}},//11
	{Skya_ArcMain_Left2, {  0,   0, 112, 141}, { 42, 130}},//12
	{Skya_ArcMain_Left2, {112,   0, 112, 141}, { 42, 130}},//13
	{Skya_ArcMain_Left2, {  0,   0, 112, 141}, { 42, 130}},//14
	{Skya_ArcMain_Left2, {112,   0, 112, 141}, { 42, 130}},//15 
	{Skya_ArcMain_Left2, {  0,   0, 112, 141}, { 42, 130}},//16 
	
	{Skya_ArcMain_Left2, {112,   0, 112, 141}, { 42, 130}},//17 
	{Skya_ArcMain_Down0, {  0,   0, 112, 141}, { 42, 130}},//18
	{Skya_ArcMain_Down0, {112,   0, 112, 141}, { 42, 130}},//19
	{Skya_ArcMain_Down1, {  0,   0, 112, 141}, { 42, 130}},//20
	{Skya_ArcMain_Down1, {112,   0, 112, 141}, { 42, 130}},//21
	{Skya_ArcMain_Down2, {  0,   0, 112, 141}, { 42, 130}},//22
	{Skya_ArcMain_Down2, {112,   0, 112, 141}, { 42, 130}},//23
	{Skya_ArcMain_Down3, {  0,   0, 112, 141}, { 42, 130}},//24
	{Skya_ArcMain_Down3, {112,   0, 112, 141}, { 42, 130}},//25
	
	{Skya_ArcMain_Up0, {  0,   0, 112, 141}, { 42, 130}},//26
	{Skya_ArcMain_Up0, {112,   0, 112, 141}, { 42, 130}},//27
	{Skya_ArcMain_Up1, {  0,   0, 112, 141}, { 42, 130}},//28
	{Skya_ArcMain_Up1, {112,   0, 112, 141}, { 42, 130}},//29
	{Skya_ArcMain_Right3, {112,   0, 112, 141}, { 42, 130}},//30
	
	{Skya_ArcMain_Right0, {  0,   0, 112, 141}, { 42, 130}},//31
	{Skya_ArcMain_Right0, {112,   0, 112, 141}, { 42, 130}},//32
	{Skya_ArcMain_Right1, {  0,   0, 112, 141}, { 42, 130}},//33
	{Skya_ArcMain_Right1, {112,   0, 112, 141}, { 42, 130}},//34
	{Skya_ArcMain_Right2, {  0,   0, 112, 141}, { 42, 130}},//35
	{Skya_ArcMain_Right2, {112,   0, 112, 141}, { 42, 130}},//36
	{Skya_ArcMain_Right3, {  0,   0, 112, 141}, { 42, 130}},//37
	
	{Skya_ArcMain_Upalt, {  0,   0, 112, 141}, { 42, 130}},//38
	
	//Sky annoyed pissed
	
	{Skya_ArcMain_PIdle0, {  0,   0, 112, 141}, { 42, 130}},//39
	{Skya_ArcMain_PIdle0, {112,   0, 112, 141}, { 42, 130}},//40
	{Skya_ArcMain_PIdle1, {  0,   0, 112, 141}, { 42, 130}},//41
	{Skya_ArcMain_PIdle1, {112,   0, 112, 141}, { 42, 130}},//42
	{Skya_ArcMain_PIdle2, {  0,   0, 112, 141}, { 42, 130}},//43
	{Skya_ArcMain_PIdle2, {112,   0, 112, 141}, { 42, 130}},//44
	{Skya_ArcMain_PIdle3, {  0,   0, 112, 141}, { 42, 130}},//45
	{Skya_ArcMain_PIdle3, {112,   0, 112, 141}, { 42, 130}},//46
	
	{Skya_ArcMain_PLeft0, {  0,   0, 112, 141}, { 42, 130}},//47
	{Skya_ArcMain_PLeft0, {112,   0, 112, 141}, { 42, 130}},//48
	{Skya_ArcMain_PLeft1, {  0,   0, 112, 141}, { 42, 130}},//49
	{Skya_ArcMain_PLeft1, {112,   0, 112, 141}, { 42, 130}},//50
	{Skya_ArcMain_PLeft2, {  0,   0, 112, 141}, { 42, 130}},//51
	{Skya_ArcMain_PLeft2, {112,   0, 112, 141}, { 42, 130}},//52
	{Skya_ArcMain_PLeft3, {  0,   0, 112, 141}, { 42, 130}},//53
	{Skya_ArcMain_PLeft3, {112,   0, 112, 141}, { 42, 130}},//54 
	
	{Skya_ArcMain_PDown0, {  0,   0, 112, 141}, { 42, 130}},//55
	{Skya_ArcMain_PDown0, {112,   0, 112, 141}, { 42, 130}},//56
	{Skya_ArcMain_PDown1, {  0,   0, 112, 141}, { 42, 130}},//57
	{Skya_ArcMain_PDown1, {112,   0, 112, 141}, { 42, 130}},//58
	{Skya_ArcMain_PDown2, {  0,   0, 112, 141}, { 42, 130}},//59
	{Skya_ArcMain_PDown2, {112,   0, 112, 141}, { 42, 130}},//60
	{Skya_ArcMain_PDown3, {  0,   0, 112, 141}, { 42, 130}},//61
	{Skya_ArcMain_PDown3, {112,   0, 112, 141}, { 42, 130}},//62
	{Skya_ArcMain_PRight3, {112,   0, 112, 141}, { 42, 130}},//63
	
	{Skya_ArcMain_PUp0, {  0,   0, 112, 141}, { 42, 130}},//64
	{Skya_ArcMain_PUp0, {112,   0, 112, 141}, { 42, 130}},//65
	{Skya_ArcMain_PUp1, {  0,   0, 112, 141}, { 42, 130}},//66
	{Skya_ArcMain_PUp1, {112,   0, 112, 141}, { 42, 130}},//67
	{Skya_ArcMain_PLefta1, {112,   0, 112, 141}, { 42, 130}},//68
	
	{Skya_ArcMain_PRight0, {  0,   0, 112, 141}, { 42, 130}},//69
	{Skya_ArcMain_PRight0, {112,   0, 112, 141}, { 42, 130}},//70
	{Skya_ArcMain_PRight1, {  0,   0, 112, 141}, { 42, 130}},//71
	{Skya_ArcMain_PRight1, {112,   0, 112, 141}, { 42, 130}},//72
	{Skya_ArcMain_PRight2, {  0,   0, 112, 141}, { 42, 130}},//73
	{Skya_ArcMain_PRight2, {112,   0, 112, 141}, { 42, 130}},//74
	{Skya_ArcMain_PRight3, {  0,   0, 112, 141}, { 42, 130}},//75
	
	{Skya_ArcMain_PLefta0, {  0,   0, 112, 141}, { 42, 130}},//76
	{Skya_ArcMain_PLefta0, {112,   0, 112, 141}, { 42, 130}},//77
	{Skya_ArcMain_PLefta1, {  0,   0, 112, 141}, { 42, 130}},//78
	
	{Skya_ArcMain_PDowna0, {  0,   0, 112, 141}, { 42, 130}},//79
	{Skya_ArcMain_PDowna0, {112,   0, 112, 141}, { 42, 130}},//80
	{Skya_ArcMain_PDowna1, {  0,   0, 112, 141}, { 42, 130}},//81
	{Skya_ArcMain_PDowna1, {112,   0, 112, 141}, { 42, 130}},//82
	
	{Skya_ArcMain_PUpalt0, {  0,   0, 112, 141}, { 42, 130}},//83
	{Skya_ArcMain_PUpalt0, {112,   0, 112, 141}, { 42, 130}},//84
	{Skya_ArcMain_PUpalt1, {  0,   0, 112, 141}, { 42, 130}},//85
};

static const Animation char_Skya_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, 4, 5, 6, 7, ASCR_BACK, 8}}, //CharAnim_Idle
	{1, (const u8[]){ 8,  9, 10, 11, 12, ASCR_BACK, 3}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{1, (const u8[]){ 18,  19, 20, 21, 22, 23, 24, 25, 17, ASCR_BACK, 5}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){ 26,  27, 28, 29, 30, ASCR_BACK, 3}},         //CharAnim_Up
	{1, (const u8[]){ 38, ASCR_BACK, 1}},   //CharAnim_UpAlt
	{2, (const u8[]){ 31, 32, 33, 34, 35, 36, 37, ASCR_BACK, 4}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

static const Animation char_PSkya_anim[CharAnim_Max] = {
	{2, (const u8[]){ 39, 40, 41, 42, 43, 44, 45, 46, ASCR_BACK, 8}}, //CharAnim_Idle
	{1, (const u8[]){ 47,  48, 49, 50, 51, 52, 53, 54, ASCR_BACK, 5}},         //CharAnim_Left
	{1, (const u8[]){ 76, 77, 78, ASCR_BACK, 1}},   //CharAnim_LeftAlt
	{1, (const u8[]){ 55,  56, 57, 58, 59, 60, 61, 62, 63, ASCR_BACK, 5}},         //CharAnim_Down
	{2, (const u8[]){ 79, 80, 81, 82, ASCR_BACK, 4}},   //CharAnim_DownAlt
	{1, (const u8[]){ 64,  65, 66, 67, 68, ASCR_BACK, 3}},         //CharAnim_Up
	{2, (const u8[]){ 83, 83, 84, 84, 85, 85, ASCR_BACK, 1}},    //CharAnim_UpAlt
	{2, (const u8[]){ 69, 70, 71, 72, 73, 74, 75, ASCR_BACK, 4}},         //CharAnim_Right
	{4, (const u8[]){ 85, ASCR_BACK, 1}},   //CharAnim_RightAlt
};

//Clucky character functions
void Char_Skya_SetFrame(void *user, u8 frame)
{
	Char_Skya *this = (Char_Skya*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Skya_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Skya_Tick(Character *character)
{
	Char_Skya *this = (Char_Skya*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
    	{   //Stage specific animations
		if (stage.note_scroll >= 0)
		{
			switch (stage.stage_id)
			{
				case StageId_1_2: //Animations
				{
					switch (stage.song_step)
					{
						case 121:
							character->set_anim(character, CharAnim_UpAlt);
							break;
						case 248:
							character->set_anim(character, CharAnim_UpAlt);
							break;
						case 504:
							character->set_anim(character, CharAnim_DownAlt);
							break;
						case 760:
							character->set_anim(character, CharAnim_DownAlt);
							break;
						case 1080:
							character->set_anim(character, CharAnim_LeftAlt);
							break;
						case 1143:
							character->set_anim(character, CharAnim_UpAlt);
							break;
					}
					if(stage.song_step>=1145)
					{
						character->set_anim(character, CharAnim_RightAlt);
					}
				}
				default:
					break;
			}
		}
	}
	if(pissed==1)
	{
		//Animate and draw pissed sky
		Animatable_Animate(&character->animatable2, (void*)this, Char_Skya_SetFrame);
		Character_Draw(character, &this->tex, &char_Skya_frame[this->frame]);
	}
	else
	{
		//Animate and draw annoyed sky
		Animatable_Animate(&character->animatable, (void*)this, Char_Skya_SetFrame);
		Character_Draw(character, &this->tex, &char_Skya_frame[this->frame]);
	}
	if (stage.song_step == 504)
	{
		pissed = 1;
	}
}

void Char_Skya_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Animatable_SetAnim(&character->animatable2, anim);
	Character_CheckStartSing(character);
}

void Char_Skya_Free(Character *character)
{
	Char_Skya *this = (Char_Skya*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Skya_New(fixed_t x, fixed_t y)
{
	//Allocate clucky object
	Char_Skya *this = Mem_Alloc(sizeof(Char_Skya));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Skya_New] Failed to allocate skya object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Skya_Tick;
	this->character.set_anim = Char_Skya_SetAnim;
	this->character.free = Char_Skya_Free;
	
	Animatable_Init(&this->character.animatable, char_Skya_anim);
	Animatable_Init(&this->character.animatable2, char_PSkya_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 2; 
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SKYA.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", 
		"idle1.tim",
		"idle2.tim",
		"idle3.tim", 
		"left0.tim",  
		"left1.tim",
		"left2.tim", 
		"down0.tim",
		"down1.tim",  
		"down2.tim", 
		"down3.tim", 
		"up0.tim",  
		"up1.tim",  
		"right0.tim", 
		"right1.tim", 
		"right2.tim",
		"right3.tim",
		"upalt.tim",
		"pidle0.tim", 
		"pidle1.tim",
		"pidle2.tim",
		"pidle3.tim", 
		"pleft0.tim",  
		"pleft1.tim",
		"pleft2.tim", 
		"pleft3.tim", 
		"pdown0.tim",
		"pdown1.tim",  
		"pdown2.tim", 
		"pdown3.tim", 
		"pup0.tim",  
		"pup1.tim",  
		"pright0.tim", 
		"pright1.tim", 
		"pright2.tim",
		"pright3.tim",
		"plefta0.tim",
		"plefta1.tim",
		"pdowna0.tim",
		"pdowna1.tim",
		"pupalt0.tim",
		"pupalt1.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	pissed = 0;
	
	return (Character*)this;
}
