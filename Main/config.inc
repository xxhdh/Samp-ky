#include <a_mysql>
#include <ASAN>
#include <Pawn.CMD>
#include <streamer>
#include <sscanf2>
#define MAX_CONNMSG                 (10)
#define MAX_CONNMSG_LEN             (144)
#define MAX_SERVNAME_LEN            (128)
#define SCM                         SendClientMessage
#define SCMToAll                    SendClientMessageToAll
#define COLOR_WHITE                 (0xEFEFF7FF)
#define COLOR_GREEN                 (0x4DF752FF)
#define COLOR_BLUE                  (0x3FB7ECFF)
#define COLOR_RED                   (0xFF0000FF)
#define COLOR_YELLOW                (0xF9FF2DFF)
#define COLOR_ORANGE                (0xFEA979FF)
#define defaultIT                   "sbkyc1ty"
#define MAX_REASON_LEN              (88)
#define MAX_PLAYER_IP_LEN           (16)
#define MAX_PSPS                    (20)
#define MAX_LENOF_SPNAME            (30)
#define MAX_ANNOUNCEMENTS           (30)
#define DEFAULT_ANNOUNCEMENT_SEC    (600)
#define SERV_MODE_PRODUCTION		(0)
#define SERV_MODE_DEVELOPMENT		(1)
#define Attire.                     Attire_

new announcement[MAX_ANNOUNCEMENTS][MAX_CONNMSG_LEN];
new announcement_sec[2];//0 = 多少秒发送一次消息, 1 = 还差多少秒发送
new announcement_count[2];// 0 = 有多少条消息, 1 = 已发送了多少条消息

new serverRules[2048] = "\
1.请勿在服务器内公屏进行辱骂、挑衅行为\n\
2.请勿R人，禁止使用影响其他玩家游戏的CLEO和工具（如吸车等）\n\
3.集体活动时，请勿捣乱，听从管理员指挥\n\
4.禁止在服务器内进行其他服务器的宣传（包括但不限于私聊发送广告）\n\
5.当其他玩家明确表示厌烦你，请勿继续打扰其他玩家游戏\n\
6.sf机场为人多区域，请勿进行轰炸\n\
7.服务器内禁止使用外挂（包括但不限于喷车、吸车、花屏、BOT多开等外挂软件）\n\
8.海盗船为挂机区域，请勿在海盗船内刷车、杀人、轰炸等\n\
";

new MySQL:mariadb;
new connMsg[MAX_CONNMSG][MAX_CONNMSG_LEN];
new pReason[MAX_PLAYERS][MAX_REASON_LEN];
new pNitro[MAX_PLAYERS];
new dyServMode;

enum configservtype {
    Name[MAX_SERVNAME_LEN],
    GMT[MAX_SERVNAME_LEN]
};

new configServ[configservtype];

stock GetServMode() {
	return dyServMode;
}

new GetWeatherNames[][MAX_PLAYER_NAME] = {
    "LA极晴",
    "LA晴",
    "LA极晴雾",
    "LA晴雾",
    "LA阴",
    "SF晴",
    "SF极晴",
    "SF阴",
    "SF雨",
    "SF雾",
    "LV晴",
    "LV热晴",
    "LV阴",
    "乡村极晴",
    "乡村晴",
    "乡村阴",
    "乡村雨",
    "沙漠极晴",
    "沙漠晴",
    "沙漠沙尘暴",
    "水下(绿,雾)"
};

enum spawnposstruct {
    Name[MAX_LENOF_SPNAME],
    Float:X,
    Float:Y,
    Float:Z,
    Float:A
};

new pSPos[MAX_PSPS][spawnposstruct], count_spawnpos;

new PlayerColors[] = {
    0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
	0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0x00FF00FF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF
};

enum configQAtype {
    Timer,
    Now,
    Solution[128]
};

new QAs[configQAtype];

stock configRuntime_QAs() {
    QAs[Now] ++;
    if(QAs[Now] >= QAs[Timer]) {
        QAs[Now] = 0;
        new Cache:result, msg[144], id, q[128];
        result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword='qa'");
		cache_get_row_count(id);
        if(id != 0) {
            id = random(id);
            cache_get_value_name(id, "value", msg, sizeof msg);
            cache_delete(result);
            sscanf(msg, "s[128] s[64]", q, QAs[Solution]);
            format(msg, sizeof msg, "[问答] %s", q);
            SCMToAll(COLOR_GREEN, msg); 
        }
    }
}

stock configQAIsSolution(text[]) {
    if(mk_strcmp(text, QAs[Solution]) == 0 && strlen(QAs[Solution]) != 0 ) {
        format(QAs[Solution], 2, "");
        return 1;
    }
    return 0;
}

stock configKGOBJ(playerid) {
	Streamer_UpdateEx(playerid, 0.0, 0.0, -50000.0);
    Streamer_ToggleItemUpdate(playerid, 0, GetPlayerOBJShowStatus(playerid));
}


stock configConnect(playerid) {
    pNitro[playerid] = 0;
    if(isNameColor(GetName(playerid)) == 1) {
        SCM(playerid, COLOR_RED, "[系统] 你被踢出了服务器, 原因:名字不能带有特殊字符.");
        KickEx(playerid, "错误的名字格式");
    }
}

CMD:rule(playerid, params[]) {
    DIALOG_node(playerid, serverRules, "服务器规则");
    return 1;
}

stock configOPKSC(playerid, newkeys) {
    if(newkeys == 4 || newkeys == 12) {
        if(GetPlayerVehicleSeat(playerid) == 0 && gettime() - pNitro[playerid] > 0) {
            pNitro[playerid] = gettime() + 10;
            AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
            return 1;
        }
    }
    return 0;
}

stock DIALOG_node(playerid, string[], title[] = "提示") {
    ShowPlayerDialog(playerid, 1, DIALOG_STYLE_MSGBOX, title, string, "关闭", "");
    return 1;
}

public OnPlayerCommandReceived(playerid, cmd[], params[], flags) {
    log_command(GetName(playerid), GetIp(playerid), cmd, params, GetPlayerUID(playerid));
    if(GetPlayerStatus(playerid) == 0) {
        SCM(playerid, COLOR_WHITE, "[系统] 你当前不可以输入任何命令, 原因:你未登录.");
        return 0;
    }
    if(isPlayerInPPC(playerid) == 1 && strcmp(cmd, "ppc") != 0) {
        SCM(playerid, COLOR_RED, "[系统] 在碰碰车中不可以使用其他命令.请先输入/ppc exit 退出碰碰车");
        return 0;
    }
    if(GetPlayerDMStatus(playerid) == 1 && strcmp(cmd, "dm") != 0) {
        SCM(playerid, COLOR_WHITE, "[系统] 在DM中不可以使用其他命令.");
        return 0;
    }
    if(IsPlayerInJail(playerid) == 1) {
        SCM(playerid, COLOR_WHITE, "[系统] 监狱中不能使用任何命令.");
        return 0;
    }
    return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags) {
    if(result == -1) {
        new telecmd[128];
        format(telecmd, sizeof telecmd, cmd);
        if(telecmd[0] == '/' && telecmd[1]) {
            tele2Slash(playerid, telecmd);
            return 0;
        }
        if(teleSlash(playerid, telecmd) == 1) return 0;
        new msg[128];
        format(msg, sizeof msg, "[系统] 你输入的命令'/%s'不存在, 你可以输入 /help 查看帮助或关注微信公众号:samp狂野之城 寻求帮助", cmd);
        SCM(playerid, COLOR_WHITE, msg);
        return 0;
    }
    return 1;
}

configInit() {
    SetRecyclingStatus(0);
    QAs[Timer] = 1200;
    QAs[Now] = 0;
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL)
    SetServName(configServ[Name]);
	result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword='servmode'");
	cache_get_value_name(0, "value", value, sizeof value);
	dyServMode = (strcmp(value, "development") == 0 ? (1) : (0));
	printf("servmode -> %s", value);
    result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword='game_mode_text'");
    cache_get_value_name(0, "value", configServ[GMT], MAX_SERVNAME_LEN);
    if(!strlen(configServ[GMT])) {
        format(configServ[GMT], MAX_SERVNAME_LEN, "none");
    }
    SetGameModeText(configServ[GMT]);
    result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword='announcement_sec'");
    cache_get_value_name(0, "value", value);
    if(strval(value) == 0) {
        announcement_sec[0] = DEFAULT_ANNOUNCEMENT_SEC;
    } else {
        announcement_sec[0] = strval(value);
    }
    announcement_count[1] = 0;
    printf("config -> 定时发送消息时间为%d秒", announcement_sec[0]);
    result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword='announcement'");
    cache_get_row_count(announcement_count[0]);
    for(new i = 0; i < announcement_count[0]; i ++) {
        cache_get_value_name(i, "value", announcement[i], MAX_CONNMSG_LEN);
    }
    printf("config -> 已加载%d条定时发送消息", announcement_count[0]);
    result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword = 'connmsg'");
    cache_get_row_count(connmsgs);
    if(connmsgs >= MAX_CONNMSG) {
        printErr("ERR:欢迎消息已达到上限(config->MAX_CONNMSG)");
    }
    for(new i = 0; i < connmsgs; i ++) {
        cache_get_value_name(i, "value", connMsg[i]);
    }
    printf("config -> 已加载%d条欢迎消息", connmsgs);
    cache_delete(result);
    result = mysql_query(mariadb, "SELECT value FROM config WHERE keyword = 'spawnpos'");
    cache_get_row_count(count_spawnpos);
    if(count_spawnpos >= MAX_PSPS) {
        printErr("ERR:系统出生点已达到上限(config->MAX_PSPS)");
    }
    for(new i = 0; i < count_spawnpos; i ++) {
        cache_get_value_name(i, "value", value);
        pSPos[i][X] = floatstr(strtok(value, idx));
        pSPos[i][Y] = floatstr(strtok(value, idx));
        pSPos[i][Z] = floatstr(strtok(value, idx));
        pSPos[i][A] = floatstr(strtok(value, idx));
        format(pSPos[i][Name], MAX_LENOF_SPNAME, strrest(value, idx));
        idx = 0;
    }
    printf("config -> 已加载%d个系统出生点", count_spawnpos);
    cache_delete(result);
    UsePlayerPedAnims();
}

stock SetServName(name[]) {
    new msg[144];
    format(msg, sizeof(msg), "hostname %s", name);
    SendRconCommand(msg);
}

stock SetServPassword(password[]) {
    new msg[144];
    format(msg, sizeof(msg), "password %s", (strcmp(password, "none") == 0 ? (" ") : (password)));
    SendRconCommand(msg);
}

stock configPSPRandom(playerid) {
    new r = random(count_spawnpos), msg[128];
    SetPlayerPos(playerid, pSPos[r][X], pSPos[r][Y], pSPos[r][Z]);
    SetPlayerFacingAngle(playerid, pSPos[r][A]);
    format(msg, sizeof msg, "[系统] 出生在 >> %s", pSPos[r][Name]);
    SCM(playerid, COLOR_WHITE, msg);
}

configExit() {
    mysql_close(mariadb);
}

showConnMsg(playerid) {
    for(new i = 0; i < MAX_CONNMSG; i ++) {
        if(strlen(connMsg[i])) {
            SCM(playerid, COLOR_WHITE, connMsg[i]);
        }
    }
}

printErr(str[]) {
    printf(str);
    SendRconCommand("exit");
}

stock isNameHasTeamName(name[]) {
    new tname[MAX_PLAYER_NAME], brackets[2];
    brackets[0] = -1, brackets[1] = -1;
    format(tname, sizeof tname, name);
    for(new i = 0; i < strlen(tname); i ++) {
        if(tname[i] == '[') {
            if(i > brackets[0] && brackets[0] != -1) {
                brackets[0] = -1;
                break;
            }
            brackets[0] = i;
        }
        if(tname[i] == ']') {
            if(i > brackets[1] && brackets[1] != -1) {
                brackets[1] = -1;
                break;
            }
            brackets[1] = i;
        }
    }
    if(brackets[0] != -1 && brackets[1] != -1) {
        strdel(tname, brackets[1], strlen(tname));
        strdel(tname, 0, brackets[0]+1);
    } else {
        format(tname, 2, "");
    }
    return tname;
}

stock ChangeSpecialChar(str[]) {
	/* \ ' */
	new msg[144];
	for(new i = 0; i < strlen(str); i ++) {
		if(str[i] == 92 || str[i] == 39) {
			str[i] = 32;
		}
	}
	format(msg, sizeof msg, str);
	return msg;
}

stock log_ban(from_name[], from_ip[], from_uid, to_name[], to_ip[], to_uid, time, reason[]) {
    new msg[512];
    format(msg, sizeof msg, "INSERT INTO log_ban (fname,fip,fuid,tname,tip,tuid,time,reason,date) VALUES ('%s','%s','%d','%s','%s','%d','%d','%s','%d')", 
    from_name, from_ip, from_uid, to_name, to_ip, to_uid, time, reason, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_mute(from_name[], from_ip[], from_uid, to_name[], to_ip[], to_uid, time, reason[]) {
    new msg[512];
    format(msg, sizeof msg, "INSERT INTO log_mute (fname,fip,fuid,tname,tip,tuid,time,reason,date) VALUES ('%s','%s','%d','%s','%s','%d','%d','%s','%d')", 
    from_name, from_ip, from_uid, to_name, to_ip, to_uid, time, reason, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_jail(from_name[], from_ip[], from_uid, to_name[], to_ip[], to_uid, time, reason[]) {
    new msg[512];
    format(msg, sizeof msg, "INSERT INTO log_jail (fname,fip,fuid,tname,tip,tuid,time,reason,date) VALUES ('%s','%s','%d','%s','%s','%d','%d','%s','%d')", 
    from_name, from_ip, from_uid, to_name, to_ip, to_uid, time, reason, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_kick(from_name[], from_ip[], from_uid, to_name[], to_ip[], to_uid, reason[]) {
    new msg[512];
    format(msg, sizeof msg, "INSERT INTO log_kick (fname,fip,fuid,tname,tip,tuid,reason,date) VALUES ('%s','%s','%d','%s','%s','%d','%s','%d')", 
    from_name, from_ip, from_uid, to_name, to_ip, to_uid, reason, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_changepasswrd(uid, name[], ip[]) {
    new msg[256];
    format(msg, sizeof msg, "INSERT INTO log_changepwd (uid,name,ip,date) VALUES ('%d','%s','%s','%d')", 
    uid, name, ip, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_changename(name[], toname[], ip[], uid) {
    new msg[256];
    format(msg, sizeof msg, "INSERT INTO log_changename (name,toname,ip,uid,date) VALUES ('%s','%s','%s','%d','%d')", 
    name, toname, ip, uid, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_command(name[], ip[], command[], params[], uid) {
    new msg[384];
    format(msg, sizeof msg, "INSERT INTO log_command (name,ip,command,params,uid,date) VALUES ('%s','%s','%s','%s','%d','%d')", 
    name, ip, ChangeSpecialChar(command), ChangeSpecialChar(params), uid, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_chat(name[], ip[], chat[], uid) {
    new msg[384];
    format(msg, sizeof msg, "INSERT INTO log_chat (name,ip,chat,uid,date) VALUES ('%s','%s','%s','%d','%d')", 
    name, ip, ChangeSpecialChar(chat), uid, gettime());
    mysql_query(mariadb, msg, false);
}

stock log_money(from_name[], from_ip[], from_uid, to_name[], to_ip[], to_uid, money, reason[]) {
    new msg[512];
    format(msg, sizeof msg, "INSERT INTO log_money (fname,fip,fuid,tname,tip,tuid,money,reason,date) VALUES ('%s','%s','%d','%s','%s','%d','%d','%s','%d')", 
    from_name, from_ip, from_uid, to_name, to_ip, to_uid, money, reason, gettime());
    mysql_query(mariadb, msg, false);
}

stock isNameOnline(name[]) {
    new pid = -1;
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
        if(IsPlayerConnected(i) == 1) {
            if(strcmp(GetName(i), name) == 0) {
                pid = i;
                break;
            }
        }
    }
    return pid;
}

stock GetMaxPage(counts, pagemax = 20) {
    new maxpage = counts % pagemax;
    if(maxpage == 0) {
        maxpage = counts / pagemax;
    } else {
        maxpage = (counts / pagemax) + 1;
    }
    return maxpage;
}

stock GetIp(playerid) {
    new ip[16];
    GetPlayerIp(playerid, ip, sizeof ip);
    return ip;
}

stock GetName(playerid) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof name);
    return name;
}

BanExx(playerid, reason[], delay = 250) {
    format(pReason[playerid], MAX_REASON_LEN, "被踢出了服务器, 原因:%s.", reason);
    SetTimerEx("BanDelay", delay, false, "i", playerid);
}

forward BanDelay(playerid);
public BanDelay(playerid) {
    Ban(playerid);
    return 1;
}

KickEx(playerid, reason[], delay = 250) {
    format(pReason[playerid], MAX_REASON_LEN, "被踢出了服务器, 原因:%s.", reason);
    SetTimerEx("KickDelay", delay, false, "i", playerid);
}

forward KickDelay(playerid);
public KickDelay(playerid) {
    Kick(playerid);
    return 1;
}

Float:GetPlayerAngle(playerid) {
    new Float:angle;
    if(GetPlayerVehicleID(playerid) == 0) {
        GetPlayerFacingAngle(playerid, angle);
    } else {
        GetVehicleZAngle(GetPlayerVehicleID(playerid), angle);
    }
    return angle;
}

stock SetPlayerPosEx(playerid, Float:x, Float:y, Float:z, Float:a = 0.0) {
    new vid = GetPlayerVehicleID(playerid);
    if(vid == 0) {
        SetPlayerPos(playerid, x, y, z);
        if(a != 0) {
            SetPlayerFacingAngle(playerid, a);
        }
    } else {
        SetVehiclePos(vid, x, y, z);
        if(a != 0) {
            SetVehicleZAngle(vid, a);
        }
    }
}

stock SetPlayerVirtualWorldEx(playerid, worldid) {
    if(GetPlayerVehicleSeat(playerid) == 0) {
        SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), worldid);
        SetPlayerVirtualWorld(playerid, worldid);
    } else {
        SetPlayerVirtualWorld(playerid, worldid);
    }
}

new SqlInjectChars[] = {
    32,//空格
    34,//"
    35,//#
    37,//%
    39,//'
    42,//*
    43,//+
    44,//,
    45,//-
    47,///
    58,//:
    59,//;
    60,//<
    61,//=
    62,//>
    63,//?
    64,//@
    96,//`
    123,//{
    125,//}
    126,//~
    127//DEL (delete)
};

stock sqlInject(str[]) {
    for(new i = 0; i < strlen(str); i ++) {
        for(new k = 0; k < sizeof SqlInjectChars; k ++) {
            if(str[i] == SqlInjectChars[k]) {
                return 0;
            }
        }
    }
    return 1;
}

stock isNameColor(name[]) {
    for(new i = 0; i < strlen(name); i ++) {
        if(name[i] == 123 || name[1] == 125) {
            return 1;
        }
    }
    return 0;
}

stock charCheck(str[]) {
	new tz = 0;
	for(new i = 0; i < strlen(str); i++) {
  		if(str[i]!=0) {
	        if(str[i] >= 48 && str[i] <= 57 || str[i] >= 65 && str[i] <= 90 || str[i] >= 97 && str[i] <= 122) {
			    tz = 1;
			} else {
		        tz = 0;
		        i = strlen(str)+1;
		    }
		}
    }
	return tz;
}

stock UnixtoTIme(unixtime) {
    new years = 0, months = 0, days = 0, hours = 0, minutes = 0, seconds = 0, msg[12];
    if(unixtime == 0) {
        format(msg, sizeof msg, "未记录");
        return msg;
    }
    unixtime = gettime() - unixtime;
    while(unixtime != 0) {
        if(unixtime > 32136000) {
            years ++;
            unixtime = unixtime - 32136000;
        } else if(months > 2592000) {
            months ++;
            unixtime = unixtime - 2592000;
        } else if(unixtime > 86400) {
            days ++;
            unixtime = unixtime - 86400;
        } else if(unixtime > 3600) {
            hours ++;
            unixtime = unixtime - 3600;
        } else if(unixtime > 60) {
            minutes ++;
            unixtime = unixtime - 60;
        } else {
            seconds ++;
            unixtime = unixtime - 1;
        }
    }
    if(years != 0) {
        format(msg, sizeof msg, "%d年前", years);
        return msg;
    }
    if(months != 0) {
        format(msg, sizeof msg, "%d月前", months);
        return msg;
    }
    if(days != 0) {
        format(msg, sizeof msg, "%d天前", days);
        return msg;
    }
    if(hours != 0) {
        format(msg, sizeof msg, "%d小时前", hours);
        return msg;
    }
    if(minutes != 0) {
        format(msg, sizeof msg, "%d分钟前", minutes);
        return msg;
    }
    format(msg, sizeof msg, "%d秒前", seconds);
    return msg;
}

new GetVehicleModelName[212][] = {
   "Landstalker",  "Bravura",  "Buffalo", "Linerunner", "Perennial", "Sentinel",
   "Dumper",  "Firetruck" ,  "Trashmaster" ,  "Stretch",  "Manana",  "Infernus",
   "Voodoo", "Pony",  "Mule", "Cheetah", "Ambulance",  "Leviathan",  "Moonbeam",
   "Esperanto", "Taxi",  "Washington",  "Bobcat",  "Mr Whoopee", "BF Injection",
   "Hunter", "Premier",  "Enforcer",  "Securicar", "Banshee", "Predator", "Bus",
   "Rhino",  "Barracks",  "Hotknife",  "Trailer",  "Previon", "Coach", "Cabbie",
   "Stallion", "Rumpo", "RC Bandit",  "Romero", "Packer", "Monster",  "Admiral",
   "Squalo", "Seasparrow", "Pizzaboy", "Tram", "Trailer",  "Turismo", "Speeder",
   "Reefer", "Tropic", "Flatbed","Yankee", "Caddy", "Solair","Berkley's RC Van",
   "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron","RC Raider","Glendale",
   "Oceanic", "Sanchez", "Sparrow",  "Patriot", "Quad",  "Coastguard", "Dinghy",
   "Hermes", "Sabre", "Rustler", "ZR-350", "Walton",  "Regina",  "Comet", "BMX",
   "Burrito", "Camper", "Marquis", "Baggage", "Dozer","Maverick","News Chopper",
   "Rancher", "FBI Rancher", "Virgo", "Greenwood","Jetmax","Hotring","Sandking",
   "Blista Compact", "Police Maverick", "Boxville", "Benson","Mesa","RC Goblin",
   "Hotring Racer", "Hotring Racer", "Bloodring Banger", "Rancher",  "Super GT",
   "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropdust", "Stunt",
   "Tanker", "RoadTrain", "Nebula", "Majestic", "Buccaneer", "Shamal",  "Hydra",
   "FCR-900","NRG-500","HPV1000","Cement Truck","Tow Truck","Fortune","Cadrona",
   "FBI Truck", "Willard", "Forklift","Tractor","Combine","Feltzer","Remington",
   "Slamvan", "Blade", "Freight", "Streak","Vortex","Vincent","Bullet","Clover",
   "Sadler",  "Firetruck", "Hustler", "Intruder", "Primo", "Cargobob",  "Tampa",
   "Sunrise", "Merit",  "Utility Truck",  "Nevada", "Yosemite", "Windsor",  "Monster",
   "Monster","Uranus","Jester","Sultan","Stratum","Elegy","Raindance","RCTiger",
   "Flash","Tahoma","Savanna", "Bandito", "Freight", "Trailer", "Kart", "Mower",
   "Dune", "Sweeper", "Broadway", "Tornado", "AT-400",  "DFT-30", "Huntley",
   "Stafford", "BF-400", "Newsvan","Tug","Trailer","Emperor","Wayfarer","Euros",
   "Hotdog", "Club", "Trailer", "Trailer","Andromada","Dodo","RC Cam", "Launch",
   "Police Car (LSPD)", "Police Car (SFPD)","Police Car (LVPD)","Police Ranger",
   "Picador",   "S.W.A.T. Van",  "Alpha",   "Phoenix",   "Glendale",   "Sadler",
   "Luggage Trailer","Luggage Trailer","Stair Trailer", "Boxville", "Farm Plow",
   "Utility Trailer"
};

new GetWeaponNames[][24] = {
	"空","拳扣","高尔夫球杆","警棍","小刀","棒球棍","铁锹",
	"台球杆","日本刀","电锯","紫色假阴茎","假阴茎","振动器","振动器","花","甘蔗","手雷",
	"催泪瓦斯","燃烧瓶","","","","九妹妹手枪",
	"消音九妹妹手枪","沙鹰","散弹","双管散弹","连散枪","乌兹冲锋枪","MP5","AK47","M4",
	"Tec冲锋枪","步枪","狙击枪","火箭筒","热导追踪",
	"火焰喷射器","加特林","炸药包","遥控器","喷漆","灭火器","相机","夜视镜","夜视镜","降落伞"
};

stock Float:GetVehicleSpeed(vehicleid) {
	new Float:x, Float:y, Float:z;
	GetVehicleVelocity(vehicleid, x, y, z);
	return floatpower((x*x)+(y*y)+(z*z), 0.5) * 180;
}

stock strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}

	new offset = index;
	new result[20];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}

//------------------------------------------------

stock strrest(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[128];
	while ((index < length) && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
