/* See LICENSE file for copyright and license details. */

static const unsigned int gappx     = 12;       /* gaps between windows */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 0;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "JetBrainsMono Nerd Font:size=10";
static char dmenufont[]       = "monospace:size=10";
static const char *fonts[]          = { font };
static unsigned int baralpha        = 0x99;
static unsigned int borderalpha     = OPAQUE;

static char normbgcolor[]           = "#080b11";
static char normbordercolor[]       = "#1d2155";
static char normfgcolor[]           = "#a3b3d0";
static char selfgcolor[]            = "#dde3ed";
static char selbordercolor[]        = "#82aaff";
static char selbgcolor[]            = "#080b11";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance    title       tags mask     isfloating   iscentered   monitor */
	{ "Gimp",      NULL,       NULL,       0,            1,           0,           -1 },
	{ "Firefox",   NULL,       NULL,       1 << 8,       0,           0,           -1 },
	{ "CalPopup",  NULL,       NULL,       0,            1,           1,           -1 },
	
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

static const char *upvol[]   = { "/bin/sh", "-c", "pamixer -i 5 && notify-send -a 'System' -r 999 -h int:value:$(pamixer --get-volume) -i 'audio-volume-high' 'Volume Up'", NULL };
static const char *downvol[] = { "/bin/sh", "-c", "pamixer -d 5 && notify-send -a 'System' -r 999 -h int:value:$(pamixer --get-volume) -i 'audio-volume-low' 'Volume Down'", NULL };
static const char *mutevol[] = { "/bin/sh", "-c", "pamixer -t && notify-send -a 'System' -i 'audio-volume-muted' 'Mute Toggled'", NULL };
static const char *brightnessup[]   = { "/bin/sh", "-c", "brightnessctl set +5% && notify-send -a 'System' -r 998 -h int:value:$(brightnessctl i | grep -oP '\\(\\K[^%]+') -i 'display-brightness' 'Brightness Up'", NULL };
static const char *brightnessdown[] = { "/bin/sh", "-c", "brightnessctl set 5%- && notify-send -a 'System' -r 998 -h int:value:$(brightnessctl i | grep -oP '\\(\\K[^%]+') -i 'display-brightness' 'Brightness Down'", NULL };
static const char *displaysetup[] = { "display-setup", NULL };

/* key definitions */
#define MODKEY Mod1Mask
#include <X11/XF86keysym.h>
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static const int dmenudesktop = 1; /* 1 means dmenu will use only desktop files from [/usr/share/applications/] */

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { 
    "dmenu_run", 
    "-c",
    "-l", "10", 
    "-m", dmenumon, 
    "-fn", dmenufont, 
    "-nb", normbgcolor, 
    "-nf", normfgcolor, 
    "-sb", selbordercolor, 
    "-sf", selfgcolor, 
    NULL 
};
static const char *dmenucmddesktop[] = { 
    "dmenu_run_desktop", 
    "-c",
    "-l", "10", 
    "-m", dmenumon, 
    "-fn", dmenufont, 
    "-nb", normbgcolor, 
    "-nf", normfgcolor, 
    "-sb", selbordercolor, 
    "-sf", selfgcolor, 
    NULL 
};
static const char *termcmd[]  = { "gnome-terminal", NULL };
static const char *sysmenucmd[] = { "dwm-menu", NULL };
static const char *appmanager[] = { "app_manager", NULL };
static const char *layout_toggle[] = { "layout_toggle", NULL };
static const char *lockcmd[] = { "lock", NULL };
static const char *explorer[] = { "nsxiv", "-t", "~/Pictures", NULL };
static const char *screenshot_full[]   = { "screenshot", "full",   NULL };
static const char *screenshot_area[]   = { "screenshot", "area",   NULL };
static const char *screenshot_window[] = { "screenshot", "window", NULL };
static const char *calendar[] = { "gnome-terminal", "--class=CalPopup", "--geometry=22x12", "--", "mycal", NULL };
/* Commands for Dunst control */
static const char *dunstclose[]     = { "dunstctl", "close",     NULL };
static const char *dunstcloseall[]  = { "dunstctl", "close-all", NULL };
static const char *dunsthistory[]   = { "dunstctl", "history-pop", NULL };
static const char *dunstcontext[]   = { "dunstctl", "context",   NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_p,      	spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_p,      	spawn,          {.v = dmenucmddesktop } },
	{ MODKEY|ShiftMask,             XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      	togglebar,      {0} },
	{ MODKEY,                       XK_j,      	focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      	focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      	incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, 	zoom,           {0} },
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      	killclient,     {0} },
	{ MODKEY,                       XK_t,      	setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      	setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      	setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },
	{ MODKEY,                       XK_0,      	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  	focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 	focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  	tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 	tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  	setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  	setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  	setgaps,		{.i = 0  } },
	{ MODKEY,                       XK_F1,      spawn,          {.v = calendar } },
	{ MODKEY,                       XK_s,		spawn,			{.v = sysmenucmd } },
	{ MODKEY, 						XK_x, 		spawn,			{.v = appmanager } },
	{ Mod4Mask, 					XK_space, 	spawn,			{.v = layout_toggle } },
	{ Mod4Mask,						XK_l,		spawn,			{.v = lockcmd } },
	{ ControlMask,                  XK_space,	spawn,			{.v = dunstclose } },
    { ControlMask|ShiftMask,        XK_space,	spawn,			{.v = dunstcloseall } },
    { ControlMask,                  XK_grave,	spawn,			{.v = dunsthistory } },
    { ControlMask|ShiftMask,        XK_period,	spawn,			{.v = dunstcontext } },
	{ Mod4Mask|ShiftMask,           XK_p,       spawn,          {.v = explorer } },
    { Mod4Mask,                     XK_m,       spawn,          {.v = displaysetup } },
    { 0,							XK_Print,	spawn,			{.v = screenshot_full }   },
    { Mod4Mask,						XK_s,		spawn,			{.v = screenshot_area }   },
    { Mod4Mask|ShiftMask,			XK_s,		spawn,			{.v = screenshot_window } },
	{ 0, 			XF86XK_AudioLowerVolume,  	spawn,			{.v = downvol } },
    { 0, 			XF86XK_AudioMute,         	spawn,			{.v = mutevol } },
    { 0, 			XF86XK_AudioRaiseVolume,  	spawn,			{.v = upvol   } },
    { 0, 			XF86XK_MonBrightnessUp,   	spawn,			{.v = brightnessup   } },
    { 0, 			XF86XK_MonBrightnessDown, 	spawn,			{.v = brightnessdown } },
	TAGKEYS(                        XK_1,                      	0)
	TAGKEYS(                        XK_2,                      	1)
	TAGKEYS(                        XK_3,                      	2)
	TAGKEYS(                        XK_4,                      	3)
	TAGKEYS(                        XK_5,                      	4)
	TAGKEYS(                        XK_6,                      	5)
	{ MODKEY|ShiftMask,             XK_q,		quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,		quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

