/*
	===================================
    �Ի����б�ѡ������
                            	By:Dawn
	===================================
										*/
#define DIALOG_WQSD_MAIN                 (800)
#define DIALOG_WQSD_MELEE                DIALOG_WQSD_MAIN + 1
#define DIALOG_WQSD_THROW                DIALOG_WQSD_MAIN + 2
#define DIALOG_WQSD_PISTOL               DIALOG_WQSD_MAIN + 3
#define DIALOG_WQSD_SHOTGUN              DIALOG_WQSD_MAIN + 4
#define DIALOG_WQSD_SUBMACHINEGUN        DIALOG_WQSD_MAIN + 5
#define DIALOG_WQSD_RIFLE                DIALOG_WQSD_MAIN + 6
#define DIALOG_WQSD_HEAVYGUN             DIALOG_WQSD_MAIN + 7
#define DIALOG_WQSD_OTHER                DIALOG_WQSD_MAIN + 8
#define DIALOG_WQSD_HBA                  DIALOG_WQSD_MAIN + 9
#define DIALOG_WQSD_MELEE_2              DIALOG_WQSD_MAIN + 10

CMD:wuqi(playerid, params[])
{
	new msg[256];
	format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
	ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
	return 1;
}

forward wuqiODR(playerid, dialogid, response, listitem, inputtext[]);
public wuqiODR(playerid, dialogid, response, listitem, inputtext[])
{
	new msg[256];
	if(dialogid==DIALOG_WQSD_MAIN)
	{
		if(response)
		{
			if(listitem==0)
			{
				format(msg,256,"ȭ��\n�߶������\n����\nС��\n�����\n����\n̨���\n��ʿ��\n���\n��ɫ������\n������\n{FF0000}��һҳ");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MELEE,DIALOG_STYLE_LIST,"�����˵�[��ս��]",msg,"ȷ��","����");
			}
			if(listitem==1)
			{
				format(msg,256,"����\n������˹\nȼ��ƿ");
				ShowPlayerDialog(playerid,DIALOG_WQSD_THROW,DIALOG_STYLE_LIST,"�����˵�[Ͷ����]",msg,"ȷ��","����");
			}
			if(listitem==2)
			{
				format(msg,256,"��ͨ��ǹ\n������ǹ\nɳӥ");
				ShowPlayerDialog(playerid,DIALOG_WQSD_PISTOL,DIALOG_STYLE_LIST,"�����˵�[��ǹ]",msg,"ȷ��","����");
			}
			if(listitem==3)
			{
				format(msg,256,"��������ǹ\n˫������ǹ\n7��������ǹ");
				ShowPlayerDialog(playerid,DIALOG_WQSD_SHOTGUN,DIALOG_STYLE_LIST,"�����˵�[����ǹ]",msg,"ȷ��","����");
			}
			if(listitem==4)
			{
				format(msg,256,"SMG\nMP5\nTEC-9");
				ShowPlayerDialog(playerid,DIALOG_WQSD_SUBMACHINEGUN,DIALOG_STYLE_LIST,"�����˵�[���ǹ]",msg,"ȷ��","����");
			}
			if(listitem==5)
			{
				format(msg,256,"AK47\nM4\n������ǹ\n�ѻ���ǹ");
				ShowPlayerDialog(playerid,DIALOG_WQSD_RIFLE,DIALOG_STYLE_LIST,"�����˵�[��ǹ]",msg,"ȷ��","����");
			}
			if(listitem==6)
			{
				format(msg,256,"���������\n�ȸ�׷�ٵ�\n�����\n������\nң��ըҩ��\n");
				ShowPlayerDialog(playerid,DIALOG_WQSD_HEAVYGUN,DIALOG_STYLE_LIST,"�����˵�[������]",msg,"ȷ��","����");
			}
			if(listitem==7)
			{
				format(msg,256,"������\n�����\n�����\n����ɡ\n������");
				ShowPlayerDialog(playerid,DIALOG_WQSD_OTHER,DIALOG_STYLE_LIST,"�����˵�[����]",msg,"ȷ��","����");
			}
			if(listitem==8)
			{
				format(msg,256,"Ѫ\n����\nѪ�ͻ���");
				ShowPlayerDialog(playerid,DIALOG_WQSD_HBA,DIALOG_STYLE_LIST,"�����˵�[Ѫ�ͻ���]",msg,"ȷ��","����");
			}
		}
	}

	switch(dialogid)
	{
		case DIALOG_WQSD_MELEE:
		{
			if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
				case 0:
				{
					GivePlayerWeapon(playerid,1,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,2,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,3,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,4,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,5,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 5:
				{
					GivePlayerWeapon(playerid,6,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 6:
				{
					GivePlayerWeapon(playerid,7,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 7:
				{
					GivePlayerWeapon(playerid,8,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 8:
				{
					GivePlayerWeapon(playerid,9,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 9:
				{
					GivePlayerWeapon(playerid,10,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 10:
				{
					GivePlayerWeapon(playerid,11,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 11:
				{
					format(msg,256,"{FF0000}��һҳ\n��ɫ����\n����\n����\n����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MELEE_2,DIALOG_STYLE_LIST,"�����˵�[��ս��]",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_THROW:
		{
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
				case 0:
				{
					GivePlayerWeapon(playerid,16,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,17,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,18,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_PISTOL:
		{
			if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
				case 0:
				{
					GivePlayerWeapon(playerid,22,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,23,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,24,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
        case DIALOG_WQSD_SHOTGUN:
        {
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
                case 0:
				{
					GivePlayerWeapon(playerid,25,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,26,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,27,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_SUBMACHINEGUN:
		{
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
				case 0:
				{
					GivePlayerWeapon(playerid,28,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,29,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,32,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_RIFLE:
		{
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
                case 0:
				{
					GivePlayerWeapon(playerid,30,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,31,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,33,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,34,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_HEAVYGUN:
		{
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
                case 0:
				{
					GivePlayerWeapon(playerid,35,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,36,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,37,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,38,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,39,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_OTHER:
		{
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
                case 0:
				{
					GivePlayerWeapon(playerid,41,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,42,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,43,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,46,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 4:
				{
                    SetPlayerSpecialAction(playerid,2);
                    format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_HBA:
		{
            if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
                case 0:
				{
					SetPlayerHealth(playerid,100);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 1:
				{
					SetPlayerArmour(playerid,100);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
					SetPlayerHealth(playerid,100);
					SetPlayerArmour(playerid,100);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
		case DIALOG_WQSD_MELEE_2:
		{
			if(!response)
			{
				format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
				ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				return 1;
			}
			switch(listitem)
			{
				case 0:
				{
					format(msg,256,"ȭ��\n�߶������\n����\nС��\n�����\n����\n̨���\n��ʿ��\n���\n��ɫ������\n������\n{FF0000}��һҳ");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MELEE,DIALOG_STYLE_LIST,"�����˵�[��ս��]",msg,"ȷ��","����");
				}
				case 1:
				{
                    GivePlayerWeapon(playerid,12,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 2:
				{
                    GivePlayerWeapon(playerid,13,5000);
        		    format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 3:
				{
                    GivePlayerWeapon(playerid,14,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
				case 4:
				{
                    GivePlayerWeapon(playerid,15,5000);
					format(msg,256,"��ս��\nͶ����\n��ǹ\n����ǹ\n���ǹ\n��ǹ\n������\n����\nѪ�ͷ�����");
					ShowPlayerDialog(playerid,DIALOG_WQSD_MAIN,DIALOG_STYLE_LIST,"�����˵�",msg,"ȷ��","����");
				}
			}
		}
	}
	return 1;
}
