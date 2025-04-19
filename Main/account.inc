#define DIALOG_ACCOUNT                  (100)
#define DIALOG_ACCOUNT_LOGIN            DIALOG_ACCOUNT + 1
#define DIALOG_ACCOUNT_REG              DIALOG_ACCOUNT + 2
#define DIALOG_ACCOUNT_REG_CONFIRM      DIALOG_ACCOUNT + 3
#define DIALOG_ACCOUNT_SZ               DIALOG_ACCOUNT + 4
#define DIALOG_ACCOUNT_WUQI             DIALOG_ACCOUNT + 5
#define DIALOG_ACCOUNT_SZ_TIME          DIALOG_ACCOUNT + 6
#define DIALOG_ACCOUNT_SZ_WEATHER       DIALOG_ACCOUNT + 7
#define DIALOG_ACCOUNT_XGMM_OLD         DIALOG_ACCOUNT + 8
#define DIALOG_ACCOUNT_XGMM_NEW         DIALOG_ACCOUNT + 9
#define DIALOG_ACCOUNT_XGMZ             DIALOG_ACCOUNT + 10
#define DIALOG_ACCOUNT_CDK              DIALOG_ACCOUNT + 11
#define DIALOG_ACCOUNT_CHATTAIL         DIALOG_ACCOUNT + 12
#define DIALOG_ACCOUNT_CHATTAIL_TEXT    DIALOG_ACCOUNT + 13
#define DIALOG_ACCOUNT_CHATTAIL_COLOR   DIALOG_ACCOUNT + 14
#define DIALOG_ACCOUNT_CHECKIN          DIALOG_ACCOUNT + 15 
#define DIALOG_ACCOUNT_TITLE			DIALOG_ACCOUNT + 16
#define ACCOUNT_LOGIN_TIMEOUT           (30)
#define REG_GIVE_SAVINGS                (10000)
#define MAX_ACCESS_LEN                  (128)
#define GetPlayerLevelNeed(%0)          (40*(%0)*(%0))
#define GetPlayerLevelMoney(%0)         (3700*(%0))
#define GetPlayerUniqueWorld(%0)        (MAX_PLAYERS+%0)
#define MAX_CHATTAIL_LEN                (44)
#define CHATTAIL_MONEY                  (1000000)
#define CHATTAIL_TIME                   (2000)
#define XGMM_TIME                       (40)
#define XGMZ_TIME                       (40)
#define MAX_CHATTAIL_TEXT_LEN           (100)
#define CHECKIN_RANDOMMONEY             (9000)//签到奖励 基础 1000  + 随机 9000
#define QA_SOLUTION_RANDOM_MONEY        (5000)//问答随机获取 0 - 1000
#define MAX_SPAWN_POS_LEN               (128)
#define PLAYER_SZ_VALUES                (14)
#define MAX_MISSION_RACE_NAME_LEN       (128)
#define RACE_MISSION_PRICE              (10000)//每日赛车任务奖励
#define SPAWN_MUSIC_URL                 "http://www.170mv.com/kw/antiserver.kuwo.cn/anti.s?rid=MUSIC_41328585&response=res&format=mp3|aac&type=convert_url&br=128kmp3&agent=iPhone&callback=getlink&jpcallback=getlink.mp3"

enum userstruct {
    UID,
    Name[MAX_PLAYER_NAME],
    RegTime,
    RegIp[MAX_PLAYER_IP_LEN],
    LT_Login,
    LT_Exit,
    LoginIp[MAX_PLAYER_IP_LEN],
    Money,
    Savings,
    OnlineTime,
    Level,
    SZ_Value[PLAYER_SZ_VALUES],
	/*
		0 = 接受玩家传送消息, 1 = 伪无敌功能, 2= 自动修车, 3= 屏蔽系统模拟时间, 4= 屏蔽系统随机天气, 5 = 锁定时间为, 6 = 锁定天气为, 7 = 接搜房子传送消息
		8 = OBJ显示, 9 = 速度表, 10 = 自动保存皮肤, 11 = 设置出生点, 12 = 右上角时间显示, 13 = 屏蔽碰撞
	*/
    Access[MAX_ACCESS_LEN],
    ChatTail[MAX_CHATTAIL_LEN],
    BanTime,
    Checkin,
    SkinId,
    SpawnPos[MAX_SPAWN_POS_LEN],
    JailTime,//监狱时间
    Mute,//禁言时间
    MissionRaceName[MAX_MISSION_RACE_NAME_LEN],//每日赛车任务 赛道名称
    MissionRaceUnixTime, //每日赛车任务时间戳
    /*---------------------------------*/
	Title[MAX_PLAYER_NAME],//title
    LTO,//登录超时
    Status,//状态
    OTPP,//在线时间++
    PlayerText:TRTime,//右上角时间
	PlayerText:Speedometer,//速度表
    WorldLock,//专属世界锁
    CountDown,//倒计时
    Warn,//警告次数
    Freeze,//冻结状态
    NoVeh,//禁止使用载具
    GM_Status,//GM状态
    TP_PID,//TP请求PID
    TP_TIMEOUT,//TP请求timeout
    ESC//是否在挂机
};

new userInfo[MAX_PLAYERS][userstruct];

stock ReSetRaceMission(playerid) {
    if(mk_strcmp(userInfo[playerid][MissionRaceName], "0") == 0 && userInfo[playerid][MissionRaceUnixTime] - gettime() < 0) {
        format(userInfo[playerid][MissionRaceName], MAX_MISSION_RACE_NAME_LEN, "%s", PRaceGetRandomRaceName());
        userInfo[playerid][MissionRaceUnixTime] = 0;
        new msg[256];
        format(msg, sizeof msg, "UPDATE account SET mission_race_name='%s',mission_race_unixtime='%d' WHERE uid='%d'",
        userInfo[playerid][MissionRaceName], userInfo[playerid][MissionRaceUnixTime], userInfo[playerid][UID]);
        mysql_query(mariadb, msg, false);
        format(msg, sizeof msg, "[系统] 今日赛车任务已发布, 请登录微信公众号:samp狂野之城,进入UCP-我的查看吧!");
        SCM(playerid, COLOR_BLUE, msg);
    }
}

stock AccountRaceMission(playerid, racename[]) {
    if(mk_strcmp(racename, userInfo[playerid][MissionRaceName]) == 0 && userInfo[playerid][MissionRaceUnixTime] - gettime() < 0) {
        format(userInfo[playerid][MissionRaceName], MAX_MISSION_RACE_NAME_LEN, "0");
        userInfo[playerid][MissionRaceUnixTime] = gettime() + 86400;
        GiveMoney(playerid, RACE_MISSION_PRICE);
        log_money(GetName(playerid), GetIp(playerid), GetPlayerUID(playerid), "mission_race", "mission_race", 0, RACE_MISSION_PRICE, "每日赛车任务获得");
        new msg[256];
        format(msg, sizeof msg, "UPDATE account SET mission_race_name='%s',mission_race_unixtime='%d' WHERE uid='%d'",
        userInfo[playerid][MissionRaceName], userInfo[playerid][MissionRaceUnixTime], userInfo[playerid][UID]);
        mysql_query(mariadb, msg, false);
        format(msg, sizeof msg, "[系统] 你完成了今日的赛车任务 +$%d", RACE_MISSION_PRICE);
        SCM(playerid, COLOR_BLUE, msg);
    }
}

stock isUIDOnline(uid) {
    new pid = -1;
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
        if(IsPlayerConnected(i) == 1 && GetPlayerUID(i) == uid) {
            pid = i;
            break;
        }
    }
    return pid;
}


stock GetPlayerOTime(playerid) {
    return userInfo[playerid][OnlineTime];
}

stock GetPlayerTRTimeStatus(playerid) {
    return userInfo[playerid][SZ_Value][12];
}

stock GetPlayerSpawnStatus(playerid) {
    new msg[MAX_PLAYER_NAME];
    format(msg, sizeof msg, "%s", (userInfo[playerid][SZ_Value][11] == 0 ? ("系统默认") : ("自定义")));
    return msg;
}

stock GetPlayerSpeedometerStatus(playerid) {
    return userInfo[playerid][SZ_Value][9];
}

stock GetPlayerOBJShowStatus(playerid) {
	return userInfo[playerid][SZ_Value][8];
}

stock GetPlayerUID(playerid) {
    return userInfo[playerid][UID];
}

stock GetNameUID(name[]) {
    new Cache:result, msg[128], uid;
    format(msg, sizeof msg, "SELECT uid FROM account WHERE name='%s'", name);
    result = mysql_query(mariadb, msg);
    cache_get_value_name_int(0, "uid", uid);
    cache_delete(result);
    return uid;
}

stock GetUIDName(uid) {
	new Cache:result, msg[128], name[MAX_PLAYER_NAME];
    format(msg, sizeof msg, "SELECT name FROM account WHERE uid='%d'", uid);
    result = mysql_query(mariadb, msg);
    cache_get_value_name(0, "name", name, MAX_PLAYER_NAME);
    cache_delete(result);
    return name;
}

stock IsUIDExist(uid) {
	new Cache:result, msg[128], exist;
    format(msg, sizeof msg, "SELECT COUNT(uid) as exist FROM account WHERE uid='%d'", uid);
    result = mysql_query(mariadb, msg);
    cache_get_value_name_int(0, "exist", exist);
    cache_delete(result);
    return exist;
}

stock GetNameEx(playerid) {
    new name[MAX_PLAYER_NAME];
    format(name, sizeof name, "%s", userInfo[playerid][Name]);
    return name;
}

stock SetPlayerNameEx(playerid, nameex[]) {
    format(userInfo[playerid][Name], MAX_PLAYER_NAME, nameex);
}

stock GiveUIDTitle(uid, title[], from[]) {
	new msg[256];
	format(msg, sizeof msg, "INSERT INTO title (`value`,`created_at`,`from`,`active`,`uid`) VALUES ('%s','%d','%s','0','%d')",  title, gettime(), from, uid);
	mysql_query(mariadb, msg, false);
}

stock IsUIDHasTitle(uid, title[]) {
	new msg[128], Cache:result, counts;
	format(msg, sizeof msg, "SELECT COUNT(uid) FROM title WHERE uid='%d' AND title='%s'", uid, title);
	result = mysql_query(mariadb, msg);
	cache_get_value_name_int(0, "counts", counts);
	cache_delete(result);
	return counts;
}

stock DeleteUIDTitle(uid, title[]) {
	new msg[128];
	format(msg, sizeof msg, "DELETE FROM title WHERE uid='%d' AND title='%s'", uid, title);
	mysql_query(mariadb, msg, false);
}

stock accountSetBanTime(playerid, seconds, reason[]) {
    userInfo[playerid][BanTime] = gettime() + seconds;
    new msg[144];
    format(msg, sizeof msg, "UPDATE account SET bantime='%d',ban_reason='%s' WHERE uid='%d'", userInfo[playerid][BanTime], reason, userInfo[playerid][UID]);
    mysql_query(mariadb, msg, false);
}

stock accountSetNoVEH(playerid) {
    if(userInfo[playerid][NoVeh] == 0) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        SetPlayerPos(playerid, x, y, z + 1.5);
        userInfo[playerid][NoVeh] = 1;
    } else {
        userInfo[playerid][NoVeh] = 0;
    }
}

accountOPSC(playerid, newstate) {
    if(newstate == 2) {
        if(userInfo[playerid][NoVeh] == 1) {
            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);
            SetPlayerPos(playerid, x, y, z + 1.5);
            SCM(playerid, COLOR_RED, "[系统] 当前不可使用载具, 原因:你被禁止了.");
        }
    }
}

accountRuntime_Speedometer(i) {
	if(GetPlayerVehicleSeat(i) == 0 && GetPlayerSpeedometerStatus(i) == 1) {
		new speedstr[12];
        format(speedstr, sizeof speedstr, "%.1f KM/H", GetVehicleSpeed(GetPlayerVehicleID(i)));
        PlayerTextDrawSetString(i, userInfo[i][Speedometer], speedstr);
	} else {
		PlayerTextDrawSetString(i, userInfo[i][Speedometer], "_");
	}
}

stock MutePlayer(playerid, time = 600) {
    userInfo[playerid][Mute] = time;
}

stock throwInJail(playerid, time = 600) {
    userInfo[playerid][JailTime] = time;
}

accountRuntime_TP(playerid) {
    if(userInfo[playerid][TP_PID] != -1) {
        userInfo[playerid][TP_TIMEOUT] --;
        if(userInfo[playerid][TP_TIMEOUT] <= 0) {
            new msg[128];
            format(msg, sizeof msg, "[系统] %s 的TP请求已失效", GetName(userInfo[playerid][TP_PID]));
            SCM(playerid, COLOR_WHITE, msg);
            userInfo[playerid][TP_TIMEOUT] = 0;
            userInfo[playerid][TP_PID] = -1;
        }
    }
}

accountRuntime_Mute(playerid) {
    if(userInfo[playerid][Mute] > 0) {
        userInfo[playerid][Mute] --;
        if(userInfo[playerid][Mute] <= 0) {
            userInfo[playerid][Mute] = 0;
            SCM(playerid, COLOR_RED, "[系统] 禁言已解除, 请好好做人遵循服务器规则.您可以输入/rule查看服务器相关规则");
        }
    }
}

stock IsPlayerInJail(playerid) {
    if(userInfo[playerid][JailTime] > 0) return 1;
    return 0;
}

accountRuntime_Jail(playerid) {
    if(userInfo[playerid][JailTime] > 0) {
        if(IsPlayerInRangeOfPoint(playerid, 8.0, -957.8824,1904.5854,18.56) == 0) {
            SetPlayerPos(playerid, -957.8824,1904.5854,18.56);
        } else {
            userInfo[playerid][JailTime] --;
            if(userInfo[playerid][JailTime] <= 0) {
                userInfo[playerid][JailTime] = 0;
                SCM(playerid, COLOR_RED, "[系统] 你出狱了, 请好好做人并遵循服务器规则.您可以输入/rule查看服务器相关规则");
                SpawnPlayer(playerid);
            }
        }
    }
}

stock GetChatTail(playerid, id) {
    //0 = show, 1 = color, 2 = text
    new idx, msg[44];
    if(id == 0) {
        format(msg, sizeof msg, strtok(userInfo[playerid][ChatTail], idx));
    } else if(id == 1) {
        strtok(userInfo[playerid][ChatTail], idx);
        format(msg, sizeof msg, strtok(userInfo[playerid][ChatTail], idx));
    } else if(id == 2) {
        strtok(userInfo[playerid][ChatTail], idx);
        strtok(userInfo[playerid][ChatTail], idx);
        format(msg, sizeof msg, strrest(userInfo[playerid][ChatTail], idx));
    }
    return msg;
}

accountOPT(playerid, text[]) {
    if(userInfo[playerid][Mute] != 0) {
        SCM(playerid, COLOR_RED, "[系统] 禁言期间不能发言.");
    } else {
        new msg[144], tail[MAX_CHATTAIL_LEN], name[MAX_PLAYER_NAME];
        format(name, sizeof name, GetName(playerid));
        if(text[0] == ';') {
            if(GetPlayerAccess(playerid, "gm_1") == 1) {
                strdel(text, 0, 1);
                format(msg, sizeof msg, "[管理频道] %s(%d): %s", name, playerid, text);
                SCMToAdmin(COLOR_YELLOW, msg);
            } else {
                DIALOG_node(playerid, "你没有权限使用该聊天频道");
            }
        } else {
            if(strval(GetChatTail(playerid, 0)) == 1) {
                format(tail, sizeof tail, " {%s}%s", GetChatTail(playerid, 1), GetChatTail(playerid, 2));
            } else {
                format(tail, 2, "");
            }
            if(GetPlayerVirtualWorld(playerid) == 0) {
                if(!strlen(userInfo[playerid][Title])) {
                    format(msg, sizeof msg, "{%06x}%s(%d): {EFEFF7}%s %s", GetPlayerColor(playerid) >>> 8, name, playerid, text, tail);
                } else {
                    format(msg, sizeof msg, "{%06x}[%s]%s(%d): {EFEFF7}%s %s", GetPlayerColor(playerid) >>> 8, userInfo[playerid][Title], name, playerid, text, tail);
                }
                SCMToAll(COLOR_WHITE, msg);
                if(configQAIsSolution(text) == 1) {
                    new qamoney = random(QA_SOLUTION_RANDOM_MONEY);
                    format(msg, sizeof msg, "[问答] %s 回答正确 +${00FF00}%d", name, qamoney);
                    SCMToAll(COLOR_YELLOW, msg);
                    GiveMoney(playerid, qamoney);
                    log_money(GetName(playerid), GetIp(playerid), GetPlayerUID(playerid), "qa", "qa", 0, qamoney, "问答获得");
                }
            } else {
                format(msg, sizeof msg, "[当前世界] {%06x}%s(%d): {EFEFF7}%s %s", GetPlayerColor(playerid) >>> 8, name, playerid, text, tail);
                SCMToWorld(GetPlayerVirtualWorld(playerid), COLOR_BLUE, msg);
            }
            SetPlayerChatBubble(playerid, text, COLOR_WHITE, 30.0, 8000);
        }
        log_chat(name, GetIp(playerid), text, userInfo[playerid][UID]);
        format(msg, sizeof msg, "%s(%d): %s", name, playerid, text);
        UCPSendChat(msg);
        RedPacketReceive_Wrods(playerid, text);
    }
}

stock SCMToWorld(worlid, color, str[]) {
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
        if(IsPlayerConnected(i) == 1 && GetPlayerVirtualWorld(i) == worlid) {
            SCM(i, color, str);
        }
    }
}

stock GetPlayerAccess(playerid, accessto[]) {
    return GetAccess(userInfo[playerid][Access], accessto);
}

stock GetAccess(access[], accessto[]) {
    if(strcmp(access, "admin") == 0) return 1;
    if(strcmp(accessto, "admin") == 0 && strcmp(access, "admin") != 0) return 0;
    if(strfind(access, accessto) != -1) return 1;
    new accessname[12], accesstoname[12];
    format(accesstoname, 12, accessto);
    format(accessname, 12, accessto);
    strdel(accessname, 3, strlen(accessname));
    if(strcmp(accessname, "gm_") == 0) {
        strdel(accesstoname, 0, strlen(accesstoname)-1);
        new level = strval(accesstoname);
        for(new i = level; i < 10; i ++) {
            format(accessname, 12, "gm_%d", i);
            if(strfind(access, accessname) != -1) {
                return 1;
            }
        }
    }
    return 0;
}

stock GetGMStatus(playerid) {
    return userInfo[playerid][GM_Status];
}

stock GiveSavings(playerid, money) {
    userInfo[playerid][Savings] = userInfo[playerid][Savings] + money;
}

stock GetSavings(playerid) {
    return userInfo[playerid][Savings];
}

stock GiveSavingsOffline(name[], money) {
    new msg[128];
	format(msg, sizeof msg, "UPDATE account SET savings=savings+%d WHERE name='%s'", money, name);
    mysql_query(mariadb, msg, false);
}

stock GiveMoneyOffline(name[], money) {
    new msg[128];
	format(msg, sizeof msg, "UPDATE account SET money=money+%d WHERE name='%s'", money, name);
    mysql_query(mariadb, msg, false);
}

stock GiveMoney(playerid, money) {
    userInfo[playerid][Money] = userInfo[playerid][Money] + money;
    GivePlayerMoney(playerid, money);
}

stock GetMoney(playerid) {
    return userInfo[playerid][Money];
}

stock GetPlayerTeleReceiveStatus(playerid) {
    return userInfo[playerid][SZ_Value][0];
}

stock GetPlayerFZReceiveStatus(playerid) {
    return userInfo[playerid][SZ_Value][7];
}

stock accountSkinChange(playerid, params[]) {
    if(!strlen(params)) {
        SCM(playerid, COLOR_RED, "[系统] 皮肤ID不能为空");
        return 1;
    }
    new sid = strval(params);
    if(sid < 0 || sid > 311) {
        SCM(playerid, COLOR_RED, "[系统] 错误的皮肤ID");
        return 1;
    }
    userInfo[playerid][SkinId] = sid;
    SetPlayerSkin(playerid, sid);
    if(userInfo[playerid][SZ_Value][10] == 1) {
        accountUpdateInt(userInfo[playerid][UID], "skinid", sid);
    }
    new msg[128];
    format(msg, sizeof msg, "[系统] 你把你的皮肤换成了ID:%d", sid);
    SCM(playerid, COLOR_WHITE, msg);
    return 1;
}

stock accountCountDown(playerid) {
    if(userInfo[playerid][CountDown] != -1) {
        new msg[12], Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        if(userInfo[playerid][CountDown] == 0) {
            format(msg, sizeof msg, "~g~GO!!!");
        } else {
            format(msg, sizeof msg, "~r~%d", userInfo[playerid][CountDown]);
        }
        userInfo[playerid][CountDown] --;
        for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
            if(IsPlayerConnected(i) == 1) {
                if(IsPlayerInRangeOfPoint(i, 10.0, x, y, z)) {
                    GameTextForPlayer(i, msg, 888, 3);
                }
            }
        }
    }
}

stock showXGMZ(playerid, err[] = " ") {
    new msg[128], title[88];
    format(title, sizeof title, "更换名字 - {FF0000}%s", err);
    format(msg, sizeof msg, "请输入新名字(需要在线%d分钟)", XGMZ_TIME);
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_XGMZ, DIALOG_STYLE_INPUT, title, msg, "更改", "关闭");
}

stock showXGMM(playerid, err[] = " ") {
    new msg[128], title[88];
    format(title, sizeof title, "修改密码 - {FF0000}%s", err);
    format(msg, sizeof msg, "请输入当前密码(需要在线%d分钟)", XGMM_TIME);
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_XGMM_OLD, DIALOG_STYLE_INPUT, title, msg, "下一步", "关闭");
}

stock showChatTail(playerid) {
    if(strcmp(GetChatTail(playerid, 2), "0") == 0) {
        new msg[128];
        format(msg, sizeof msg, "购买聊天小尾巴($%d)", CHATTAIL_MONEY);
        ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL, DIALOG_STYLE_MSGBOX, "聊天小尾巴", msg, "购买", "关闭");
    } else {
        ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL, DIALOG_STYLE_LIST, "聊天小尾巴", "设置小尾巴文字\n设置小尾巴颜色\n小尾巴:显示或隐藏", "选择", "关闭");
    }
}

CMD:qiandao(playerid, params[]) {
    if(gettime() - userInfo[playerid][Checkin] > 0) {
        ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHECKIN, DIALOG_STYLE_MSGBOX, "签到", "你今天还没签到呢", "签到", "关闭");
    } else {
        DIALOG_node(playerid, "你今天已经签到过了.");
    }
    return 1;
}

CMD:fxq(playerid, params[]) {
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
    return 1;
}

CMD:tp(playerid, params[]) {
    if(!strlen(params)) {
        SCM(playerid, COLOR_WHITE, "[系统] 输入 /tp [对方ID] 可申请传送到对方位置.");
        return 1;
    }
    new pid = strval(params);
    if(pid == playerid) {
        SCM(playerid, COLOR_RED, "[系统] 你不能传送你自己.");
        return 1;
    }
    if(IsPlayerConnected(pid) == 0 || GetPlayerStatus(pid) == 0) {
        SCM(playerid, COLOR_RED, "[系统] 错误的对方ID.");
        return 1;
    }
    if(userInfo[pid][TP_PID] == playerid) {
        SCM(playerid, COLOR_RED, "[系统] 请耐心等待对方的回复.");
        return 1;
    }
    if(userInfo[pid][TP_PID] != -1) {
        SCM(playerid, COLOR_RED, "[系统] 对方正在考虑他人请求.");
        return 1;
    }
    userInfo[pid][TP_PID] = playerid;
    userInfo[pid][TP_TIMEOUT] = 30;
    new msg[128];
    format(msg, sizeof msg, "[系统] 你已向 %s 请求TP, 请等待对方回复.", GetName(pid));
    SCM(playerid, COLOR_WHITE, msg);
    format(msg, sizeof msg, "{FFFF00}[系统] %s 请求传送到你的身边, 同意输入'/tpy', 拒绝输入'/tpn'", GetName(playerid));
    SCM(pid, COLOR_WHITE, msg);
    return 1;
}

CMD:tpn(playerid, params[]) {
    if(userInfo[playerid][TP_PID] == -1) {
        SCM(playerid, COLOR_RED, "[系统] 没有人向你请求TP.");
        return 1;
    }
    if(IsPlayerConnected(userInfo[playerid][TP_PID]) == 0) {
        SCM(playerid, COLOR_RED, "[系统] 对方已离线.");
        return 1;
    }
    if(GetPlayerStatus(userInfo[playerid][TP_PID]) == 0) {
        SCM(playerid, COLOR_RED, "[系统] 请求已失效.");
        return 1;
    }
    new msg[128];
    format(msg, sizeof msg, "[系统] 你拒绝了 %s 的TP请求.", GetName(userInfo[playerid][TP_PID]));
    SCM(playerid, COLOR_WHITE, msg);
    format(msg, sizeof msg, "[系统] %s 拒绝了你的TP请求.", GetName(playerid));
    SCM(userInfo[playerid][TP_PID], COLOR_RED, msg);
    userInfo[playerid][TP_PID] = -1;
    userInfo[playerid][TP_TIMEOUT] = 0;
    return 1;
}

CMD:tpy(playerid, params[]) {
    if(userInfo[playerid][TP_PID] == -1) {
        SCM(playerid, COLOR_RED, "[系统] 没有人向你请求TP.");
        return 1;
    }
    if(IsPlayerConnected(userInfo[playerid][TP_PID]) == 0) {
        SCM(playerid, COLOR_RED, "[系统] 对方已离线.");
        return 1;
    }
    if(GetPlayerStatus(userInfo[playerid][TP_PID]) == 0) {
        SCM(playerid, COLOR_RED, "[系统] 请求已失效.");
        return 1;
    }
    new msg[128], Float:x, Float:y, Float:z;
    format(msg, sizeof msg, "[系统] %s 同意了你的TP请求.", GetName(playerid));
    SCM(userInfo[playerid][TP_PID], COLOR_WHITE, msg);
    format(msg, sizeof msg, "[系统] 你同意了 %s 的TP请求.", GetName(userInfo[playerid][TP_PID]));
    SCM(playerid, COLOR_WHITE, msg);
    GetPlayerPos(playerid, x, y, z);
    SetPlayerPosEx(userInfo[playerid][TP_PID], x, y, z+1);
    userInfo[playerid][TP_PID] = -1;
    userInfo[playerid][TP_TIMEOUT] = 0;
    return 1;
}

CMD:djs(playerid, params[]) {
    new msg[128];
    format(msg, sizeof msg, "[系统] %s 开始了倒计时!", GetName(playerid));
    SCMToAll(COLOR_WHITE, msg);
    userInfo[playerid][CountDown] = 5;
    return 1;
}

CMD:count(playerid, params[]) {
    new msg[128];
    format(msg, sizeof msg, "[系统] %s 开始了倒计时!", GetName(playerid));
    SCMToAll(COLOR_WHITE, msg);
    userInfo[playerid][CountDown] = 5;
    return 1;
}

CMD:hf(playerid, params[]) {
    accountSkinChange(playerid, params);
    return 1;
}

CMD:skin(playerid, params[]) {
    accountSkinChange(playerid, params);
    return 1;
}

CMD:chattail(playerid, params[]) {
    showChatTail(playerid);
    return 1;
}

CMD:xgmz(playerid, params[]) {
    showXGMZ(playerid);
    return 1;
}

CMD:xgmm(playerid, params[]) {
    showXGMM(playerid);
    return 1;
}

CMD:pm(playerid, params[]) {
    new tmp[128], idx;
    tmp = strtok(params, idx);
    if(!strlen(tmp)) {
        SCM(playerid, COLOR_WHITE, "[系统] /pm [对方ID] [内容] 发送密语至对方.");
        return 1;
    }
    new pid = strval(tmp);
    if(pid == playerid) {
        SCM(playerid, COLOR_RED, "[系统] 你不能密语你自己.");
        return 1;
    }
    if(IsPlayerConnected(pid) == 0) {
        SCM(playerid, COLOR_RED, "[系统] 错误的对方ID.");
        return 1;
    }
    tmp = strrest(params, idx);
    if(!strlen(tmp)) {
        SCM(playerid, COLOR_RED, "[系统] 内容不能为空.");
        return 1;
    }
    new msg[144];
    format(msg, sizeof msg, "[密语] %s - 来至 %s(%d)", tmp, GetName(playerid), playerid);
    SCM(pid, COLOR_WHITE, msg);
    format(msg, sizeof msg, "[密语] %s - 发至 %s(%d)", tmp, GetName(pid), pid);
    SCM(playerid, COLOR_WHITE, msg);
    return 1;
}

CMD:ww(playerid, params[]) {
    if(GetPlayerVirtualWorld(playerid) == GetPlayerUniqueWorld(playerid)) {
        SCM(playerid, COLOR_BLUE, "[世界] 你已经在你的专属世界了.");
        return 1;
    }
    SetPlayerVirtualWorldEx(playerid, GetPlayerUniqueWorld(playerid));
    SCM(playerid, COLOR_BLUE, "[世界] 你进入了你的专属世界.");
    return 1;
}

CMD:w(playerid, params[]) {
    new tmp[128], idx;
    tmp = strtok(params, idx);
    if(!strlen(tmp)) {
        if(GetPlayerVirtualWorld(playerid) == 0) {
            SCM(playerid, COLOR_WHITE, "[世界] 你已经在大世界了.");
            return 1;
        }
        new vid = GetPlayerTemporaryVID(playerid);
        if(GetVehicleModel(vid) == 577) {
            temporary_vehicleInit(playerid);
        }
        SetPlayerVirtualWorldEx(playerid, 0);
        SCM(playerid, COLOR_BLUE, "[世界] 你回到了大世界.");
        ResetPlayerWeapons(playerid);
        return 1;
    }
    if(strcmp(tmp, "help", true) == 0) {
        SCM(playerid, COLOR_BLUE, "[世界] 输入 /ww 可进入您的专属世界");
        SCM(playerid, COLOR_BLUE, "[世界] 输入 /w kick [对方ID] 可把某人踢出专属世界");
        SCM(playerid, COLOR_BLUE, "[世界] 输入 /w lock 可关闭专属世界, 别人就不可以进来了");
        SCM(playerid, COLOR_BLUE, "[世界] 输入 /w [对方ID] 可进入他人的专属世界");
        SCM(playerid, COLOR_BLUE, "[世界] 输入 /w 可回到大世界");
        return 1;
    }
    if(strcmp(tmp, "kick", true) == 0) {
        tmp = strtok(params, idx);
        if(!strlen(tmp)) {
            SCM(playerid, COLOR_WHITE, "[世界] 对方ID不能为空.");
            return 1;
        }
        new pid = strval(tmp);
        if(IsPlayerConnected(pid) == 0 || GetPlayerStatus(pid) == 0) {
            SCM(playerid, COLOR_BLUE, "[世界] 错误的对方ID.");
            return 1;
        }
        if(GetPlayerVirtualWorld(pid) != GetPlayerUniqueWorld(playerid)) {
            SCM(playerid, COLOR_BLUE, "[世界] 对方不在你的专属世界.");
            return 1;
        }
        new msg[128];
        format(msg, sizeof msg, "[世界] %s 把你踢出了ta的专属世界.", GetName(playerid));
        SetPlayerVirtualWorldEx(pid, 0);
        SCM(pid, COLOR_BLUE, msg);
        return 1;
    }
    if(strcmp(tmp, "lock", true) == 0) {
        if(userInfo[playerid][WorldLock] == 0) {
            userInfo[playerid][WorldLock] = 1;
            SCM(playerid, COLOR_BLUE, "[世界] 你开启了你的专属世界对他人开放.");
        } else {
            userInfo[playerid][WorldLock] = 0;
            SCM(playerid, COLOR_BLUE, "[世界] 你关闭了你的专属世界对他人开放.");
        }
        return 1;
    }
    new pid = strval(tmp);
    if(IsPlayerConnected(pid) == 0 || GetPlayerStatus(pid) == 0) {
        SCM(playerid, COLOR_BLUE, "[世界] 错误的对方ID.");
        return 1;
    }
    if(userInfo[pid][WorldLock] == 1) {
        SCM(playerid, COLOR_BLUE, "[世界] 对方已关闭ta的专属世界.");
        return 1;
    }
    SetPlayerVirtualWorldEx(playerid, GetPlayerUniqueWorld(pid));
    new msg[128];
    format(msg, sizeof msg, "[世界] 你进入了 %s 的专属世界.", GetName(pid));
    SCM(playerid, COLOR_BLUE, msg);
    format(msg, sizeof msg, "[世界] %s 进入了你的专属世界.", GetName(playerid));
    SCM(pid, COLOR_BLUE, msg);
    return 1;
}

/*CMD:wuqi(playerid, params[]) {
    showDialogWeapon(playerid);
    return 1;
}*/

showDialogWeapon(playerid ,id=-1) {
    if(id != -1) {
        id = id + 1;
        if(id >= 18) id = id + 3;
        GivePlayerWeapon(playerid, id, 9999);
    }
    new msg[2048];
    for(new i = 1; i < sizeof GetWeaponNames; i ++) {
        if(strlen(GetWeaponNames[i])) {
            format(msg, sizeof msg, "%s%s\n", msg, GetWeaponNames[i]);
        }
    }
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_WUQI, DIALOG_STYLE_LIST, "武器商店", msg, "选择", "关闭");
}

stock showCDKDialog(playerid, err[] = " ") {
    new title[88], msg[64];
    format(title, sizeof title, "兑换CDK - {FF0000}%s", err);
    format(msg, sizeof msg, "请输入CDK兑换码");
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CDK, DIALOG_STYLE_INPUT, title, msg, "兑换", "关闭");
}

CMD:cdk(playerid, params[]) {
    showCDKDialog(playerid);
    return 1;
}

CMD:wdch(playerid, params[]) {
	new query[128], Cache:result, counts;
	format(query, sizeof query, "SELECT value,active FROM title WHERE uid='%d'", GetPlayerUID(playerid));
	result = mysql_query(mariadb, query);
	cache_get_row_count(counts);
	if(counts == 0) {
		DIALOG_node(playerid, "你暂时没有称号,请关注微信公众号:samp狂野之城.申请");
	} else {
		new title[MAX_PLAYER_NAME], active, msg[2048];
		for(new i = 0; i < counts; i ++) {
			cache_get_value_name(i, "value", title, MAX_PLAYER_NAME);
			cache_get_value_name_int(i, "active", active);
			format(msg, sizeof msg, "%s%s%s\n", msg, (active == 1 ? ("{00FF00}") : ("")), title);
		}
		ShowPlayerDialog(playerid, DIALOG_ACCOUNT_TITLE, DIALOG_STYLE_LIST, "我的称号", msg, "选择", "关闭");
	}
	cache_delete(result);
	return 1;
}

CMD:sz(playerid, params[]) {
    new msg[1024], status[2][16] = {"{FF0000}关", "{00FF00}开"};
    format(msg, sizeof msg, "\
    接受玩家传送消息: %s\n\
    伪无敌功能: %s\n\
    自动修车: %s\n\
    屏蔽系统模拟时间: %s\n\
    屏蔽系统随机天气: %s\n\
    锁定时间为: %d\n\
    锁定天气为: %d\n\
    接受传送消息: %s\n\
	OBJ显示: %s\n\
    速度表: %s\n\
    自动保存皮肤: %s\n\
    出生点: %s\n\
    关闭时间显示: %s\n\
    屏蔽碰撞: %s\
    ", status[userInfo[playerid][SZ_Value][0]] , status[userInfo[playerid][SZ_Value][1]] , status[userInfo[playerid][SZ_Value][2]], status[userInfo[playerid][SZ_Value][3]], 
    status[userInfo[playerid][SZ_Value][4]], userInfo[playerid][SZ_Value][5], userInfo[playerid][SZ_Value][6],  status[userInfo[playerid][SZ_Value][7]],
	status[userInfo[playerid][SZ_Value][8]], status[userInfo[playerid][SZ_Value][9]], status[userInfo[playerid][SZ_Value][10]], GetPlayerSpawnStatus(playerid),
    status[userInfo[playerid][SZ_Value][12]], status[userInfo[playerid][SZ_Value][13]]);
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ, DIALOG_STYLE_LIST, "自定义设置{F35ACE}/SZ", msg, "设置", "关闭");
    return 1;
}

CMD:zhxx(playerid, params[]) {
    new pid = (!strlen(params) ? (playerid) : (strval(params)));
    if(IsPlayerConnected(pid) == 0 || GetPlayerStatus(pid) == 0) {
        ShowPlayerDialog(playerid, 1, DIALOG_STYLE_MSGBOX, "账号信息", "ERR:对方未登录", "关闭", "");
        return 1;
    }
    new msg[1024];
    format(msg, sizeof msg, "\
    账号名: %s\n\
    账号ID(PID): %d\n\
    注册时间: %s\n\
    上次登录时间: %s\n\
    上次退出时间: %s\n\
    现金: $%d\n\
    银行存款: $%d\n\
    在线时间: %d 分钟\n\
    等级: %d\n\
    下一级需要满 %d 分钟\n\
    %s\n\
    ", GetName(pid), userInfo[pid][UID], UnixtoTIme(userInfo[pid][RegTime]), UnixtoTIme(userInfo[pid][LT_Login]), UnixtoTIme(userInfo[pid][LT_Exit]),
    userInfo[pid][Money], userInfo[pid][Savings], userInfo[pid][OnlineTime], userInfo[pid][Level], GetPlayerLevelNeed(userInfo[pid][Level]), 
    GetPlayerHouseStats(pid));
    ShowPlayerDialog(playerid, 1, DIALOG_STYLE_LIST, "账号信息", msg, "关闭", "");
    return 1;
}

public OnPlayerLogin(playerid) {
    userInfo[playerid][LTO] = -1;
    new Cache:result, query[128], ip[MAX_PLAYER_IP_LEN], sz_value[128], idx;
    GetPlayerIp(playerid, ip, sizeof ip);
    format(query, sizeof query, "SELECT * FROM account WHERE name = '%s'", GetName(playerid));
    result = mysql_query(mariadb, query);
    cache_get_value_name_int(0, "uid", userInfo[playerid][UID]);
    cache_get_value_name(0, "name", userInfo[playerid][Name], MAX_PLAYER_NAME);
    cache_get_value_name_int(0, "reg_time", userInfo[playerid][RegTime]);
    cache_get_value_name(0, "reg_ip", userInfo[playerid][RegIp], MAX_PLAYER_IP_LEN);
    cache_get_value_name_int(0, "lt_login", userInfo[playerid][LT_Login]);
    cache_get_value_name_int(0, "lt_exit", userInfo[playerid][LT_Exit]);
    cache_get_value_name(0, "login_ip", userInfo[playerid][LoginIp], MAX_PLAYER_IP_LEN);
    cache_get_value_name_int(0, "money", userInfo[playerid][Money]);
    cache_get_value_name_int(0, "savings", userInfo[playerid][Savings]);
    cache_get_value_name_int(0, "onlinetime", userInfo[playerid][OnlineTime]);
    cache_get_value_name_int(0, "level", userInfo[playerid][Level]);
    cache_get_value_name(0, "sz_value", sz_value);
    cache_get_value_name(0, "access", userInfo[playerid][Access], MAX_ACCESS_LEN);
    cache_get_value_name(0, "chattail", userInfo[playerid][ChatTail], MAX_CHATTAIL_LEN);
    cache_get_value_name_int(0, "bantime", userInfo[playerid][BanTime]);
    cache_get_value_name_int(0, "checkin", userInfo[playerid][Checkin]);
    cache_get_value_name_int(0, "skinid", userInfo[playerid][SkinId]);
    cache_get_value_name(0, "spawn_pos", userInfo[playerid][SpawnPos], MAX_SPAWN_POS_LEN);
    cache_get_value_name_int(0, "jail_time", userInfo[playerid][JailTime]);
    cache_get_value_name_int(0, "mute_time", userInfo[playerid][Mute]);
    cache_get_value_name(0, "mission_race_name", userInfo[playerid][MissionRaceName], MAX_MISSION_RACE_NAME_LEN);
    cache_get_value_name_int(0, "mission_race_unixtime", userInfo[playerid][MissionRaceUnixTime]);
    cache_delete(result);
    for(new i = 0; i < PLAYER_SZ_VALUES; i ++) {
        userInfo[playerid][SZ_Value][i] = strval(strtok(sz_value, idx));
    }
    userInfo[playerid][SkinId] = (userInfo[playerid][SZ_Value][10] == 0 ? (0) : (userInfo[playerid][SkinId]));
    DisableRemoteVehicleCollisions(playerid, userInfo[playerid][SZ_Value][13]);
    userInfo[playerid][OTPP] = 0;
    userInfo[playerid][Status] = 1;
    userInfo[playerid][WorldLock] = 0;
    userInfo[playerid][Warn] = 0;
    userInfo[playerid][Freeze] = 1;
    userInfo[playerid][NoVeh] = 0;
    userInfo[playerid][GM_Status] = 0;
    userInfo[playerid][TP_PID] = -1;
    userInfo[playerid][TP_TIMEOUT] = 0;
    format(query, sizeof query, "UPDATE account SET lt_login = '%d', login_ip = '%s' WHERE uid = '%d'", gettime(), ip, userInfo[playerid][UID]);
    mysql_query(mariadb, query, false);
    userInfo[playerid][TRTime] = CreatePlayerTextDraw(playerid, 582.00, 25.303728, "_");
    PlayerTextDrawLetterSize(playerid, userInfo[playerid][TRTime], 0.300000, 2.000000);
    PlayerTextDrawAlignment(playerid, userInfo[playerid][TRTime], 2);
    PlayerTextDrawColor(playerid, userInfo[playerid][TRTime], -1);
    PlayerTextDrawSetShadow(playerid, userInfo[playerid][TRTime], 2);
    PlayerTextDrawSetOutline(playerid, userInfo[playerid][TRTime], 0);
    PlayerTextDrawBackgroundColor(playerid, userInfo[playerid][TRTime], 51);
    PlayerTextDrawFont(playerid, userInfo[playerid][TRTime], 2);
    PlayerTextDrawSetProportional(playerid, userInfo[playerid][TRTime], 1);
    PlayerTextDrawShow(playerid, userInfo[playerid][TRTime]);
	userInfo[playerid][Speedometer] = CreatePlayerTextDraw(playerid, 580.00, 377.303728, "_");
    PlayerTextDrawLetterSize(playerid, userInfo[playerid][Speedometer], 0.450000, 2.400000);
    PlayerTextDrawAlignment(playerid, userInfo[playerid][Speedometer], 2);
    PlayerTextDrawColor(playerid, userInfo[playerid][Speedometer], 0x0088FFFF);
    PlayerTextDrawSetShadow(playerid, userInfo[playerid][Speedometer], 2);
    PlayerTextDrawSetOutline(playerid, userInfo[playerid][Speedometer], 1);
    PlayerTextDrawBackgroundColor(playerid, userInfo[playerid][Speedometer], 51);
    PlayerTextDrawFont(playerid, userInfo[playerid][Speedometer], 1);
    PlayerTextDrawSetProportional(playerid, userInfo[playerid][Speedometer], 1);
    PlayerTextDrawShow(playerid, userInfo[playerid][Speedometer]);
    SetPlayerIntoDM(playerid, -1);
    ReSetRaceMission(playerid);
    ACConnect(playerid);
    BillBoardConnect(playerid);
    if(GetPlayerAccess(playerid, "gm_1") == 1) {
        SCM(playerid, COLOR_RED, "[系统] 尊敬的GM,欢迎回来! 你可以输入 /gm login 来进入执勤状态./gm help 查看GM帮助");
    }
    Attire.load(playerid);
	LoadingTitle(playerid);
	configKGOBJ(playerid);
    pc_cmd_sm(playerid, "fuckyou");
	SpawnPlayer(playerid);
    StopAudioStreamForPlayer(playerid);
    return 1;
}

CMD:sm(playerid, params[]) {
    new msg[2048];
    format(msg, sizeof msg, "最近狂野发生了一些事，在这里我想说明下：\n");
    format(msg, sizeof msg, "%s无论你之前在哪个服务器，我都欢迎你来到狂野玩，狂野欢迎所有想要正常游戏的玩家，不针对其他任何服务器\n", msg);
    format(msg, sizeof msg, "%s狂野从2018年5月份开服至今，旨在为各位热爱SAMP的玩家提供一个良好的游戏平台，这也是我开狂野的初衷。\n", msg);
    format(msg, sizeof msg, "%s但是近期发现有的玩家，故意挑事，打擦边球，对于此类玩家，你可以输入/q 一键退服了，任何挑事等行为，都\n", msg);
    format(msg, sizeof msg, "%s将受到惩罚，如被GM处罚，请先想想自己做了什么，是否有仔细的了解服务器相关规则。如果对GM处罚有疑问，\n", msg);
    format(msg, sizeof msg, "%s或者被骚扰了，也可以进入微信公众号：samp狂野之城，进行举报（附上截图）。\n", msg);
    format(msg, sizeof msg, "%s总结就是一句话，好好玩游戏的我欢迎，故意找茬的，就没必要来了。\n", msg);
    format(msg, sizeof msg, "%s狂野的发展离不开每一位玩家，需要每一位玩家遵守游戏的相关规则，希望大家积极配合GM的工作，让狂野变得\n", msg);
    format(msg, sizeof msg, "%s更好。同时服务器也会经常组织活动，欢迎加入官方QQ群：630203140，获得最新活动消息\n", msg);
    format(msg, sizeof msg, "%s\n", msg);
    format(msg, sizeof msg, "%s                                                                                                                                                                                               Nocturne \n", msg);
    format(msg, sizeof msg, "%s                                                                                                                                                                                                2020.3.13 \n", msg);
    ShowPlayerDialog(playerid, 1, DIALOG_STYLE_MSGBOX, "至所有玩家", msg, "了解", "");
    return 1;
}

stock LoadingTitle(playerid) {
	new msg[128], Cache:result, counts;
	format(msg, sizeof msg, "SELECT value FROM title WHERE active='1' AND uid='%d'", GetPlayerUID(playerid));
	result = mysql_query(mariadb, msg);
	cache_get_row_count(counts);
	if(counts == 1) {
		cache_get_value_name(0, "value", userInfo[playerid][Title], MAX_PLAYER_NAME);
	} else {
		format(userInfo[playerid][Title], 2, "");
	}
	cache_delete(result);
}

stock accountSpawn(playerid) {
    if(GetPlayerStatus(playerid) == 1) {
        if(userInfo[playerid][SZ_Value][11] == 0) {
            configPSPRandom(playerid);
        } else {
            new Float:x, Float:y, Float:z, Float:a;
            sscanf(userInfo[playerid][SpawnPos], "ffff", x, y, z, a);
            SetPlayerPos(playerid, x, y, z);
            SetPlayerFacingAngle(playerid, a);
        }
        SetPlayerTimeEx(playerid);
        SetPlayerWeatherEx(playerid);
        SetPlayerSkin(playerid, userInfo[playerid][SkinId]);
        Attire.Spawn(playerid);
    }
}

stock GetPlayerStatus(playerid) {
    return userInfo[playerid][Status];
}

stock accountConnect(playerid) {
    SpawnPlayer(playerid);
    SetPlayerColor(playerid, PlayerColors[playerid]);
    userInfo[playerid][CountDown] = -1;
    userInfo[playerid][UID] = 0;
    userInfo[playerid][LTO] = -1;
    userInfo[playerid][Status] = 0;
}

stock accountDisconnect(playerid) {
    if(GetPlayerStatus(playerid) == 1) {
        accountSaveData(playerid);
        new query[128];
        format(query, sizeof query, "UPDATE account SET lt_exit = '%d' WHERE uid = '%d'", gettime(), userInfo[playerid][UID]);
        mysql_query(mariadb, query, false);
        PlayerTextDrawDestroy(playerid, userInfo[playerid][TRTime]);
        PlayerTextDrawDestroy(playerid, userInfo[playerid][Speedometer]);
    }
}

stock accountSaveData(playerid) {
    new msg[512];
    format(msg, sizeof msg, "UPDATE account SET onlinetime='%d', money='%d', savings='%d', jail_time='%d', mute_time='%d' WHERE uid='%d'",
    userInfo[playerid][OnlineTime], userInfo[playerid][Money], userInfo[playerid][Savings], userInfo[playerid][JailTime], userInfo[playerid][Mute],
    userInfo[playerid][UID]);
    mysql_query(mariadb, msg, false);
}

stock accountSetLevel(playerid, level) {
    userInfo[playerid][Level] = level;
    new msg[128];
    format(msg, sizeof msg, "UPDATE account SET level = '%d' WHERE uid = '%d'", userInfo[playerid][Level], userInfo[playerid][UID]);
    mysql_query(mariadb, msg, false);
    SetPlayerScore(playerid, level);
}

stock accountIsNameExist(name[]) {
    new Cache:result, exist, query[128];
    format(query, sizeof query, "SELECT COUNT(name) AS exist FROM account WHERE name = '%s'", name);
    result = mysql_query(mariadb, query);
    cache_get_value_name_int(0, "exist", exist);
    cache_delete(result);
    return exist;
}

stock checkSameIp(playerid) {
    new ip[MAX_PLAYER_NAME], checked = -1;
    format(ip, sizeof ip, GetIp(playerid));
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
        if(IsPlayerConnected(i) && i != playerid) {
            if(userInfo[i][ESC] > 3 && mk_strcmp(ip, GetIp(i)) == 0) {
                checked = i;
                break;
            }
        }
    }
    return checked;
}

stock accountLogin(playerid) {
    if(GetRecyclingStatus() == 1) {
        DIALOG_node(playerid, "服务器目前不能进入, 原因:系统正在自动清理资源");
        KickEx(playerid, "资源清理中");
    } else {
        if(checkSameIp(playerid) != -1) {
            DIALOG_node(playerid, "不能多开哦");
            KickEx(playerid, "多开");
        } else {
            new name[MAX_PLAYER_NAME];
            format(name, sizeof name, GetName(playerid));
            if(accountIsNameExist(name) == 0) {
                if(strlen(name) < 2) {
                    KickEx(playerid, "昵称长度不符合标准");
                } else {
                    showReg(playerid);
                }
            } else {
                new ip[MAX_PLAYER_IP_LEN], login_ip[MAX_PLAYER_IP_LEN], Cache:result, reason[64], query[256], bantime, nowtime = gettime();
                GetPlayerIp(playerid, ip, sizeof ip);
                format(query, sizeof query, "SELECT login_ip,bantime,ban_reason FROM account WHERE name = '%s'", name);
                result = mysql_query(mariadb, query);
                cache_get_value_name(0, "login_ip", login_ip, MAX_PLAYER_IP_LEN);
                cache_get_value_name_int(0, "bantime", bantime);
                cache_get_value_name(0, "ban_reason", reason);
                cache_delete(result);
                if(bantime == -1) {
                    format(query, sizeof query, "该账号已被封禁, 原因:%s\n解封时间:无期\n如有疑问请关注微信公众号:samp狂野之城 进行申诉", reason);
                    DIALOG_node(playerid, query);
                    KickEx(playerid, "该账号已被封杀");
                } else if(bantime - nowtime > 0) {
                    bantime = (bantime - nowtime) / 3600;
                    format(query, sizeof query, "该账号已被封禁, 原因:%s\n解封时间:%d小时后\n如有疑问请关注微信公众号:samp狂野之城 进行申诉", reason, bantime);
                    DIALOG_node(playerid, query);
                    KickEx(playerid, "该账号已被封杀");
                } else {
                    PlayAudioStreamForPlayer(playerid, SPAWN_MUSIC_URL);
                    if(strcmp(ip, login_ip) == 0) {
                        SetTimerEx("AutoLogin", 1000, false, "i", playerid);
                    } else {
                        userInfo[playerid][LTO] = ACCOUNT_LOGIN_TIMEOUT;
                        showLogin(playerid);
                    }
                }
            }
        }
    }
}

forward AutoLogin(playerid);
public AutoLogin(playerid) {
    SCM(playerid, COLOR_GREEN, "-| 检测到ip与上次登录时相同, 自动登录 |-");
    OnPlayerLogin(playerid);
    return 1;
}

stock showLogin(playerid, err[] = " ") {
    new title[88], msg[128];
    format(title, sizeof title, "{FF0000}%s", err);
    format(msg, sizeof msg, "{FFFF00}昵称 {FF0000}%s {FFFF00}已注册, 请在{FF0000}%d{FFFF00}秒内输入密码登录", GetName(playerid), ACCOUNT_LOGIN_TIMEOUT);
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_LOGIN, DIALOG_STYLE_PASSWORD, title, msg, "登录", "退出");
}

stock showReg(playerid, err[] = " ") {
    new title[88], msg[512];
    format(title, sizeof title, "{FF0000}%s", err);
    format(msg, sizeof msg, "{FFFFFF}欢迎 {FF0000}%s {FFFFFF}您尚未在本服务器注册\n{FFFFFF}请输入{FF0000}3-20{FFFFFF}长度的由仅含{FF0000}数字, 大小写英文字母{FFFFFF}密码进行注册{FF0000}请注意密码强度\n{FFFF00}密码区分大小写, 请牢记您的注册密码", GetName(playerid));
    ShowPlayerDialog(playerid, DIALOG_ACCOUNT_REG, DIALOG_STYLE_PASSWORD, title, msg, "登录", "退出");
}

password2etect(name[], password[]) {
    new Cache:result, ojbk, query[128];
    format(query, sizeof query, "SELECT COUNT(password) AS ojbk FROM account WHERE name='%s' AND password = MD5('%s')", name, password);
    result = mysql_query(mariadb, query);
    cache_get_value_name_int(0, "ojbk", ojbk);
    cache_delete(result);
    return ojbk;
}

stock accountUpdateSZValue(playerid) {
    new msg[256];
    for(new i = 0; i < PLAYER_SZ_VALUES; i ++) {
        format(msg, sizeof msg, "%s%d ", msg, userInfo[playerid][SZ_Value][i]);
    }
    format(msg, sizeof msg, "UPDATE account SET sz_value='%s' WHERE uid='%d'", msg, userInfo[playerid][UID]);
    mysql_query(mariadb, msg, false);
}

stock accountUpdateInt(uid, columnname[], value) {
    new msg[128];
    format(msg, sizeof msg, "UPDATE account SET %s='%d' WHERE uid='%d'", columnname, value, uid);
    mysql_query(mariadb, msg, false);
}

forward accountODR(playerid, dialogid, response, listitem, inputtext[]);
public accountODR(playerid, dialogid, response, listitem, inputtext[]) {
	if(dialogid == DIALOG_ACCOUNT_TITLE) {
		if(!response) return 1;
		new msg[128], Cache:result, titleid, active;
		format(msg, sizeof msg, "SELECT id,active FROM title WHERE uid='%d'", GetPlayerUID(playerid));
		result = mysql_query(mariadb, msg);
		cache_get_value_name_int(listitem, "id", titleid);
		cache_get_value_name_int(listitem, "active", active);
		cache_delete(result);
		format(msg, sizeof msg, "UPDATE title SET active='0' WHERE active='1' AND uid='%d'", GetPlayerUID(playerid));
		mysql_query(mariadb, msg, false);
		if(active == 0) {
			format(msg, sizeof msg, "UPDATE title SET active='1' WHERE id='%d' AND uid='%d'", titleid, GetPlayerUID(playerid));
			mysql_query(mariadb, msg, false);
		}
		LoadingTitle(playerid);
		return 1;
	}
    if(dialogid == DIALOG_ACCOUNT_CHECKIN) {
        if(!response) return 1;
        new msg[128], checkin_money = 1000 + random(CHECKIN_RANDOMMONEY);
        userInfo[playerid][Checkin] = gettime() + 86400;
        accountUpdateInt(userInfo[playerid][UID], "checkin", userInfo[playerid][Checkin]);
        GiveSavings(playerid, checkin_money);
        accountSaveData(playerid);
        format(msg, sizeof msg, "签到获得 $%d", checkin_money);
        DIALOG_node(playerid, msg);
        log_money(GetName(playerid), GetIp(playerid), GetPlayerUID(playerid), "checkin", "checkin", 0, checkin_money, "签到获得");
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_CDK) {
        if(!response) return 1;
        if(strcmp(inputtext, defaultIT) == 0) {
            showCDKDialog(playerid, "兑换码不能为空");
            return 1;
        }
        if(sqlInject(inputtext) == 0) {
            showCDKDialog(playerid, "错误的兑换码");
            return 1;
        }
        new msg[256], Cache:result, status;
        format(msg, sizeof msg, "SELECT COUNT(id) as exist FROM server_cdk WHERE keycode='%s'", inputtext);
        result = mysql_query(mariadb, msg);
        cache_get_value_name_int(0, "exist", status);
        cache_delete(result);
        if(status == 0) {
            showCDKDialog(playerid, "错误的兑换码");
            return 1;
        }
        new id, keyword[MAX_PLAYER_NAME], value[128];
        format(msg, sizeof msg, "SELECT * FROM server_cdk WHERE keycode='%s'", inputtext);
        result = mysql_query(mariadb, msg);
        cache_get_value_name_int(0, "id", id);
        cache_get_value_name(0, "keyword", keyword, MAX_PLAYER_NAME);
        cache_get_value_name(0, "value", value, sizeof value);
        cache_get_value_name_int(0, "status", status);
        cache_delete(result);
        if(status == 1) {
            showCDKDialog(playerid, "该兑换码已经被使用过了");
            return 1;
        }
        if(mk_strcmp(keyword, "金钱") == 0) {
            new money = strval(value);
            GiveMoney(playerid, money);
            log_money(GetName(playerid), GetIp(playerid), GetPlayerUID(playerid), "cdk", keyword, 0, money, inputtext);
            format(msg, sizeof msg, "兑换成功, 你兑换了 $%d", money);
        } else if(mk_strcmp(keyword, "点歌卡") == 0) {
            new cards = strval(value);
            format(msg, sizeof msg, "UPDATE account SET music_cards=music_cards+%d WHERE uid='%d'", cards, GetPlayerUID(playerid));
            mysql_query(mariadb, msg, false);
            format(msg, sizeof msg, "兑换成功, 你兑换了 %d张点歌卡", cards);
        }
        DIALOG_node(playerid, msg);
        format(msg, sizeof msg, "UPDATE server_cdk SET status='1' WHERE id='%d'", id);
        mysql_query(mariadb, msg, false);
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_XGMZ) {
        if(!response) return 1;
        if(userInfo[playerid][OnlineTime] < XGMZ_TIME) {
            SCM(playerid, COLOR_RED, "[系统] 你不可以使用该功能, 原因:你的在线时间不够.");
            return 1;
        }
        if(strcmp(inputtext, defaultIT) == 0) {
            showXGMZ(playerid, "新名字不能为空");
            return 1;
        }
        if(strlen(inputtext) < 5) {
            showXGMZ(playerid, "新名字不能小于5字符");
            return 1;
        }
        new msg[128];
        if(strlen(inputtext) >= 20) {
            format(msg, sizeof msg, "新名字不能大于%d字符", 20);
            showXGMZ(playerid, msg);
            return 1;
        }
        if(sqlInject(inputtext) == 0) {
            showXGMZ(playerid, "新名字不能带有特殊字符");
            return 1;
        }
        if(accountIsNameExist(inputtext) != 0) {
            showXGMZ(playerid, "该新名字已被注册");
            return 1;
        }
        new oldname[MAX_PLAYER_NAME];
        format(oldname, sizeof oldname, GetName(playerid));
        format(msg, sizeof msg, "UPDATE account SET name='%s' WHERE uid='%d'", inputtext, userInfo[playerid][UID]);
        mysql_query(mariadb, msg, false);
        FRIEND_changeNtoN(oldname, inputtext);
        PRace_changeNtoN(oldname, inputtext);
        PHouse_ChangePlayerNameToName(oldname, inputtext);
        team_changeNtoN(oldname, inputtext, playerid);
        tele_changeNtoN(oldname, inputtext);
        AC_changeNtoN(oldname, inputtext);
		BillBoard_changeNtoN(playerid, inputtext);
        log_changename(oldname, inputtext, GetIp(playerid), userInfo[playerid][UID]);
        format(msg, sizeof msg, "修改成功, 新名字为 %s 请牢记\n请重新进入游戏", inputtext);
        DIALOG_node(playerid, msg);
        KickEx(playerid, "改名", 500);
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_XGMM_OLD) {
        if(!response) return 1;
        if(userInfo[playerid][OnlineTime] < XGMM_TIME) {
            SCM(playerid, COLOR_RED, "[系统] 你不可以使用该功能, 原因:你的在线时间不够.");
            return 1;
        }
        if(strcmp(inputtext, defaultIT) == 0) {
            showXGMM(playerid, "当前密码不能为空");
            return 1;
        }
        if(password2etect(GetName(playerid), inputtext) == 0) {
            showXGMM(playerid, "错误的当前密码");
            return 1;
        }
        ShowPlayerDialog(playerid, DIALOG_ACCOUNT_XGMM_NEW, DIALOG_STYLE_INPUT, "修改密码", "请输入新密码", "更改", "关闭");
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_XGMM_NEW) {
        if(!response) return 1;
        if(strcmp(inputtext, defaultIT) == 0) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_XGMM_NEW, DIALOG_STYLE_INPUT, "修改密码", "请输入新密码\n{FF0000}新密码不能为空", "更改", "关闭");
            return 1;
        }
        if(strlen(inputtext) < 3 || strlen(inputtext) > 20) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_XGMM_NEW, DIALOG_STYLE_INPUT, "修改密码", "请输入新密码\n{FF0000}密码长度3-20", "更改", "关闭");
            return 1;
        }
        if(charCheck(inputtext) != 1) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_XGMM_NEW, DIALOG_STYLE_INPUT, "修改密码", "请输入新密码\n{FF0000}密码只能用 A-Z a-z 0-9 组合", "更改", "关闭");
            return 1;
        }
        new msg[256];
        format(msg, sizeof msg, "UPDATE account SET password=MD5('%s') WHERE uid='%d'", inputtext, userInfo[playerid][UID]);
        mysql_query(mariadb, msg, false);
        format(msg, sizeof msg, "新密码为 %s 请牢记", inputtext);
        DIALOG_node(playerid, msg);
        log_changepasswrd(userInfo[playerid][UID], GetName(playerid), GetIp(playerid));
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_CHATTAIL_TEXT) {
        if(!response) {
            showChatTail(playerid);
            return 1;
        }
        if(strcmp(inputtext, defaultIT) == 0) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL_TEXT, DIALOG_STYLE_INPUT, "聊天小尾巴", "请输入小尾巴内容\n{FF0000}小尾巴内容不能为空", "设置", "返回");
            return 1;
        }
        new msg[256];
        if(strlen(inputtext) > MAX_CHATTAIL_TEXT_LEN) {
            format(msg, sizeof msg, "请输入小尾巴内容\n{FF0000}小尾巴内容长度不能大于%d字符.", MAX_CHATTAIL_TEXT_LEN);
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL_TEXT, DIALOG_STYLE_INPUT, "聊天小尾巴", msg, "设置", "返回");
            return 1;
        }
        format(userInfo[playerid][ChatTail], MAX_CHATTAIL_LEN, "%d %s %s", strval(GetChatTail(playerid, 0)), GetChatTail(playerid, 1), inputtext);
        format(msg, sizeof msg, "UPDATE account SET chattail='%s' WHERE uid='%d'", userInfo[playerid][ChatTail], userInfo[playerid][UID]);
        mysql_query(mariadb, msg, false);
        format(msg, sizeof msg, "[系统] 已把聊天小尾巴设置为 %s", inputtext);
        SCM(playerid, COLOR_WHITE, msg);
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_CHATTAIL_COLOR) {
        if(!response) {
            showChatTail(playerid);
            return 1;
        }
        if(strcmp(inputtext, defaultIT) == 0) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL_COLOR, DIALOG_STYLE_INPUT, "聊天小尾巴", "请输入颜色RGB\n{FF0000}颜色不能为空", "设置", "返回");
            return 1;
        }
        if(strlen(inputtext) > 6) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL_COLOR, DIALOG_STYLE_INPUT, "聊天小尾巴", "请输入颜色RGB\n{FF0000}颜色长度错误", "设置", "返回");
            return 1;
        }
        new msg[256];
        format(userInfo[playerid][ChatTail], MAX_CHATTAIL_LEN, "%d %s %s", strval(GetChatTail(playerid, 0)), inputtext, GetChatTail(playerid, 2));
        format(msg, sizeof msg, "UPDATE account SET chattail='%s' WHERE uid='%d'", userInfo[playerid][ChatTail], userInfo[playerid][UID]);
        mysql_query(mariadb, msg, false);
        format(msg, sizeof msg, "[系统] 已把聊天小尾巴颜色设置为 {%s}██████", inputtext);
        SCM(playerid, COLOR_WHITE, msg);
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_CHATTAIL) {
        if(!response) return 1;
        if(strcmp(GetChatTail(playerid, 2), "0") == 0) {
            if(GetMoney(playerid) < CHATTAIL_MONEY) {
                SCM(playerid, COLOR_RED, "[系统] 你的钱不够.");
                return 1;
            }
            new msg[128];
            if(userInfo[playerid][OnlineTime] < CHATTAIL_TIME) {
                format(msg, sizeof msg, "[系统] 你的时间不够, 购买小尾巴需要在线 %d 分钟", CHATTAIL_TIME);
                SCM(playerid, COLOR_RED, msg);
                return 1;
            }
            GiveMoney(playerid, -CHATTAIL_MONEY);
            log_money(GetName(playerid), GetIp(playerid), GetPlayerUID(playerid), "chattail", "chattail", 0, -CHATTAIL_MONEY, "购买聊天小尾巴");
            format(userInfo[playerid][ChatTail], MAX_CHATTAIL_LEN, "1 FFFFFF 新狂野之城");
            format(msg, sizeof msg, "UPDATE account SET chattail='%s' WHERE uid='%d'", "新狂野之城", userInfo[playerid][UID]);
            mysql_query(mariadb, msg, false);
            SCM(playerid, COLOR_WHITE, "[系统] 聊天小尾巴购买成功, 你可以再次输入'/chattail'进行编辑.");
            return 1;
        } else {
            if(listitem == 0) {
                ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL_TEXT, DIALOG_STYLE_INPUT, "聊天小尾巴", "请输入小尾巴内容", "设置", "返回");
            } else if(listitem == 1) {
                ShowPlayerDialog(playerid, DIALOG_ACCOUNT_CHATTAIL_COLOR, DIALOG_STYLE_INPUT, "聊天小尾巴", "请输入颜色RGB", "设置", "返回");
            } else if(listitem == 2) {
                new msg[256];
                if(strval(GetChatTail(playerid, 0)) == 0) {
                    SCM(playerid, COLOR_WHITE, "[系统] 聊天小尾巴已设置为 {00FF00}显示");
                    format(userInfo[playerid][ChatTail], MAX_CHATTAIL_LEN, "1 %s %s", GetChatTail(playerid, 1), GetChatTail(playerid, 2));
                } else {
                    SCM(playerid, COLOR_WHITE, "[系统] 聊天小尾巴已设置为 {FF0000}隐藏");
                    format(userInfo[playerid][ChatTail], MAX_CHATTAIL_LEN, "0 %s %s", GetChatTail(playerid, 1), GetChatTail(playerid, 2));
                }
                format(msg, sizeof msg, "UPDATE account SET chattail='%s' WHERE uid='%d'", userInfo[playerid][ChatTail], userInfo[playerid][UID]);
                mysql_query(mariadb, msg, false);
            }
            return 1;
        }
    }
    if(dialogid == DIALOG_ACCOUNT_SZ) {
        if(!response) return 1;
        new msg[128];
        if(listitem == 11) {
            userInfo[playerid][SZ_Value][listitem] = (userInfo[playerid][SZ_Value][listitem] == 0 ? (1) : (0));
            accountUpdateSZValue(playerid);
            if(userInfo[playerid][SZ_Value][listitem] == 0) {
                DIALOG_node(playerid, "你把出生点改为了系统默认");
            } else {
                new str[256], Float:x, Float:y, Float:z;
                GetPlayerPos(playerid, x, y, z);
                format(userInfo[playerid][SpawnPos], MAX_SPAWN_POS_LEN, "%.2f %.2f %.2f %.2f", x, y, z, GetPlayerAngle(playerid));
                format(str, sizeof str, "UPDATE account SET spawn_pos='%s' WHERE uid='%d'", userInfo[playerid][SpawnPos], userInfo[playerid][UID]);
                mysql_query(mariadb, str, false);                
                DIALOG_node(playerid, "你把出生点改为了自定义(下次出生将会在此)");
            }
        } else if(listitem < 5 || listitem > 6) {
			userInfo[playerid][SZ_Value][listitem] = (userInfo[playerid][SZ_Value][listitem] == 0 ? (1) : (0));
            accountUpdateSZValue(playerid);
			if(listitem == 1) {
				SetPlayerHealth(playerid, 100);
			} else if(listitem == 3) {
                SetPlayerTimeEx(playerid);
            } else if(listitem == 4) {
                SetPlayerWeatherEx(playerid);
            } else if(listitem == 8) {
				configKGOBJ(playerid);
			} else if(listitem == 12) {
				if(GetPlayerTRTimeStatus(playerid) == 1) {
                    PlayerTextDrawSetString(playerid, userInfo[playerid][TRTime], "_");
                } else {
                    SetPlayerTimeEx(playerid);
                }
			} else if(listitem == 13) {
                DisableRemoteVehicleCollisions(playerid, userInfo[playerid][SZ_Value][listitem]);
            }
            pc_cmd_sz(playerid, "ojbk");
        } else if(listitem == 5) {
            format(msg, sizeof msg, "当前锁定时间为:%d (请输入时间0-23)", userInfo[playerid][SZ_Value][5]);
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ_TIME, DIALOG_STYLE_INPUT, "自定义设置-时间", msg, "设置", "关闭");
        } else if(listitem == 6) {
            new str[2048];
            for(new i = 0; i < sizeof GetWeatherNames; i ++) {
                format(str, sizeof str, "%s%d - %s\n", str, i, GetWeatherNames[i]);
            }
            format(str, sizeof str, "%s当前锁定天气ID为:%d (请输入天气ID0-20)", str, userInfo[playerid][SZ_Value][6]);
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ_WEATHER, DIALOG_STYLE_INPUT, "自定义设置-天气", str, "设置", "关闭");
        }
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_SZ_WEATHER) {
        if(!response) return 1;
        if(strcmp(inputtext, defaultIT) == 0) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ_WEATHER, DIALOG_STYLE_INPUT, "自定义设置-天气", "{FF0000}天气不能为空", "设置", "关闭");
            return 1;
        }
        new weather = strval(inputtext);
        if(weather < 0 || weather > 20) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ_WEATHER, DIALOG_STYLE_INPUT, "自定义设置-天气", "{FF0000}天气ID范围0-20", "设置", "关闭");
            return 1;
        }
        userInfo[playerid][SZ_Value][6] = weather;
        accountUpdateSZValue(playerid);
        SetPlayerWeatherEx(playerid, weather);
        DIALOG_node(playerid, "天气修改成功");
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_SZ_TIME) {
        if(!response) return 1;
        if(strcmp(inputtext, defaultIT) == 0) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ_TIME, DIALOG_STYLE_INPUT, "自定义设置-时间", "{FF0000}时间不能为空", "设置", "关闭");
            return 1;
        }
        new time = strval(inputtext);
        if(time < 0 || time > 23) {
            ShowPlayerDialog(playerid, DIALOG_ACCOUNT_SZ_TIME, DIALOG_STYLE_INPUT, "自定义设置-时间", "{FF0000}时间范围0-23", "设置", "关闭");
            return 1;
        }
        userInfo[playerid][SZ_Value][5] = time;
        accountUpdateSZValue(playerid);
        SetPlayerTimeEx(playerid, time);
        DIALOG_node(playerid, "时间修改成功");
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_WUQI) {
        if(!response) return 1;
        showDialogWeapon(playerid, listitem);
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_LOGIN) {
        if(!response) {
            KickEx(playerid, "登录时退出");
            return 1;
        }
        if(strcmp(inputtext, defaultIT) == 0) {
            showLogin(playerid, "密码不能为空");
            return 1;
        }
        if(password2etect(GetName(playerid), inputtext) == 0) {
            SCM(playerid, COLOR_RED, "[系统] 你被踢出了服务器, 原因:密码错误.");
            KickEx(playerid, "密码错误");
        } else {
            OnPlayerLogin(playerid);
        }
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_REG_CONFIRM) {
        OnPlayerLogin(playerid);
        return 1;
    }
    if(dialogid == DIALOG_ACCOUNT_REG) {
        if(!response) {
            KickEx(playerid, "注册时退出");
            return 1;
        }
        if(strcmp(inputtext, defaultIT) == 0) {
            showReg(playerid, "密码不能为空");
            return 1;
        }
        if(strlen(inputtext) < 3 || strlen(inputtext) > 20) {
            showReg(playerid, "密码长度3-20");
            return 1;
        }
        if(charCheck(inputtext) != 1) {
            showReg(playerid, "密码只能用 A-Z a-z 0-9 组合");
            return 1;
        }
        new msg[2048], ip[MAX_PLAYER_IP_LEN];
        GetPlayerIp(playerid, ip, sizeof ip);
        format(msg, sizeof msg, "INSERT INTO account (name,password,reg_time,reg_ip,savings) VALUES ('%s', MD5('%s'),'%d','%s','%d')", GetName(playerid), inputtext, gettime(), ip, REG_GIVE_SAVINGS);
        mysql_query(mariadb, msg, false);
        new Cache:result, uid, query[128];
        format(query, sizeof query, "SELECT uid FROM account WHERE name = '%s'", GetName(playerid));
        result = mysql_query(mariadb, query);
        cache_get_value_name_int(0, "uid", uid);
        cache_delete(result);
        format(msg, sizeof msg, "\
        {FFFFFF}注册账户: {FF0000}%s\n\
        {FFFFFF}注册密码: {FF0000}%s {FFFF00}(请牢记您注册的密码)\n\
        {FFFFFF}注册时间: {00ECFF}今天\n\
        {FFFFFF}账户PID: {00ECFF}%d {FFFF00}(重要){FFFFFF}\n\
        %s", GetName(playerid), inputtext, uid, serverRules);
        ShowPlayerDialog(playerid, DIALOG_ACCOUNT_REG_CONFIRM, DIALOG_STYLE_MSGBOX, " ", msg, "了解", "");
        return 1;
    }
    return 1;
}

forward OnPlayerLogin(playerid);
