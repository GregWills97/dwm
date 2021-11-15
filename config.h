/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "UbuntuMono Nerd Font:size=14:antialias=true:autohint=true", 
                                        "FontAwesome:pixelsize=15:antialias=true:autohint=true",
                                        "Font Awesome 5 Free:pixelsize=15:antialias=true:autohint=true",
                                        "Font Awesome 5 Brands:pixelsize=15:antialias=true:autohint=true" };
static const char dmenufont[]       = "UbuntuMono Nerd Font:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#e65e00";    //Orange
//static const char col_cyan[]        = "#0f273b";  //Street
//static const char col_cyan[]        = "#45ADBB";  //purp
//static const char col_cyan[]        = "#F92660";  //pink
static const char col_normtags[]        = "#4685bc";  //purp
//static const char col_cyan[]        = "#B77575";  //Catalina
static const char col_catalina[]    = "#D27C7C";  //Catalina
static const unsigned int baralpha = 0xFF;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeSelInfo]  = { col_gray4, col_cyan,  col_gray2 },
	[SchemeNormTags] = { col_gray4, col_gray1, col_gray2 },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeSelInfo]  = { OPAQUE, baralpha, baralpha  },
	[SchemeNormTags] = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { " ", " ", "", " ", " ", " ", " ", " ", " " };
//static const char *tags[] = { " ", " ", " "," ", " ", " ", " ", " ", " " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ " ",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
 	{ "HHH",      grid },
	{ NULL,       NULL },
};

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          SHCMD("~/.scripts/dmenu_run.sh") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_x,      killclient,     {0} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1} },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("brave-bin") },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("discord") },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("blueman-manager") },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("brave-bin canvas.uncc.edu") },
	{ MODKEY|ShiftMask,             XK_y,      spawn,          SHCMD("brave-bin youtube.com") },
	{ MODKEY|ShiftMask,             XK_F2,     spawn,          SHCMD("brave-bin twitch.tv") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("passmenu") },
	{ MODKEY,                       XK_t,      spawn,          SHCMD("emacs") },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          SHCMD("pavucontrol") },
	{ MODKEY|ShiftMask,             XK_i,      spawn,          SHCMD("st -e ranger") },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("st -e sudo nmtui") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("doas poweroff") },
	{ MODKEY|ShiftMask,             XK_g,      spawn,          SHCMD("lutris") },
	{ MODKEY|ShiftMask,             XK_F1,     spawn,          SHCMD("steam") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("sudo poweroff") },
    { 0,         XF86XK_AudioRaiseVolume,      spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10%") },
    { 0,         XF86XK_AudioLowerVolume,      spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10%") },
    { 0,         XF86XK_AudioMute,             spawn,          SHCMD("amixer set Capture toggle") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

