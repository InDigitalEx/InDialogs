#if defined _InDialogs_included
    #endinput
#endif

#define _InDialogs_included

#tryinclude <a_samp>

#if !defined _samp_included
    #error "Include <a_samp> before <InDialogs>
#endif

// Defines

#define DialogResponse:%0()     forward dr_%0(playerid, dialogid, response, listitem, inputtext[]);\
                                public dr_%0(playerid, dialogid, response, listitem, inputtext[])

#define DialogCreate:%0(%1)     forward dc_%0(%1);\
                                public dc_%0(%1)

#define DialogShow:%0(%1)       dc_%0(%1)

#define Dialog:                 #dr_
#define DLG:                    #dr_
#define D:                      #dr_

#define MAX_FUNC_NAME           32 // 31+1

// Magic code
static  dialogID[MAX_PLAYERS] = {-1, ...},
        dialogCallback[MAX_PLAYERS][MAX_FUNC_NAME];


public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogID[playerid] != dialogid)
        return printf("[InDialogs] Differences between ID's (%d != %d)",
            dialogID[playerid], dialogid);

    new function[MAX_FUNC_NAME];
    strcat(function, dialogCallback[playerid]);

    dialogCallback[playerid][0] = EOS;
    dialogID[playerid] = -1;

    return CallLocalFunction(function, "dddds",
        playerid, dialogid, response, listitem, (inputtext[0] == EOS) ? "\1" : inputtext);
}

stock dgtl_ShowPlayerDialog(playerid, const dialog[],
    style, const caption[], const info[], const button1[], const button2[])
{
    strmid(dialogCallback[playerid], dialog, 0, 31);
    dialogID[playerid] = funcidx(dialog);

    if(dialogID[playerid] == -1)
    {
        printf("[InDialogs] Unknown callback: \"%s\" ID: %d",
            dialogCallback[playerid], playerid);
        dialogCallback[playerid][0] = EOS;
        return false;
    }

    return ShowPlayerDialog(playerid, dialogID[playerid], style, caption, info, button1, button2);
}

stock Dialog_ShowByName(playerid, const dialog[])
{
    new function[32] = "dc_";
    strcat(function, dialog);
    return CallLocalFunction(function, "%d", playerid);
}

stock Dialog_Close(playerid)
{
    dialogID[playerid] = -1;
    dialogCallback[playerid][0] = EOS;
    return ShowPlayerDialog(playerid, -1, 0, "", "", "", "");
}

stock Dialog_IsOpened(playerid)
{
    return dialogID[playerid] == -1 ? false : true;
}

stock Dialog_GetID(playerid)
{
    return dialogID[playerid] == -1 ? -1 : dialogID[playerid];
}

// ===== HOOKS =====

// Hook OnDialogResponse
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse dgtl_OnDialogResponse
#if defined dgtl_OnDialogResponse
forward dgtl_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

// Hook ShowPlayerDialog
#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog dgtl_ShowPlayerDialog