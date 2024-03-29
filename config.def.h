/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const int startwithgaps	    = 1;        /* 1 means gaps are used by default */
static const unsigned int gappx     = 10;       /* default gap between windows in pixels */
static const unsigned int snap      = 32;       /* snap pixel */
static const int barheight          = 24;       /* bar height */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows */
static const int startwithbargaps   = 1;        /* 0 means no gaps at start */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const char *fonts[]          = { "Liberation Mono:style=Regular:size=10:antialias=true:autohint=true",
                                        "JoyPixels:style=Regular:size=10:antialias=true:autohint=true",
                                        "Font Awesome 5 Free Regular:style=Regular:size=10:antialias=true:autohint=true",
                                        "Font Awesome 5 Free Solid:style=Solid:size=10:antialias=true:autohint=true",
                                        "Font Awesome 5 Brands Regular:style=Regular:size=10:antialias=true:autohint=true" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#47407D";
static const char col_highlight[]   = "#569fba";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeNormTags] = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_highlight },
	[SchemeHighlight] = { col_highlight, col_gray4, col_gray2 },
};
static const unsigned int baralpha    = 0xDF;
static const unsigned int borderalpha = OPAQUE;
static unsigned int alphas[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeNormTags] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeHighlight]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const Tag tags[] = {
	{ "/usr/share/icons/gde/terminal-icon.xpm", Image },
	{ "/usr/share/icons/gde/brave-logo.xpm", Image },
	{ "/usr/share/icons/gde/discord-logo.xpm", Image },
	{ "/usr/share/icons/gde/steam-logo.xpm", Image },
	{ "", Text },
	{ "/usr/share/icons/gde/gentoo-logo.xpm", Image },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "",        NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* Statusbar program name */
#define STATUSBAR "dwmblocks"

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-l", "15",
                                  "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan,
                                  "-sf", col_gray4, "-nhb", col_gray1, "-nhf", col_highlight,
                                  "-shb", col_cyan, "-shf", col_highlight, "-p", "Run: ", NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier                     key                 function        argument */
	{ MODKEY,                       XK_b,               togglebar,      {0} },
	{ MODKEY,                       XK_j,               focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,               focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,               rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,               rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_i,               incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,               incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,               setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,               setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,           zoom,           {0} },
	{ MODKEY,                       XK_Tab,             view,           {0} },
	{ MODKEY,                       XK_x,               killclient,     {0} },
	{ MODKEY,                       XK_w,               setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,               togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_space,           togglefloating, {0} },
	{ MODKEY,                       XK_0,               view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,               tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,           focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,          focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,           tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,          tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,           setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,           setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,           setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,           setgaps,        {.i = GAP_TOGGLE} },
	{ MODKEY|ControlMask,           XK_minus,           setbargaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_equal,           setbargaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_bracketleft,     setbargaps,     {.i = +11 } },
	{ MODKEY|ControlMask,           XK_bracketright,    setbargaps,     {.i = -11 } },
	{ MODKEY|ControlMask|ShiftMask, XK_minus,           setbargaps,     {.i = GAP_RESET } },
	{ MODKEY|ControlMask|ShiftMask, XK_equal,           setbargaps,     {.i = GAP_TOGGLE} },
	/* HOTKEYS */
	{ MODKEY,                       XK_d,               spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,               spawn,          SHCMD("discord") },
	{ MODKEY|ShiftMask,             XK_f,               spawn,          SHCMD("brave-bin") },
	{ MODKEY,                       XK_p,               spawn,          SHCMD("passmenu") },
	{ MODKEY|ShiftMask,             XK_p,               spawn,          SHCMD("gde-session") },
	{ MODKEY|ShiftMask,             XK_v,               spawn,          SHCMD("pavucontrol") },
	{ MODKEY|ShiftMask,             XK_y,               spawn,          SHCMD("brave-bin youtube.com") },
	{ MODKEY,                       XK_Return,          spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_F1,              spawn,          SHCMD("steam") },
	{ MODKEY|ShiftMask,             XK_F2,              spawn,          SHCMD("brave-bin twitch.tv") },
	{ 0,          XF86XK_AudioRaiseVolume,              spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10%; pkill -RTMIN+4 dwmblocks") },
	{ 0,          XF86XK_AudioLowerVolume,              spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10%; pkill -RTMIN+4 dwmblocks") },
	{ 0,           XF86XK_MonBrightnessUp,              spawn,          SHCMD("xbacklight -inc 10") },
	{ 0,         XF86XK_MonBrightnessDown,              spawn,          SHCMD("xbacklight -dec 10") },
	{ 0,                 XF86XK_AudioPlay,              spawn,          SHCMD("playerctl --all-players play-pause") },
	TAGKEYS(                         XK_1,                  0)
	TAGKEYS(                         XK_2,                  1)
	TAGKEYS(                         XK_3,                  2)
	TAGKEYS(                         XK_4,                  3)
	TAGKEYS(                         XK_5,                  4)
	TAGKEYS(                         XK_6,                  5)
	TAGKEYS(                         XK_7,                  6)
	TAGKEYS(                         XK_8,                  7)
	TAGKEYS(                         XK_9,                  8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

