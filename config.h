/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 2;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 2;        /* vert inner gap between windows */
static const unsigned int gappoh    = 2;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 2;        /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 2 means swallow floating windows by default */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static char dmenufont[]             = "monospace:size=14";

/* NORD Colour scheme */
/* Polar night:   nord0..3 going from near black to a middle grey */
static char nord0[]                 = "#2e3440";
static char nord1[]                 = "#3b4252";
static char nord2[]                 = "#434c5e";
static char nord3[]                 = "#4c566a";
/* Snow Storm nord4..6 going from light grey to almost white */
static char nord4[]                 = "#d8dee9";
static char nord5[]                 = "#e5e9f0";
static char nord6[]                 = "#eceff4";
/* Frost nord7..10   teal / light blue / slightly darker blue / mid blue */
static char nord7[]                 = "#8fbcbb";
static char nord8[]                 = "#88c0d0";
static char nord9[]                 = "#81a1c1";
static char nord10[]                = "#5e81ac";
/* Aurora    nord11..15   red, orange, yellow, green ,purple */
static char nord11[]                = "#bf616a";
static char nord12[]                = "#d08770";
static char nord13[]                = "#ebcb8b";
static char nord14[]                = "#a3be8c";
static char nord15[]                = "#b48ead";

static char black[]                 = "#000000";


enum {
    SchemeNorm,
    SchemeSel,
    SchemeInactive,
    SchemeBlue,
    SchemeRed,
    SchemeOrange,
    SchemeYellow,
    SchemeGreen,
    SchemePurple,
    SchemeWhite,
    SchemeLayout,
    NUM_COLOR_SCHEMES,   // make sure this is never over 32 or it will clash with ASCII space!
}; /* color schemes */
static const char *colors[][3] = {
       /*                   fg      bg      border   */
       [SchemeNorm]     = { nord4,  nord1,  nord2 },
       [SchemeSel]      = { nord0,  nord10, nord12 },
       [SchemeInactive] = { nord3,  nord0,  nord1 },

       [SchemeBlue]     = { nord10, nord0,  nord2 },
       [SchemeRed]      = { nord11, nord0,  nord2 },
       [SchemeOrange]   = { nord12, nord0,  nord2 },
       [SchemeYellow]   = { nord13, nord0,  nord2 },
       [SchemeGreen]    = { nord14, nord0,  nord2 },
       [SchemePurple]   = { nord15, nord0,  nord8 },
       [SchemeWhite]    = { nord5,  nord0,  nord2 },
       //[SchemeLayout]   = { nord8,  nord0,  nord2 },
       [SchemeLayout]   = { nord7,  black, nord9 },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;

const char *sptermcmd[] = {"st", "-n", "spterm", "-g", "140x54", NULL };
const char *sprangercmd[] = {"st", "-n", "spranger", "-g", "140x54", "-e", "ranger", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      sptermcmd},
    {"spranger",    sprangercmd},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
    */
    /* class    instance      title            tags mask    isfloating   isterminal  noswallow  monitor */
    { "Gimp",     NULL,       NULL,               1 << 8,       0,           0,         0,        -1 },
    { "St",       NULL,       NULL,               0,            0,           1,         0,        -1 },
    { NULL,       NULL,       "Event Tester",     0,            0,           0,         1,        -1 },
    { NULL,      "spterm",    NULL,               SPTAG(0),     1,           1,         0,        -1 },
    { NULL,      "spranger",  NULL,               SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */

#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol   arrange function */
    { "[]=",    tile },                     /* Default: Master on left, slaves on right */

    { "[]D",    deck },                     /* Master on left, slaves in monocle-like mode on right */
    { "[M]",    monocle },                  /* All windows on top of eachother */

    { "><>",    NULL },                     /* no layout function means floating behavior */
    { NULL,     NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
/* these are standard keys for selecting/moving to tags */
#define TAGKEYS(KEY, TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
/* alt tags handles the case where shift key is different in the keymap i.e. 2 on mine */
#define TAGKEYSALT(KEY, SHIFTKEY, TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             SHIFTKEY, tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
    { MOD,    XK_j,    ACTION##stack,    {.i = INC(+1) } }, \
    { MOD,    XK_k,    ACTION##stack,    {.i = INC(-1) } }, \
    { MOD,    XK_v,    ACTION##stack,    {.i = 0 } }, \
    /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
    /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
    /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
    /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", nord0, "-nf", nord4, "-sb", nord10, "-sf", nord6, NULL };
static const char *roficmd[] = { "rofi", "-show", "combi" };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
    /* modifier                     key        function        argument */

    /* selecting windows */
    /* stack keys sets jkv to move down/up/to start of stack */
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)

    /* move the selected window to the master */
    { MODKEY,           XK_space,      zoom,            {0} },
    
    /* selecting tags */

    /* Number keys move to tag, shift-# moves screen to tag,
     * control-# allows to select multiple tags, control-shift-# unselects a tag */
    TAGKEYS(            XK_1,                   0)
    TAGKEYSALT(         XK_2, XK_apostrophe,    1)   /* handle the shift-2 being apostrophe */
    TAGKEYS(            XK_3,                   2)
    TAGKEYS(            XK_4,                   3)
    TAGKEYS(            XK_5,                   4)
    TAGKEYS(            XK_6,                   5)
    TAGKEYS(            XK_7,                   6)
    TAGKEYS(            XK_8,                   7)
    TAGKEYS(            XK_9,                   8)

    /* ; to go to the next populated tag and comma to go back*/
    { MODKEY,           XK_semicolon,   shiftview,      { .i = 1 } },
    { MODKEY|ShiftMask, XK_semicolon,   shifttag,       { .i = 1 } },
    { MODKEY,           XK_comma,       shiftview,      { .i = -1 } },
    { MODKEY|ShiftMask, XK_comma,       shifttag,       { .i = -1 } },

    /* the number 0 key or tab selects all tags */
    { MODKEY,           XK_0,           view,           {.ui = ~0 } },
    { MODKEY|ShiftMask, XK_0,           tag,            {.ui = ~0 } },
    { MODKEY,           XK_Tab,         view,           {0} },
    /* a puts the selected window on every tag */
    { MODKEY,           XK_a,           togglesticky,   {0} },

    /* restart */
    { MODKEY|ShiftMask, XK_BackSpace,   quit,           {.i = 1} },
    /* exit window/dwm */
    { MODKEY,           XK_q,           killclient,     {0} },
    { MODKEY|ShiftMask, XK_q,           quit,           {.i = 0} },

    /* launchers */
    { ControlMask,      XK_space,       spawn,          {.v = roficmd } },
    { MODKEY,           XK_d,           spawn,          {.v = dmenucmd } },
    { MODKEY,           XK_w,           spawn,          SHCMD("$BROWSER") },
    { MODKEY,           XK_apostrophe,  togglescratch,  {.ui = 0} },  /* start a st shell scratch */
    { MODKEY,           XK_r,           togglescratch,  {.ui = 1 } }, /* start a scratch ranger */
    { MODKEY,           XK_Return,      spawn,          {.v = termcmd } },

    /* layouts */
    { MODKEY,           XK_u,           setlayout,      {.v = &layouts[0]} }, /* tile */
    { MODKEY,           XK_i,           setlayout,      {.v = &layouts[1]} }, /* deck */
    { MODKEY,           XK_o,           setlayout,      {.v = &layouts[2]} }, /* monicle */
    { MODKEY,           XK_p,           setlayout,      {.v = &layouts[3]} }, /* floating */
    { MODKEY,           XK_f,           togglefullscr,  {0} },
    { MODKEY|ShiftMask, XK_f,           togglefloating, {0} },
    { MODKEY,           XK_m,           incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask, XK_m,           incnmaster,     {.i = -1 } },

    /* gaps */
    { MODKEY,           XK_g,           togglegaps,     {0} },
    { MODKEY|ShiftMask, XK_g,           defaultgaps,    {0} },
    { MODKEY,           XK_z,           incrgaps,       {.i = +3 } },
    { MODKEY,           XK_x,           incrgaps,       {.i = -3 } },

    /* resize the master */
    { MODKEY,           XK_h,           setmfact,       {.f = -0.05} },
    { MODKEY,           XK_l,           setmfact,       {.f = +0.05} },

    /* toggle the bar at the top */
    { MODKEY,           XK_b,           togglebar,      {0} },

    /*  monitor keys  - only have 2 so rotate in one direction */
    { MODKEY,           XK_s,       focusmon,           {.i = -1 } },
    { MODKEY|ShiftMask, XK_s,       tagmon,             {.i = -1 } },

    { MODKEY,           XK_Insert,     spawn,           SHCMD("notify-send \"📋 Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },


};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
  /*{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
    { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD("st -e nvim ~/.local/src/dwmblocks/config.h") }, */
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
    { ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
    { ClkTagBar,            0,              Button5,        shiftview,      {.i = 1} },
/*    { ClkLtSymbol,          0,              Button1,        cyclelayout,    {.i = 1} }, */
    { ClkRootWin,           0,              Button2,        togglebar,      {0} },
};
