	{ //StageId_1_1 (Wife Forever)
		//Characters
		{Char_BF_New,    FIXED_DEC(170,1),  FIXED_DEC(125,1)},
		{Char_Sky_New, FIXED_DEC(0,1),  FIXED_DEC(125,1)},
		{Char_GF_New,     FIXED_DEC(90,1),  FIXED_DEC(30,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(22,10),FIXED_DEC(22,10),FIXED_DEC(24,10)},
		1, 1,
		XA_Wifeforever, 0,
		1, //dialogue check
		XA_Shift, 0, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SKY.ARC;1"},
		
		StageId_1_2, STAGE_LOAD_FLAG | STAGE_LOAD_OPPONENT
	},
	{ //StageId_1_2 (Sky)
		//Characters
		{Char_BF_New,    FIXED_DEC(170,1),  FIXED_DEC(125,1)},
		{Char_Skya_New, FIXED_DEC(-16,1),  FIXED_DEC(125,1)},
		{Char_GF_New,     FIXED_DEC(90,1),  FIXED_DEC(30,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(2,1),FIXED_DEC(24,10),FIXED_DEC(26,10)},
		1, 2,
		XA_Sky, 2,
		1, //dialogue check
		XA_Blank, 3, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SKY.ARC;1"},
		
		StageId_1_5, STAGE_LOAD_FLAG | STAGE_LOAD_STAGE | STAGE_LOAD_OPPONENT
	},
	{ //StageId_1_3 (Manifest)
		//Characters
		{Char_BF_New,    FIXED_DEC(170,1),  FIXED_DEC(125,1)},
		{Char_Skym_New, FIXED_DEC(-5,1),  FIXED_DEC(130,1)},
		{Char_GFM_New,     FIXED_DEC(65,1),  FIXED_DEC(20,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(26,10),FIXED_DEC(28,10),FIXED_DEC(28,10)},
		1, 3,
		XA_Manifest, 0,
		1, //dialogue check
		XA_Fest, 1, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SKY.ARC;1"},
		
		StageId_1_6,
	},
	{ //StageId_1_4 (Clucked)
		//Characters
		{Char_BF_New,    FIXED_DEC(225,1),  FIXED_DEC(138,1)},
		{Char_Clucky_New, FIXED_DEC(16,1),  FIXED_DEC(118,1)},
		{NULL},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(25,10),FIXED_DEC(25,10),FIXED_DEC(25,10)},
		1, 4,
		XA_Clucked, 2,
		0, //dialogue check
		XA_Wifeforever, 0, //dialogue song and it's channel
		
		StageId_1_4, 0
	},
	{ //StageId_1_5 (Cutscene)
		//Characters
		{Char_BF_New,    FIXED_DEC(170,1),  FIXED_DEC(125,1)},
		{Char_Skycut_New, FIXED_DEC(0-28,1),  FIXED_DEC(120,1)},
		{Char_GF_New,     FIXED_DEC(90,1),  FIXED_DEC(30,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(25,10),FIXED_DEC(25,10),FIXED_DEC(25,10)},
		1, 5,
		XA_Cut, 2,
		1, //dialogue check
		XA_Blank, 3, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SKY.ARC;1"},
		
		StageId_1_3, STAGE_LOAD_FLAG | STAGE_LOAD_STAGE | STAGE_LOAD_OPPONENT | STAGE_LOAD_GIRLFRIEND
	},
	{ //StageId_1_6 (Manifest)
		//Characters
		{Char_BF_New,    FIXED_DEC(170,1),  FIXED_DEC(125,1)},
		{Char_Skym_New, FIXED_DEC(-5,1),  FIXED_DEC(130,1)},
		{Char_GFM_New,     FIXED_DEC(65,1),  FIXED_DEC(20,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(26,10),FIXED_DEC(28,10),FIXED_DEC(28,10)},
		1, 6,
		XA_Manifest, 0,
		0, //dialogue check
		XA_Fest, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_1_6, 0
	},
