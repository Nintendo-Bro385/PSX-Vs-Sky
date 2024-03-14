#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11295)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3840)},  //XA_GameOver
	{XA_Menu, XA_LENGTH(11295)}, //XA_Freeky
	{XA_Menu, XA_LENGTH(3700)}, //XA_Ludum
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(10000)}, //XA_Wifeforever
	{XA_Week1A, XA_LENGTH(8700)}, //XA_SKy
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(11600)}, //XA_Manifest
	{XA_Week1B, XA_LENGTH(11000)}, //XA_Clucked
	//DIA.XA
	{XA_Dia, XA_LENGTH(5200)}, //XA_Shift
	{XA_Dia, XA_LENGTH(2800)}, //XA_Fest
	{XA_Dia, XA_LENGTH(600)}, //XA_Cut
	{XA_Dia, XA_LENGTH(10000)}, //XA_Blank
	//END.XA
	{XA_End, XA_LENGTH(800)}, //XA_Blank
	{XA_End, XA_LENGTH(800)}, //XA_Blank
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\WEEK1A.XA;1", //XA_Week1A
	"\\MUSIC\\WEEK1B.XA;1", //XA_Week1B
	"\\MUSIC\\DIA.XA;1", //XA_Dia
	"\\MUSIC\\END.XA;1", //XA_End
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	{"freeky", false}, //XA_Freeky
	{"ludum", false}, //XA_Ludum
	//WEEK1A.XA
	{"wifeforever", true}, //XA_Wifeforever
	{"sky", true},   //XA_Sky
	//WEEK1B.XA
	{"manifest", true}, //XA_Manifest
	{"clucked", true}, //XA_Clucked
	//DIA.XA
	{"shift", false}, //XA_Shift
	{"fest", false}, //XA_Fest
	{"cut", false}, //XA_Cut
	{"blank", false}, //XA_Cut
	
	{"gend", false}, //XA_Cut
	{"bend", false}, //XA_Cut
	
	{NULL, false}
};
