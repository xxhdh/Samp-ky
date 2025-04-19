CMD:stopsong(playerid, params[]) {
    StopAudioStreamForPlayer(playerid);
    SCM(playerid, COLOR_RED, "[系统] 你停止了音乐播放!");
	return 1;
}

stock PlayerMusicForAll(url[]) {
	for(new i = 0, all = GetPlayerPoolSize()+1; i < all; i ++) {
		if(IsPlayerConnected(i) == 1) {
			PlayAudioStreamForPlayer(i, url);
		}
	}
}