#define DIALOG_ATTIRE                   (1200)
#define MAX_ATTIRE_MODELNAME_LEN        (50)
#define MAX_PLAYER_ATTIRE               (20)
#define DIALOG_ATTIRE_MAIN              DIALOG_ATTIRE + 1
#define DIALOG_ATTIRE_DETAILS           DIALOG_ATTIRE + 2
#define DIALOG_ATTIRE_BONE_CHANGE       DIALOG_ATTIRE + 3

enum attirestruct {
  Id,
  UId,
  ModelName[MAX_ATTIRE_MODELNAME_LEN],
  ModelId,
  BoneId,
  Float:OffsetX,
  Float:OffsetY,
  Float:OffsetZ,
  Float:RotX,
  Float:RotY,
  Float:RotZ,
  Float:ScaleX,
  Float:ScaleY,
  Float:ScaleZ,
  Status // 0 = 未装扮, 1 = 装扮中
};

enum playerattirestruct {
  Slot[MAX_PLAYER_ATTACHED_OBJECTS],
  SelectId
};

new AttireInfo[MAX_PLAYERS][MAX_PLAYER_ATTIRE][attirestruct];
new pAttire[MAX_PLAYERS][playerattirestruct];


new AttireBoneNames[19][12] = {
	" ", "脊椎", "头", "左上臂", "右上臂", "左手", "右手", "左大腿", "右大腿", "左脚", "右脚", "右小腿", "左小腿", "左前臂", "右前臂", "左锁骨", "右锁骨", "脖子", "下巴"
};

new AttireStatusNames[2][MAX_PLAYER_NAME] = {
	"{CFCFCF}未装扮", "{00FF7F}装扮中"
};

stock Attire.initSlot(playerid) {
  CancelEdit(playerid);
  for(new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i ++) {
    pAttire[playerid][Slot][i] = -1;
  }
  pAttire[playerid][SelectId] = -1;
}

stock Attire.load(playerid) {
	for(new i = 0; i < MAX_PLAYER_ATTIRE; i ++) {
		AttireInfo[playerid][i][Id] = 0;
	}
	new msg[128], Cache:result, count, myname[MAX_PLAYER_NAME];
	format(myname, sizeof myname, "%s", GetName(playerid));
	format(msg, sizeof msg, "SELECT * FROM attire WHERE uid='%d' AND created_at=deleted_at", GetPlayerUID(playerid));
	result = mysql_query(mariadb, msg);
	cache_get_row_count(count);
	for(new i = 0; i < count; i ++) {
		if(i < MAX_PLAYER_ATTIRE) {
      cache_get_value_name(i, "model_name", AttireInfo[playerid][i][ModelName], MAX_ATTIRE_MODELNAME_LEN);
      cache_get_value_name_int(i, "model_id", AttireInfo[playerid][i][ModelId]);
      cache_get_value_name_int(i, "bone_id", AttireInfo[playerid][i][BoneId]);
      cache_get_value_name_float(i, "offset_x", AttireInfo[playerid][i][OffsetX]);
      cache_get_value_name_float(i, "offset_y", AttireInfo[playerid][i][OffsetY]);
      cache_get_value_name_float(i, "offset_z", AttireInfo[playerid][i][OffsetZ]);
      cache_get_value_name_float(i, "rot_x", AttireInfo[playerid][i][RotX]);
      cache_get_value_name_float(i, "rot_y", AttireInfo[playerid][i][RotY]);
      cache_get_value_name_float(i, "rot_z", AttireInfo[playerid][i][RotZ]);
      cache_get_value_name_float(i, "scale_x", AttireInfo[playerid][i][ScaleX]);
      cache_get_value_name_float(i, "scale_y", AttireInfo[playerid][i][ScaleY]);
      cache_get_value_name_float(i, "scale_z", AttireInfo[playerid][i][ScaleZ]);
      cache_get_value_name_int(i, "status", AttireInfo[playerid][i][Status]);
      cache_get_value_name_int(i, "id", AttireInfo[playerid][i][Id]);
    }
	}
	cache_delete(result);
}

stock Attire.Spawn(playerid) {
  Attire.initSlot(playerid);
  for(new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i ++) {
    if(IsPlayerAttachedObjectSlotUsed(playerid, i)) {
      RemovePlayerAttachedObject(playerid, i);
    }
  }
  new slot;
  for(new i = 0; i < MAX_PLAYER_ATTIRE; i ++) {
    if(AttireInfo[playerid][i][Id] != 0 && AttireInfo[playerid][i][Status] == 1) {
      slot = Attire.GetNullSlot(playerid);
      if(slot != -1) {
        Attire.Attach(playerid, slot, i);
      }
    }
  }
}

stock Attire.Attach(playerid, slot, id) {
  pAttire[playerid][Slot][slot] = id;
  SetPlayerAttachedObject(playerid, slot, AttireInfo[playerid][id][ModelId], AttireInfo[playerid][id][BoneId],AttireInfo[playerid][id][OffsetX],
    AttireInfo[playerid][id][OffsetY], AttireInfo[playerid][id][OffsetZ], AttireInfo[playerid][id][RotX], AttireInfo[playerid][id][RotY], 
    AttireInfo[playerid][id][RotZ], AttireInfo[playerid][id][ScaleX], AttireInfo[playerid][id][ScaleY], AttireInfo[playerid][id][ScaleZ], 0, 0);
}

stock Attire.GetNullSlot(playerid) {
  for(new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i ++) {
    if(pAttire[playerid][Slot][i] == -1) {
      return i;
    }
  }
  return -1;
}

stock Attire.Dialog(playerid, err[] = " ") {
  new msg[2048], title[128];
  format(title, sizeof title, "我的装扮 - {FF0000}%s", err);
  format(msg, sizeof msg, "名称\t部位\t状态");
  for(new i = 0; i < MAX_PLAYER_ATTIRE; i ++) {
    if(AttireInfo[playerid][i][Id] == 0) {
      format(msg, sizeof msg, "%s\n \t \t ", msg);
    } else {
      format(msg, sizeof msg, "%s\n%s\t%s\t%s", msg, AttireInfo[playerid][i][ModelName], 
      AttireBoneNames[AttireInfo[playerid][i][BoneId]], AttireStatusNames[AttireInfo[playerid][i][Status]]);
    }
  }
  ShowPlayerDialog(playerid, DIALOG_ATTIRE_MAIN, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "选择", "关闭");
}

stock Attire.Details(playerid , err[] = " ") {
  new id = pAttire[playerid][SelectId];
  if(id != -1) {
    new msg[256], title[128];
    format(title, sizeof title, "我的装扮 - {FF0000}%s", err);
    format(msg, sizeof msg, "____________________|装扮信息|____________________\n");
    format(msg, sizeof msg, "%s装扮名称:%s\n装扮部位:%s\n装扮DBID:%d\n装扮状态:%s\n", 
    msg, AttireInfo[playerid][id][ModelName], AttireBoneNames[AttireInfo[playerid][id][BoneId]],
    AttireInfo[playerid][id][Id], AttireStatusNames[AttireInfo[playerid][id][Status]]);
    format(msg, sizeof msg, "%s____________________|操作编辑|____________________\n", msg);
    format(msg, sizeof msg, "%s编辑位置\n更改部位\n脱下\n穿上\n\n{FF0000}丢掉", msg);
    ShowPlayerDialog(playerid, DIALOG_ATTIRE_DETAILS, DIALOG_STYLE_LIST, title, msg, "选择", "返回");
  }
}

CMD:attire(playerid, params[]) {
  Attire.Dialog(playerid);
  return 1;
}

forward ATTIREODR(playerid, dialogid, response, listitem, inputtext[]);
public ATTIREODR(playerid, dialogid, response, listitem, inputtext[]) {
	if(dialogid == DIALOG_ATTIRE_BONE_CHANGE) {
		if(!response) {
			Attire.Details(playerid);
			return 1;
		}
		new slot = Attire.GetEditId(playerid);
		if(slot == -1) {
			Attire.Details(playerid, "请先穿上");
			return 1;
		} 
		new msg[128], id = pAttire[playerid][SelectId];
		AttireInfo[playerid][id][BoneId] = listitem + 1;
		format(msg, sizeof msg, "UPDATE `attire` SET `bone_id`='%d' WHERE `id`='%d'", AttireInfo[playerid][id][BoneId], AttireInfo[playerid][id][Id]);
		mysql_query(mariadb, msg, false);
		RemovePlayerAttachedObject(playerid, slot);
		Attire.Attach(playerid, slot, id);
		Attire.UpdateData(playerid, id);
		Attire.Details(playerid, "部位更改成功");
		return 1;
  }

	if(dialogid == DIALOG_ATTIRE_DETAILS) {
    if(!response) {
      Attire.Dialog(playerid);
      return 1;
    }
    if(strcmp(inputtext, "编辑位置") == 0) {
      new slot = Attire.GetEditId(playerid);
      if(slot == -1) {
        Attire.Details(playerid, "请先穿上");
      } else {
        EditAttachedObject(playerid, slot);
      }
      return 1;
    } else if(strcmp(inputtext, "更改部位") == 0) {
			new msg[256];
			for(new i = 1; i < sizeof AttireBoneNames; i ++) {
				format(msg, sizeof msg, "%s\n%s", msg, AttireBoneNames[i]);
			}
			ShowPlayerDialog(playerid, DIALOG_ATTIRE_BONE_CHANGE, DIALOG_STYLE_LIST, "我的装扮 - 部位更改", msg, "更改", "返回");
			return 1;
    } else if(strcmp(inputtext, "脱下") == 0) {
      if(AttireInfo[playerid][pAttire[playerid][SelectId]][Status] == 1) {
        Attire.UpdateStatus(playerid, pAttire[playerid][SelectId], 0);
        Attire.Details(playerid);
      } else {
        Attire.Details(playerid, "已经脱下了啊");
      }
    } else if(strcmp(inputtext, "穿上") == 0) {
      if(Attire.GetNullSlot(playerid) == -1) {
        Attire.Details(playerid, "穿不下了!!!");
      } else if(AttireInfo[playerid][pAttire[playerid][SelectId]][Status] == 0) {
        Attire.UpdateStatus(playerid, pAttire[playerid][SelectId], 1);
        Attire.Details(playerid);
      } else {
        Attire.Details(playerid, "已经穿上了啊");
      }
    } else if(strcmp(inputtext, "丢掉") == 0) {
      if(Attire.GetEditId(playerid) != -1) {
        Attire.Details(playerid, "请先脱下再丢弃");
        return 1;
      }
      new msg[128], id = pAttire[playerid][SelectId];
      pAttire[playerid][SelectId] = -1;
      format(msg, sizeof msg, "UPDATE `attire` SET `deleted_at`=now() WHERE `id`='%d'", AttireInfo[playerid][id][Id]);
      mysql_query(mariadb, msg, false);
      AttireInfo[playerid][id][Id] = 0;
      format(msg, sizeof msg, "你把装扮[%s]丢弃了!", AttireInfo[playerid][id][ModelName]);
      DIALOG_node(playerid, msg);
      return 1;
    }
    return 1;
	}

	if(dialogid == DIALOG_ATTIRE_MAIN) {
		pAttire[playerid][SelectId] = -1;
		if(!response) {
			return 1;
		}
		if(AttireInfo[playerid][listitem][Id] == 0) {
			Attire.Dialog(playerid, "空的!!!点你妹啊");
			return 1;
		}
		pAttire[playerid][SelectId] = listitem;
		Attire.Details(playerid);
		return 1;
  }
	return 1;
}

stock Attire.UpdateStatus(playerid, id, status) {
  new slot;
  AttireInfo[playerid][id][Status] = status;
  Attire.UpdateData(playerid, id);
  if(status == 0) {
    slot = Attire.GetEditId(playerid);
    pAttire[playerid][Slot][slot] = -1;
    RemovePlayerAttachedObject(playerid, slot);
  } else {
    slot = Attire.GetNullSlot(playerid);
    Attire.Attach(playerid, slot, id);
  }
}

stock Attire.UpdateData(playerid, id) {
  if(AttireInfo[playerid][id][Id] != 0) {
    new msg[512];
    format(msg, sizeof msg, "UPDATE `attire` SET `bone_id`='%d',`offset_x`='%.3f',`offset_y`='%.3f',`offset_z`='%.3f',`rot_x`='%.3f',`rot_y`='%.3f',`rot_z`='%.3f',`scale_x`='%.3f',`scale_y`='%.3f',`scale_z`='%.3f',`status`='%d' WHERE `id`='%d'",
    AttireInfo[playerid][id][BoneId], AttireInfo[playerid][id][OffsetX], AttireInfo[playerid][id][OffsetY], AttireInfo[playerid][id][OffsetZ], 
    AttireInfo[playerid][id][RotX], AttireInfo[playerid][id][RotY], AttireInfo[playerid][id][RotZ], AttireInfo[playerid][id][ScaleX], 
    AttireInfo[playerid][id][ScaleY], AttireInfo[playerid][id][ScaleZ], AttireInfo[playerid][id][Status], AttireInfo[playerid][id][Id]);
    mysql_query(mariadb, msg, false);
  }
}

stock AttireEAO(playerid, response, index, /*modelid, boneid,*/ Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ) {
	if(pAttire[playerid][SelectId] != -1) {
		new id = pAttire[playerid][SelectId];
		pAttire[playerid][SelectId] = -1;
		if(response == 1) {
			AttireInfo[playerid][id][OffsetX] = fOffsetX;
			AttireInfo[playerid][id][OffsetY] = fOffsetY;
			AttireInfo[playerid][id][OffsetZ] = fOffsetZ;
			AttireInfo[playerid][id][RotX] = fRotX;
			AttireInfo[playerid][id][RotY] = fRotY;
			AttireInfo[playerid][id][RotZ] = fRotZ;
			AttireInfo[playerid][id][ScaleX] = fScaleX;
			AttireInfo[playerid][id][ScaleY] = fScaleY;
			AttireInfo[playerid][id][ScaleZ] = fScaleZ;
			RemovePlayerAttachedObject(playerid, index);
			Attire.Attach(playerid, index, id);
			Attire.UpdateData(playerid, id);
		} else {
			Attire.Attach(playerid, index, id);
		}
	}
	return 0;
}

stock Attire.GetEditId(playerid) {
	for(new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
		if(pAttire[playerid][Slot][i] == pAttire[playerid][SelectId]) {
			return i;
		}
	}
	return -1;
}