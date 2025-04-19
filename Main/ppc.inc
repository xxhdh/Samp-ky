#define PPC_FREEZE_TIME       (10)
#define PPC_OBJECT_DROPTIME   (8) 
#define PPC_LOWEST_HEIGHT     (5.2)
#define MAX_PPC_OBJECTS       (121)

new ppc_rules[][128] = {
  "[PPC] 请勿在游戏中使用CLEO,外挂等软件,否则将会被封号",
  "[PPC] 碰碰车游戏规则:",
  "[PPC] 开始时,各位玩家将随机分配到车辆,开始后每隔4秒将会有三块板子掉落,掉落前,板子会有晃动提示",
  "[PPC] 想不被淘汰,那就赶紧找到稳定的板子吧.比赛结束,可以输入/ppc exit 来离开比赛",
  "[PPC] 已进入倒计时, 请等待比赛开始!"
};

new ppc_timer;
new ppc_status = 0; // 0 = 未开启, 1 = 已开启, 2 = 倒计时中, 3 = 发车, 4 = 比赛中
new ppc_countdown;
new ppc_dropcd = 0;
new ppc_vehs[] = {407, 406, 451, 416, 431, 457, 461, 481, 495, 514, 510, 532, 524, 530, 541, 568, 572, 557, 583, 578, 596, 601, 574, 560}; //随机载具列表
new ppc_players[MAX_PLAYERS];

enum ppcobjectsstruct {
  Id,
  Shake
};

new ppc_objects[MAX_PPC_OBJECTS][ppcobjectsstruct], ppc_valid_objs[MAX_PPC_OBJECTS], ppc_max_valid;

enum ppcspawnstruct {
    Float:X,
    Float:Y,
    Float:Z,
    Float:A
};

new ppc_spawnpos[][ppcspawnstruct] = {
  // {x, y, z, a}
  {1440.0431, -4042.2747, 12.0000, 0.0000},
  {1446.3936, -4042.2004, 12.0000, 0.0000},
  {1452.5046, -4042.3943, 12.0000, 0.0000},
  {1458.9154, -4042.4817, 12.0000, 0.0000},
  {1465.5645, -4042.4111, 12.0000, 0.0000},
  {1471.3910, -4042.2625, 12.0000, 0.0000},
  {1477.3131, -4042.2378, 12.0000, 0.0000},
  {1478.5629, -4023.6040, 12.0000, 0.0000},
  {1472.8219, -4023.5740, 12.0000, 0.0000},
  {1466.2646, -4023.3245, 12.0000, 0.0000},
  {1459.1835, -4023.2419, 12.0000, 0.0000},
  {1452.3954, -4022.6643, 12.0000, 0.0000},
  {1445.8988, -4022.4163, 12.0000, 0.0000},
  {1439.6926, -4022.5740, 12.0000, 0.0000},
  {1439.9608, -4005.7126, 12.0000, 0.0000},
  {1446.4218, -4005.9153, 12.0000, 0.0000},
  {1453.1368, -4005.2297, 12.0000, 0.0000},
  {1460.0383, -4005.6797, 12.0000, 0.0000},
  {1466.2972, -4005.3440, 12.0000, 0.0000},
  {1472.8932, -4005.5647, 12.0000, 0.0000}
};

stock isPlayerInPPC(playerid) {
  if(ppc_players[playerid] == playerid) {
    return 1;
  }
  return 0;
}

CMD:ppc(playerid, params[]) {
  new msg[128];
  if(strcmp(params, "open", true) == 0) {
    if(GetPlayerAccess(playerid, "gm_2") == 0) return DIALOG_node(playerid, "你没有权限使用该命令");
    if(GetGMStatus(playerid) == 0) return DIALOG_node(playerid, "当前不可以使用该命令, 原因:你没有进入执勤状态.");
    if(ppc_status != 0) {
      DIALOG_node(playerid, "PPC已经开启了!");
      return 1;
    }
    for(new i = 0; i < MAX_PLAYERS; i ++) {
      ppc_players[i] = -1;
    }
    ppc_status = 1;
    format(msg, sizeof msg, "[系统] 管理员 %s 开启了碰碰车, 输入'/ppc'可加入!", GetNameEx(playerid));
    SCMToAll(COLOR_RED, msg);
    DIALOG_node(playerid, "输入 /ppc start 可开始");
    return 1;
  } else if(strcmp(params, "start", true) == 0) {
    if(GetPlayerAccess(playerid, "gm_2") == 0) return DIALOG_node(playerid, "你没有权限使用该命令");
    if(GetGMStatus(playerid) == 0) return DIALOG_node(playerid, "当前不可以使用该命令, 原因:你没有进入执勤状态.");
    if(ppc_status == 0) return DIALOG_node(playerid, "PPC都没开启");
    if(ppc_status == 2) return DIALOG_node(playerid, "PPC已进入倒计时");
    if(ppc_status == 3 && ppc_status == 4) return DIALOG_node(playerid, "PPC已在运行");
    new rules_len = sizeof ppc_rules, spawn_len = sizeof ppc_spawnpos;
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
      if(ppc_players[i] == i) {
        for(new r = 0; r < rules_len; r ++) {
          SCM(i, COLOR_BLUE, ppc_rules[r]);
        }
        new sid = random(spawn_len);
        SetPlayerPos(i, ppc_spawnpos[sid][X], ppc_spawnpos[sid][Y], ppc_spawnpos[sid][Z]);
        SetPlayerFacingAngle(i, ppc_spawnpos[sid][A]);
        TogglePlayerControllable(i, 0);
      }
    }
    ppc_countdown = PPC_FREEZE_TIME;
    ppc_status = 2;
    ppc_timer = SetTimer("onPPCTimer", 1000, true);
    PPCInit();
    return 1;
  } else if(strcmp(params, "close", true) == 0) {
    if(GetPlayerAccess(playerid, "gm_2") == 0) return DIALOG_node(playerid, "你没有权限使用该命令");
    if(GetGMStatus(playerid) == 0) return DIALOG_node(playerid, "当前不可以使用该命令, 原因:你没有进入执勤状态.");
    if(ppc_status != 4) return DIALOG_node(playerid, "PPC只能在开始后才能关闭!");
    ppc_status = 0;
    if(IsValidTimer(ppc_timer)) {
      KillTimer(ppc_timer);
		}
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
      if(ppc_players[i] == i) {
        ppc_players[i] = -1;
        TogglePlayerSpectating(i, 0);
        SpawnPlayer(i);
      }
    }
    SCM(playerid, COLOR_RED, "[系统] 你关闭了PPC");
    return 1;
  } else if(strcmp(params, "exit", true) == 0) {
    if(ppc_status == 0) {
      DIALOG_node(playerid, "碰碰车都没开启, 不需要退出!");
      return 1;
    }
    if(ppc_players[playerid] == playerid) {
      ppc_players[playerid] = -1;
      TogglePlayerSpectating(playerid, 0);
      SpawnPlayer(playerid);
      DIALOG_node(playerid, "你退出了碰碰车!");
    } else {
      DIALOG_node(playerid, "你都没加入!");
    }
    return 1;
  }
  if(ppc_status == 0) {
    SCM(playerid, COLOR_RED, "[PPC] 碰碰车都没开启呢!");
    return 1;
  }
  if(ppc_status > 1) {
    SCM(playerid, COLOR_RED, "[PPC] 当前已经不可加入了!");
    return 1;
  }
  if(ppc_players[playerid] == playerid) {
    DIALOG_node(playerid, "你已经加入了, 请等待开始!");
    return 1;
  }
  format(msg, sizeof msg, "[系统] %s 加入了碰碰车!输入/ppc 加入碰碰车比赛", GetNameEx(playerid));
  SCMToAll(COLOR_RED, msg);
  ppc_players[playerid] = playerid;
  return 1;
}

stock PPCPlayerInit(playerid) {
  ppc_players[playerid] = -1;
}

stock PPCCountdown() {
  new msg[12];
  if(ppc_countdown == 0) {
    format(msg, sizeof msg, "~g~GO!!!");
    ppc_status = 3;
  } else {
    format(msg, sizeof msg, "~r~%d", ppc_countdown);
    ppc_countdown --;
  }
  for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
    if(IsPlayerConnected(i) == 1 && ppc_players[i] == i) {
      GameTextForPlayer(i, msg, 888, 3);
    }
  }
}

forward onPPCTimer();
public onPPCTimer() {
  if(ppc_status == 0) {
    if(IsValidTimer(ppc_timer)) {
      KillTimer(ppc_timer);
		}
  } else if(ppc_status == 2) {
    PPCCountdown();
  } else if(ppc_status == 3) {
    ppc_dropcd = PPC_OBJECT_DROPTIME;
    new car[MAX_PLAYER_NAME], lens = sizeof ppc_vehs;
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
      if(ppc_players[i] == i) {
        format(car, sizeof car, "%d", ppc_vehs[random(lens)]);
        pc_cmd_c(i, car);
        TogglePlayerControllable(i, 1);
      }
    }
    ppc_status = 4;
    SCMToAll(COLOR_BLUE, "[PPC] 正式开始！撞死这些傻逼吧！");
  } else if(ppc_status == 4) {
    ppc_dropcd --;
    if(ppc_dropcd <= 0) {
      ppc_dropcd = PPC_OBJECT_DROPTIME;
      PPCShakeObject();
      PPCShakeObject();
      PPCShakeObject();
    }
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
      if(ppc_players[i] == i) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(i, x, y, z);
        if(z < PPC_LOWEST_HEIGHT) {
          TogglePlayerSpectating(i, 1);
          PlayerSpectateVehicle(i, GetPlayerVehicleID(GetRandomAlivePPCPlayerId(i)));
          temporary_vehicleInit(i);
        }
      }
    }
  }
  return 1;
}

stock PPCShakeObject() {
  if(ppc_max_valid != 0) {
    new id = random(ppc_max_valid);
    new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
    GetDynamicObjectPos(ppc_objects[ppc_valid_objs[id]][Id], x, y, z);
    GetDynamicObjectRot(ppc_objects[ppc_valid_objs[id]][Id], rx, ry, rz);
    MoveDynamicObject(ppc_objects[ppc_valid_objs[id]][Id], x+0.2, y+0.2, z+0.2, 0.2, 0.0, ry, rz);
    ppc_objects[ppc_valid_objs[id]][Shake] = 1;
    ppc_valid_objs[id] = ppc_valid_objs[ppc_max_valid];
    ppc_max_valid --;
  }
}

stock PPCObjectMoved(objectid) {
  new id = -1;
  for(new i = 0; i < MAX_PPC_OBJECTS; i ++) {
    if(ppc_objects[i][Id] == objectid) {
      id = i;
      break;
    }
  }
  if(id != -1) {
    if(ppc_objects[id][Shake] == 1) {
      new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
      GetDynamicObjectPos(ppc_objects[id][Id], x, y, z);
      GetDynamicObjectRot(ppc_objects[id][Id], rx, ry, rz);
      MoveDynamicObject(ppc_objects[id][Id], x+0.2, y+0.2, z+0.2, 0.2, 55.0, ry, rz);
      ppc_objects[id][Shake] = 2;
    } else if(ppc_objects[id][Shake] == 2) {
      new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
      GetDynamicObjectPos(ppc_objects[id][Id], x, y, z);
      GetDynamicObjectRot(ppc_objects[id][Id], rx, ry, rz);
      MoveDynamicObject(ppc_objects[id][Id], x+0.2, y+0.2, -3.0, 5.0, rx, ry, rz);
      ppc_objects[id][Shake] = 3;
    } else if(ppc_objects[id][Shake] == 3) {
      if(IsValidDynamicObject(ppc_objects[id][Id])) {
        DestroyDynamicObject(ppc_objects[id][Id]);
      }
    }
  }
}

stock GetRandomAlivePPCPlayerId(playerid) {
  new alives[MAX_PLAYERS], maxids = 0;
  for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) {
    if(ppc_players[i] == i && i != playerid) {
      alives[maxids] = i;
      maxids ++;
    }
  }
  new targetid = alives[random(maxids)];
  return targetid;
}

stock PPC_OnPlayerExitVehicle(playerid) {
  if(ppc_players[playerid] == playerid) {
    TogglePlayerSpectating(playerid, 1);
    PlayerSpectateVehicle(playerid, GetPlayerVehicleID(GetRandomAlivePPCPlayerId(playerid)));
    temporary_vehicleInit(playerid);
  }
}

stock PPCInit() {
  for(new i = 0; i < MAX_PPC_OBJECTS; i ++) {
    if(IsValidDynamicObject(ppc_objects[i][Id])) {
      DestroyDynamicObject(ppc_objects[i][Id]);
    }
  }
  ppc_objects[0][Id] = CreateDynamicObject(1697, 1482.15, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[1][Id] = CreateDynamicObject(1697, 1482.14, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[2][Id] = CreateDynamicObject(1697, 1482.14, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[3][Id] = CreateDynamicObject(1697, 1482.17, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[4][Id] = CreateDynamicObject(1697, 1482.19, -3994.65, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[5][Id] = CreateDynamicObject(1697, 1477.73, -3994.65, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[6][Id] = CreateDynamicObject(1697, 1473.26, -3994.63, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[7][Id] = CreateDynamicObject(1697, 1468.82, -3994.62, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[8][Id] = CreateDynamicObject(1697, 1464.33, -3994.60, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[9][Id] = CreateDynamicObject(1697, 1459.86, -3994.58, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[10][Id] = CreateDynamicObject(1697, 1455.40, -3994.58, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[11][Id] = CreateDynamicObject(1697, 1446.47, -3994.60, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[12][Id] = CreateDynamicObject(1697, 1450.93, -3994.58, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[13][Id] = CreateDynamicObject(1697, 1442.00, -3994.60, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[14][Id] = CreateDynamicObject(1697, 1437.54, -3994.60, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[15][Id] = CreateDynamicObject(1697, 1477.72, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[16][Id] = CreateDynamicObject(1697, 1473.26, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[17][Id] = CreateDynamicObject(1697, 1468.80, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[18][Id] = CreateDynamicObject(1697, 1464.33, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[19][Id] = CreateDynamicObject(1697, 1459.86, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[20][Id] = CreateDynamicObject(1697, 1455.40, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[21][Id] = CreateDynamicObject(1697, 1450.93, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[22][Id] = CreateDynamicObject(1697, 1446.47, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[23][Id] = CreateDynamicObject(1697, 1442.00, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[24][Id] = CreateDynamicObject(1697, 1437.54, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[25][Id] = CreateDynamicObject(1697, 1477.72, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[26][Id] = CreateDynamicObject(1697, 1473.26, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[27][Id] = CreateDynamicObject(1697, 1468.80, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[28][Id] = CreateDynamicObject(1697, 1464.33, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[29][Id] = CreateDynamicObject(1697, 1459.86, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[30][Id] = CreateDynamicObject(1697, 1455.40, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[31][Id] = CreateDynamicObject(1697, 1450.93, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[32][Id] = CreateDynamicObject(1697, 1446.47, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[33][Id] = CreateDynamicObject(1697, 1442.00, -4005.35, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[34][Id] = CreateDynamicObject(1697, 1437.54, -3999.99, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[35][Id] = CreateDynamicObject(1697, 1477.72, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[36][Id] = CreateDynamicObject(1697, 1473.26, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[37][Id] = CreateDynamicObject(1697, 1468.80, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[38][Id] = CreateDynamicObject(1697, 1464.33, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[39][Id] = CreateDynamicObject(1697, 1459.86, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[40][Id] = CreateDynamicObject(1697, 1455.40, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[41][Id] = CreateDynamicObject(1697, 1450.93, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[42][Id] = CreateDynamicObject(1697, 1446.47, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[43][Id] = CreateDynamicObject(1697, 1442.00, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[44][Id] = CreateDynamicObject(1697, 1437.54, -4010.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[45][Id] = CreateDynamicObject(1697, 1477.72, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[46][Id] = CreateDynamicObject(1697, 1473.26, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[47][Id] = CreateDynamicObject(1697, 1468.80, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[48][Id] = CreateDynamicObject(1697, 1464.33, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[49][Id] = CreateDynamicObject(1697, 1459.86, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[50][Id] = CreateDynamicObject(1697, 1455.40, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[51][Id] = CreateDynamicObject(1697, 1450.93, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[52][Id] = CreateDynamicObject(1697, 1446.47, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[53][Id] = CreateDynamicObject(1697, 1442.00, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[54][Id] = CreateDynamicObject(1697, 1437.54, -4016.04, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[55][Id] = CreateDynamicObject(1697, 1482.15, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[56][Id] = CreateDynamicObject(1697, 1477.72, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[57][Id] = CreateDynamicObject(1697, 1473.26, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[58][Id] = CreateDynamicObject(1697, 1468.80, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[59][Id] = CreateDynamicObject(1697, 1464.33, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[60][Id] = CreateDynamicObject(1697, 1459.86, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[61][Id] = CreateDynamicObject(1697, 1455.40, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[62][Id] = CreateDynamicObject(1697, 1450.93, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[63][Id] = CreateDynamicObject(1697, 1446.47, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[64][Id] = CreateDynamicObject(1697, 1442.00, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[65][Id] = CreateDynamicObject(1697, 1437.54, -4021.36, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[66][Id] = CreateDynamicObject(1697, 1482.15, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[67][Id] = CreateDynamicObject(1697, 1477.72, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[68][Id] = CreateDynamicObject(1697, 1473.26, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[69][Id] = CreateDynamicObject(1697, 1468.80, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[70][Id] = CreateDynamicObject(1697, 1464.33, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[71][Id] = CreateDynamicObject(1697, 1459.86, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[72][Id] = CreateDynamicObject(1697, 1455.40, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[73][Id] = CreateDynamicObject(1697, 1450.93, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[74][Id] = CreateDynamicObject(1697, 1446.47, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[75][Id] = CreateDynamicObject(1697, 1442.00, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[76][Id] = CreateDynamicObject(1697, 1437.54, -4026.71, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[77][Id] = CreateDynamicObject(1697, 1482.15, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[78][Id] = CreateDynamicObject(1697, 1477.72, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[79][Id] = CreateDynamicObject(1697, 1473.26, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[80][Id] = CreateDynamicObject(1697, 1468.80, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[81][Id] = CreateDynamicObject(1697, 1464.33, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[82][Id] = CreateDynamicObject(1697, 1459.86, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[83][Id] = CreateDynamicObject(1697, 1455.40, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[84][Id] = CreateDynamicObject(1697, 1450.93, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[85][Id] = CreateDynamicObject(1697, 1446.47, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[86][Id] = CreateDynamicObject(1697, 1442.00, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[87][Id] = CreateDynamicObject(1697, 1437.54, -4032.07, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[88][Id] = CreateDynamicObject(1697, 1482.15, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[89][Id] = CreateDynamicObject(1697, 1477.72, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[90][Id] = CreateDynamicObject(1697, 1473.26, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[91][Id] = CreateDynamicObject(1697, 1468.80, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[92][Id] = CreateDynamicObject(1697, 1464.33, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[93][Id] = CreateDynamicObject(1697, 1459.86, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[94][Id] = CreateDynamicObject(1697, 1455.40, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[95][Id] = CreateDynamicObject(1697, 1450.93, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[96][Id] = CreateDynamicObject(1697, 1446.47, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[97][Id] = CreateDynamicObject(1697, 1442.00, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[98][Id] = CreateDynamicObject(1697, 1437.54, -4037.39, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[99][Id] = CreateDynamicObject(1697, 1482.15, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[100][Id] = CreateDynamicObject(1697, 1477.72, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[101][Id] = CreateDynamicObject(1697, 1473.26, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[102][Id] = CreateDynamicObject(1697, 1468.80, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[103][Id] = CreateDynamicObject(1697, 1464.33, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[104][Id] = CreateDynamicObject(1697, 1459.86, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[105][Id] = CreateDynamicObject(1697, 1455.40, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[106][Id] = CreateDynamicObject(1697, 1450.93, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[107][Id] = CreateDynamicObject(1697, 1446.47, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[108][Id] = CreateDynamicObject(1697, 1442.00, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[109][Id] = CreateDynamicObject(1697, 1437.54, -4042.72, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[110][Id] = CreateDynamicObject(1697, 1482.15, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[111][Id] = CreateDynamicObject(1697, 1477.72, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[112][Id] = CreateDynamicObject(1697, 1473.26, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[113][Id] = CreateDynamicObject(1697, 1468.80, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[114][Id] = CreateDynamicObject(1697, 1464.33, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[115][Id] = CreateDynamicObject(1697, 1459.86, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[116][Id] = CreateDynamicObject(1697, 1455.40, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[117][Id] = CreateDynamicObject(1697, 1450.93, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[118][Id] = CreateDynamicObject(1697, 1446.47, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[119][Id] = CreateDynamicObject(1697, 1442.00, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_objects[120][Id] = CreateDynamicObject(1697, 1437.54, -4048.06, 10.00,   31.80, 0.00, 0.00);
  ppc_max_valid = 120;
  for(new i = 0; i < MAX_PPC_OBJECTS; i ++) {
    ppc_valid_objs[i] = i;
    ppc_objects[i][Shake] = 0;
  }
}