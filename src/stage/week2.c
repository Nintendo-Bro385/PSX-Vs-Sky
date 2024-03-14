/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week2.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

u8 animation2 = 0;
int red;
static const RECT wholescreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Wall left
	Gfx_Tex tex_back1; //Wall right
	
	Gfx_Tex tex_floor; //Floor
	
	Gfx_Tex tex_end1; //bad ending
	Gfx_Tex tex_end2; //good ending
	Gfx_Tex tex_end3; //peanut bf
	
} Back_Week2;
void Back_Week2_DrawFG(StageBack *back)
{
	
	Back_Week2 *this = (Back_Week2*)back;
	

	
	fixed_t fx, fy;
	
	fy = stage.camera.y;
	
	//Draw room
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT end_src = {0, 0, 256, 240};
	RECT_FIXED end_dst = {
		FIXED_DEC(-128 - SCREEN_WIDEOADD2,1),
		FIXED_DEC(-120,1),
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(240,1)
	};
	if(stage.stage_id == StageId_1_6)
	{
		if(stage.ending==1)
		{
			Stage_DrawTex(&this->tex_end1, &end_src, &end_dst, stage.camera.bzoom); 
		}
		else if(stage.ending==2)
		{
			Stage_DrawTex(&this->tex_end3, &end_src, &end_dst, stage.camera.bzoom); 
		}
		else
		{
			Stage_DrawTex(&this->tex_end2, &end_src, &end_dst, stage.camera.bzoom);
		}
		Gfx_DrawRect(&wholescreen, 0, 0, 0);
	}
}
void Back_Week2_DrawBG(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	

	
	fixed_t fx, fy;
	
	
	
	//Draw sunset
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	//Draw room
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT halll_src = {0, 0, 255, 256};
	RECT_FIXED halll_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};

	RECT hallr_src = {0, 0, 256, 256};
	RECT_FIXED hallr_dst = {
		FIXED_DEC(90 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};

	RECT floorl_src = {0, 0, 255, 125};
	RECT_FIXED floorl_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(76,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};

	RECT floorr_src = {0, 125, 255, 125};
	RECT_FIXED floorr_dst = {
		FIXED_DEC(91 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(74,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};
	if(!(stage.stage_id == StageId_1_6))
	{
		if (stage.prefs.lowquality ==0)
		{
			Stage_DrawTex(&this->tex_back0, &halll_src, &halll_dst, stage.camera.bzoom); 
			Stage_DrawTex(&this->tex_back1, &hallr_src, &hallr_dst, stage.camera.bzoom); 
			Stage_DrawTex(&this->tex_floor, &floorl_src, &floorl_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_floor, &floorr_src, &floorr_dst, stage.camera.bzoom);
		}
		if(stage.paused==false)
		{
			if(animation2 <=59)
				animation2++;
			else
				animation2=0;
		}
		
		switch (animation2)
		{
			case 0:
				red=0;
				break;
			case 15:
				red=1;
				break;
			case 30:
				red=2;
				break;
			case 45:
				red=3;
				break;
		}
		if(stage.prefs.flashing ==0)
		{
			if(red==0)
				Gfx_DrawRect(&wholescreen, 215, 19, 48);
			else if(red==1)
				Gfx_DrawRect(&wholescreen, 35, 0, 0);
			else if(red==2)
				Gfx_DrawRect(&wholescreen, 152, 0, 0);
			else
				Gfx_DrawRect(&wholescreen, 50, 0, 0);
		}
		else
			Gfx_DrawRect(&wholescreen, 215, 19, 48);
	}
}

void Back_Week2_Free(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week2_New(void)
{
	//Allocate background structure
	Back_Week2 *this = (Back_Week2*)Mem_Alloc(sizeof(Back_Week2));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_Week2_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week2_DrawBG;
	this->back.free = Back_Week2_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK2\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_floor, Archive_Find(arc_back, "floor.tim"), 0);
	Gfx_LoadTex(&this->tex_end1, Archive_Find(arc_back, "end1.tim"), 0);
	Gfx_LoadTex(&this->tex_end2, Archive_Find(arc_back, "end2.tim"), 0);
	Gfx_LoadTex(&this->tex_end3, Archive_Find(arc_back, "end3.tim"), 0);
	Mem_Free(arc_back);
	
	Gfx_SetClear(0, 0, 0);
	
	animation2=0;
	return (StageBack*)this;
}
