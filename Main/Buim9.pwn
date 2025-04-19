#include <a_samp>
#include <scripts/config>
#include <scripts/account>
#include <scripts/timer>
#include <scripts/tele>
#include <scripts/admin>
#include <scripts/bank>
#include <scripts/temporary_vehicle>
#include <scripts/friend>
#include <scripts/team>
#include <scripts/prace>
#include <scripts/phouse>
#include <scripts/wuqi>
#include <scripts/dm>
#include <scripts/ucp>
#include <scripts/acveh>
#include <scripts/acobj>
#include <scripts/billboard>
#include <scripts/cmds>
#include <scripts/music>
#include <scripts/recycling>
#include <scripts/redpacket>
#include <scripts/ppc>
#include <scripts/attire>
#include <scripts/subtitles>
#include <scripts/nex-ac>

main() {
	print("\n----------------------------------");
	print(" 新狂野 Powered by Dylan");
	print("----------------------------------\n");
}

public OnGameModeInit() {
	configInit();
	adminInit();
	timerInit();
	teleInit();
	PRaceInit();
	PHouseInit();
	bankInit();
	UCPInit();
	ACVehInit();
	BillBoard_Init();
	return 1;
}

public OnGameModeExit() {
	configExit();
	timerExit();
	PRaceExit();
	PHouseExit();
	return 1;
}

public OnPlayerRequestClass(playerid, classid) {
	SetSpawnInfo(playerid, 0, 0,  -85.32656, -222.87402, 41.16307, 0.0, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	SetPlayerCameraPos(playerid, -82.9859, -227.0373, 80.0687);
	SetPlayerCameraLookAt(playerid, 100.6493, -570.7420, 92.0905);
	showConnMsg(playerid);
	accountLogin(playerid);
	return 1;
}

public OnPlayerConnect(playerid) {
	configConnect(playerid);
	accountConnect(playerid);
	temporary_vehicleInit(playerid);
	//FRIEND_Connect(playerid);
	PRaceConnect(playerid);
	new msg[128];
	format(msg, sizeof msg, "[系统] 欢迎玩家 %s(%d) 进入新狂野之城&&", GetName(playerid), playerid);
	SCMToAll(COLOR_WHITE, msg);
	UCPSendChat(msg);
	teamInit(playerid);
	RedPacketConnect(playerid);
	vspawner_OnPlayerConnect(playerid);
	PPCPlayerInit(playerid);
	if(IsServRestarting() == 1) {
		KickEx(playerid, "服务器正在重启中");
	}
	return 1;
}

public OnPlayerDisconnect(playerid, reason) {
	accountDisconnect(playerid);
	temporary_vehicleInit(playerid);
	PRaceDisconnect(playerid);
	ACOBJViewInit(playerid);
	RedPacketDisconnect(playerid);
	PPCPlayerInit(playerid);
	new szString[144];
	new szDisconnectReason[2][] = {
        "掉线了.",
        "离开了服务器."
    };
	format(szString, sizeof szString, "[系统] 玩家 %s %s", GetName(playerid), (reason == 2 ? (pReason[playerid]) : (szDisconnectReason[reason])));
    SCMToAll(COLOR_WHITE, szString);
	UCPSendChat(szString);
	return 1;
}

public OnPlayerSpawn(playerid) {
	if(GetPlayerDMStatus(playerid) == 0) {
		accountSpawn(playerid);
	} else {
		DMSetPAW(playerid);
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {
    SendDeathMessage(killerid, playerid, reason);
	DMDKA(playerid, killerid);
	return 1;
}

public OnVehicleSpawn(vehicleid) {
    ACRespawn(vehicleid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid) {
	ACOBJAttachInit(vehicleid);
	return 1;
}

public OnPlayerText(playerid, text[]) {
	accountOPT(playerid, text);
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid) {
	PPC_OnPlayerExitVehicle(playerid);
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate) {
	accountOPSC(playerid, newstate);
	temporary_vehicleOPSC(playerid, newstate);
	ACOPSC(playerid, newstate);
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid) {
	PRaceOPERC(playerid);
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnDynamicObjectMoved(objectid) {
	PPCObjectMoved(objectid);
	MoveMoveObjed(objectid);
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid) {
	ACVEHOVM(playerid, vehicleid, componentid);
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid) {
    ACVEHOEEMS(playerid, enterexit);
    return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2) {
	ACVEHOVR(playerid, vehicleid, color1, color2);
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(configOPKSC(playerid, newkeys) == 1) return 1;
	if(bankOPKSC(playerid, newkeys) == 1) return 1;
	if(ACOBJOPKSC(playerid, newkeys) == 1) return 1;
	PHouseOPKSC(playerid, newkeys);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid) {
    userInfo[playerid][ESC] = 0;
    new keys,updown,leftright;
	GetPlayerKeys(playerid,keys,updown,leftright);
	if(keys & KEY_ACTION || keys & KEY_FIRE) {
		if(GetPlayerVirtualWorld(playerid) == 0) {
			if(IsPlayerInAnyVehicle(playerid)) {
				new vehModelId = GetVehicleModel(GetPlayerVehicleID(playerid));
	 			if(vehModelId == 520 || vehModelId == 432 || vehModelId == 425) {
	 				return 0;
	   			}
	   		} else {
				new weaponId = GetPlayerWeapon(playerid);
				if(weaponId == 35 || weaponId == 36) {
	        		return 0;
	      		}
	        }
	 	}
	}
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if(!strlen(inputtext)) {
		format(inputtext, 12, "%s", defaultIT);
	}
	if(dialogid >= DIALOG_ACCOUNT && dialogid < DIALOG_ACCOUNT + 100) {
		accountODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_BANK && dialogid < DIALOG_BANK + 100) {
		bankODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_TELE && dialogid < DIALOG_TELE + 100) {
		teleODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= FRIEND_DIALOG_ID && dialogid < FRIEND_DIALOG_ID + 100) {
		friendODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_TEAM && dialogid < DIALOG_TEAM + 100) {
		teamODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_PRACE && dialogid < DIALOG_PRACE + 100) {
		PRaceODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_PHOUSE && dialogid < DIALOG_PHOUSE + 100) {
		PHouseODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_WQSD_MAIN && dialogid < DIALOG_WQSD_MAIN + 100) {
		wuqiODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_AC && dialogid < DIALOG_AC + 100) {
		ACODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_BILLBOARD && dialogid < DIALOG_BILLBOARD + 100) {
		BillBoardODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_CMDS && dialogid < DIALOG_CMDS + 100) {
		CMDSODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
    if(dialogid >= DIALOG_ATTIRE && dialogid < DIALOG_ATTIRE + 100) {
		ATTIREODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	if(dialogid >= DIALOG_REDP && dialogid < DIALOG_REDP + 100) {
		REDPCODR(playerid, dialogid, response, listitem, inputtext);
		return 1;
	}
	return 0;
}

public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ) {
	AttireEAO(playerid, response, index, /*modelid, boneid,*/ fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source) {
	new msg[4];
	format(msg, sizeof msg, "%d", clickedplayerid);
	pc_cmd_zhxx(playerid, msg);
	return 1;
}


public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) {
	ACOBJOPEDO(playerid, response, x, y, z, rx, ry, rz);
	BillBoardOPEDO(playerid, response, x, y, z, rx, ry, rz);
    return 1;
}
