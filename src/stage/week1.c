/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week1.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

int pink;
u8 animation = 0;
static const RECT wholescreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	Gfx_Tex tex_floor; //Floor
	
} Back_Week1;
void Back_Week1_DrawBG(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	

	
	fixed_t fx, fy;
	
	
	
	//Draw sunset
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT floorl_src = {0, 0, 255, 128};
	RECT_FIXED floorl_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(50,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};

	RECT floorr_src = {0, 128, 255, 128};
	RECT_FIXED floorr_dst = {
		FIXED_DEC(91 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(50,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};
	
	RECT bluebox = {60, 77, 147, 150};
	RECT leftbar = {0, 0, 90, 240};
	RECT rightbar = {177, 0, 233, 240};
	
	if(!(stage.stage_id == StageId_1_5))
	{
		if(stage.paused==false)
		{
			if(animation <=39)
				animation++;
			else
				animation=0;
		}
		switch (animation)
		{
			case 0:
				pink=0;
				break;
			case 10:
				pink=1;
				break;
			case 20:
				pink=2;
				break;
			case 30:
				pink=3;
				break;
		}
		if(stage.prefs.flashing ==0)
		{
			if(pink==0)
				Gfx_BlendRect(&wholescreen, 255, 255, 255, 0);
			else if(pink==1)
				Gfx_BlendRect(&wholescreen, 255, 242, 247, 0);
			else if(pink==2)
				Gfx_BlendRect(&wholescreen, 255, 229, 238, 0);
			else
				Gfx_BlendRect(&wholescreen, 255, 216, 230, 0);
		}
		else
			Gfx_BlendRect(&wholescreen, 255, 255, 255, 0);
	}
	else
	{
		if(stage.prefs.flashing ==0)
		{
			if(stage.cutdia==0)
			{
				if(stage.song_step ==5)
					Gfx_DrawRect(&wholescreen, 0, 0, 0);
				if(stage.song_step ==6)
			    		Gfx_DrawRect(&bluebox, 0, 0, 255);
			    	if(stage.song_step ==7)
			    	{
			    		Gfx_DrawRect(&leftbar, 0, 0, 255);
			    		Gfx_DrawRect(&rightbar, 0, 0, 255);
			    	}
			    	if(stage.song_step ==8)
			    	{
			    		Gfx_DrawRect(&leftbar, 0, 0, 170);
			    		Gfx_DrawRect(&rightbar, 0, 0, 170);
			    	}
			    	if(stage.song_step ==9)
			    	{
			    		Gfx_DrawRect(&leftbar, 0, 0, 170);
			    		Gfx_DrawRect(&rightbar, 0, 0, 170);
			    	}
			    	if(stage.song_step ==10)
			    	{
			    		Gfx_DrawRect(&leftbar, 0, 0, 85);
			    		Gfx_DrawRect(&rightbar, 0, 0, 85);
			    	}
			    	if(stage.song_step >10)
			    	{
			    		Gfx_DrawRect(&leftbar, 0, 0, 0);
			    		Gfx_DrawRect(&rightbar, 0, 0, 0);
			    	}
		    	}
		    	else
		    	{
		    		Gfx_BlendRect(&wholescreen, 255, 255, 255, 0);
		    	}
            	}
            	else
            	{
            		Gfx_DrawRect(&leftbar, 0, 0, 0);
			Gfx_DrawRect(&rightbar, 0, 0, 0);
            	}
            		
	}
	
	if (stage.prefs.lowquality ==0)
	{
		Stage_DrawTex(&this->tex_floor, &floorl_src, &floorl_dst, stage.camera.bzoom);
		Stage_DrawTex(&this->tex_floor, &floorr_src, &floorr_dst, stage.camera.bzoom);
	}
}

void Back_Week1_Free(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week1_New(void)
{
	//Allocate background structure
	Back_Week1 *this = (Back_Week1*)Mem_Alloc(sizeof(Back_Week1));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week1_DrawBG;
	this->back.free = Back_Week1_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK1\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_floor, Archive_Find(arc_back, "floor.tim"), 0);
	Mem_Free(arc_back);
	
	Gfx_SetClear(255, 255, 255);
	
	animation=0;
	return (StageBack*)this;
}
