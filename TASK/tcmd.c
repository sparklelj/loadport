#include "tcmd.h"
#include "includes.h"
#include "uart.h"
#include "stdio.h"

u8 gAddr[2] = {'0','0'};
u8 gCom_mod = 0;
u8 gLED_status[8];
u8 gVersion[13] = {'/','V','E','R',' ','1','.','0','1',' ',' ',' ',' '};

bool send_msg(u8 type, char* cmd_n, u8* param, u8 pLen)
{
	u8 msg[100];
	u8 mlen = 0;
	u8 sum = 0;
	u8 i;
	u8 s_len;
	OS_ERR err;
	
	msg[mlen] = 0x01;
	mlen++;
	msg[mlen] = pLen + 14;
	mlen++;
	msg[mlen] = 0x00;
	mlen++;
	msg[mlen] = gAddr[0];
	mlen++;
	msg[mlen] = gAddr[1];
	mlen++;
	if((type & 0x0F) == 0x0)
	{
		msg[mlen] = 'A';
		mlen++;
		msg[mlen] = 'C';
		mlen++;
		msg[mlen] = 'K';
		mlen++;
	}
	if((type & 0x0F) == 0x1)
	{
		msg[mlen] = 'N';
		mlen++;
		msg[mlen] = 'A';
		mlen++;
		msg[mlen] = 'K';
		mlen++;
	}
	msg[mlen] = ':';
	mlen++;
	memcpy(msg+mlen, cmd_n, 5);
	mlen += 5;
	memcpy(msg+mlen, param, pLen);
	mlen += pLen;
	msg[mlen] = ';';
	mlen++;
	for(i=0;i<mlen;i++)
	{
		sum += msg[i];
	}
	sprintf((char*)(msg + mlen), "%2X", sum);
	mlen += 2;
	msg[mlen] = 0x03;
	mlen++;
	if((type & 0xF0) == 0x00)
	{
		s_len = ONLINE_Write(msg, mlen);
		while(s_len !=  mlen)
		{
			mlen = mlen - s_len;
			s_len = ONLINE_Write(msg, mlen);
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		}
	}
	if((type & 0xF0) == 0x10)
	{
		s_len = CMD_Write(msg, mlen);
		while(s_len !=  mlen)
		{
			mlen = mlen - s_len;
			s_len = CMD_Write(msg, mlen);
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		}
	}
}

bool check_sum(u8* msg)
{
    u8 i;
    u16 len;
    u8 sum = 0;
    u8 strsum[5];
    len = *(msg + 1);
    len = (len << 8) + *(msg + 2);
    for(i=1; i<=len; i++)
    {
        sum += *(msg + i);
    }
    sprintf((char*)strsum, "%2X", sum);
    if(strsum[0] == *(msg+len+1) && strsum[0] == *(msg+len+2))
    {
        return true;
    }
    return false;
}

bool format_state(u8* param)
{
}

bool format_ledst(u8* param)
{
}

bool format_mapdt(u8* param)
{
}

bool format_maprd(u8* param)
{
}

bool format_wfcnt(u8* param)
{
}

bool proc_set(u8* cmd_name)
{
    if(memcmp(cmd_name, "RESET", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "INITL", 5) == 0)
    {
    }
    if((memcmp(cmd_name, "LPLOD", 5) == 0) || (memcmp(cmd_name, "LON01", 5) == 0))
    {
			gLED_status[0] = 0x11;
    }
    if((memcmp(cmd_name, "BLLOD", 5) == 0) || (memcmp(cmd_name, "LBL01", 5) == 0))
    {
			gLED_status[0] = 0x12;
    }
    if((memcmp(cmd_name, "LOLOD", 5) == 0) || (memcmp(cmd_name, "LOF01", 5) == 0))
    {
			gLED_status[0] = 0x13;
    }
    if((memcmp(cmd_name, "LPULD", 5) == 0) || (memcmp(cmd_name, "LON02", 5) == 0))
    {
			gLED_status[1] = 0x11;
    }
    if((memcmp(cmd_name, "BLULD", 5) == 0) || (memcmp(cmd_name, "LBL02", 5) == 0))
    {
			gLED_status[1] = 0x12;
    }
    if((memcmp(cmd_name, "LOULD", 5) == 0) || (memcmp(cmd_name, "LOF02", 5) == 0))
    {
			gLED_status[1] = 0x13;
    }
    if((memcmp(cmd_name, "LPMSW", 5) == 0) || (memcmp(cmd_name, "LON03", 5) == 0))
    {
			gLED_status[2] = 0x11;
    }
    if((memcmp(cmd_name, "BLMSW", 5) == 0) || (memcmp(cmd_name, "LBL03", 5) == 0))
    {
			gLED_status[2] = 0x12;
    }
    if((memcmp(cmd_name, "LOMSW", 5) == 0) || (memcmp(cmd_name, "LOF03", 5) == 0))
    {
			gLED_status[2] = 0x13;
    }
    if((memcmp(cmd_name, "LPCON", 5) == 0) || (memcmp(cmd_name, "LON04", 5) == 0))
    {
			gLED_status[3] = 0x11;
    }
    if((memcmp(cmd_name, "BLCON", 5) == 0) || (memcmp(cmd_name, "LBL04", 5) == 0))
    {
			gLED_status[3] = 0x12;
    }
    if((memcmp(cmd_name, "LOCON", 5) == 0) || (memcmp(cmd_name, "LOF04", 5) == 0))
    {
			gLED_status[3] = 0x13;
    }
    if((memcmp(cmd_name, "LPCST", 5) == 0) || (memcmp(cmd_name, "LON05", 5) == 0))
    {
			gLED_status[4] = 0x11;
    }
    if((memcmp(cmd_name, "BLCST", 5) == 0) || (memcmp(cmd_name, "LBL05", 5) == 0))
    {
			gLED_status[4] = 0x12;
    }
    if((memcmp(cmd_name, "LOCST", 5) == 0) || (memcmp(cmd_name, "LOF05", 5) == 0))
    {
			gLED_status[4] = 0x13;
    }
    if(memcmp(cmd_name, "LON07", 5) == 0)
    {
			gLED_status[6] = 0x11;
    }
    if(memcmp(cmd_name, "LBL07", 5) == 0)
    {
			gLED_status[6] = 0x12;
    }
    if(memcmp(cmd_name, "LOF07", 5) == 0)
    {
			gLED_status[6] = 0x13;
    }
    if(memcmp(cmd_name, "LON08", 5) == 0)
    {
			gLED_status[7] = 0x11;
    }
    if(memcmp(cmd_name, "LBL08", 5) == 0)
    {
			gLED_status[7] = 0x12;
    }
    if(memcmp(cmd_name, "LOF08", 5) == 0)
    {
			gLED_status[7] = 0x13;
    }
}

bool proc_mod(u8* cmd_name)
{
    if(memcmp(cmd_name, "ONMGV", 5) == 0)
    {
			gCom_mod = 0;
    }
    if(memcmp(cmd_name, "MENTE", 5) == 0)
    {
			gCom_mod = 1;
    }
}

bool proc_get(u8* cmd_name)
{
    if(memcmp(cmd_name, "STATE", 5) == 0)
    {
			u8 param[21];
			format_state(param);
			send_msg(0x00, "VERSN", param, 21);
    }
    if(memcmp(cmd_name, "VERSN", 5) == 0)
    {
			send_msg(0x00, "STATE", gVersion, 13);
    }
    if(memcmp(cmd_name, "LEDST", 5) == 0)
    {
			u8 param[10];
			format_ledst(param);
			send_msg(0x00, "VERSN", param, 10);
    }
    if(memcmp(cmd_name, "MAPDT", 5) == 0)
    {
			u8 param[26];
			format_mapdt(param);
			send_msg(0x00, "MAPDT", param, 26);
    }
    if(memcmp(cmd_name, "MAPRD", 5) == 0)
    {
			u8 param[26];
			format_maprd(param);
			send_msg(0x00, "MAPRD", param, 26);
    }
    if(memcmp(cmd_name, "WFCNT", 5) == 0)
    {
			u8 param[3];
			format_wfcnt(param);
			send_msg(0x00, "WFCNT", param, 3);
    }
}

bool proc_fin(u8* cmd_name)
{
}

bool proc_mov(u8* cmd_name)
{
    if(memcmp(cmd_name, "ORGSH", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ABORG", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CLOAD", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CLDDK", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CLDYD", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CLDOP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CLDMP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CLMPO", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CULOD", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CULDK", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CUDCL", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CUDNC", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CULYD", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CULFC", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CUDMP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CUMDK", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "CUMFC", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "MAPDO", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "REMAP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "PODOP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "PODCL", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "VACON", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "VACOF", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "DOROP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "DORCL", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "MAPOP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "MAPCL", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ZDRUP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ZDRDW", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ZDRMP", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ZMPST", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ZMPED", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "MSTON", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "MSTOF", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "YWAIT", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "YDOOR", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "DORBK", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "DORFW", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "RETRY", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "STOP_", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "PAUSE", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "ABORT", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "RESUM", 5) == 0)
    {
    }
}

bool proc_evt(u8* cmd_name)
{
    if(memcmp(cmd_name, "EVTON", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "EVTOF", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "FPEON", 5) == 0)
    {
    }
    if(memcmp(cmd_name, "FPEOF", 5) == 0)
    {
    }
}

bool proc_rst(u8* cmd_name)
{
}

bool proc_rfn(u8* cmd_name)
{
}

bool proc_rmv(u8* cmd_name)
{
}

bool proc_cmd(u8* msg)
{
    u8 addr[2];
    u8 cmd_t[3];
    u8 cmd_n[5];
    bool res;
    memcpy(addr, msg+3, 2);
    if(memcmp(addr, gAddr, 2) != 0)
    {
        return false;
    }
    memcpy(cmd_t, msg+5, 3);
    memcpy(cmd_n, msg+9, 5);

    if(memcmp(cmd_t, "SET", 3) == 0)
    {
        res = proc_set(cmd_n);
    }

    if(memcmp(cmd_t, "MOD", 3) == 0)
    {
        res = proc_mod(cmd_n);
    }

    if(memcmp(cmd_t, "GET", 3) == 0)
    {
        res = proc_get(cmd_n);
    }

    if(memcmp(cmd_t, "FIN", 3) == 0)
    {
        res = proc_fin(cmd_n);
    }

    if(memcmp(cmd_t, "MOV", 3) == 0)
    {
        res = proc_mov(cmd_n);
    }

    if(memcmp(cmd_t, "EVT", 3) == 0)
    {
        res = proc_evt(cmd_n);
    }

    if(memcmp(cmd_t, "RST", 3) == 0)
    {
        res = proc_rst(cmd_n);
    }

    if(memcmp(cmd_t, "RFN", 3) == 0)
    {
        res = proc_rfn(cmd_n);
    }

    if(memcmp(cmd_t, "RMV", 3) == 0)
    {
        res = proc_rmv(cmd_n);
    }
}

void tCMD_Proc(void *p_arg)
{
    u8 msg[105];
    u8 len;
    u16 lencmd;
    OS_ERR err;
    CPU_SR_ALLOC();
    while(1)
    {
        OSTimeDlyHMSM(0,0,0,900,OS_OPT_TIME_HMSM_STRICT,&err);
        OS_CRITICAL_ENTER();
        len = ONLINE_Peer(msg, 1);
        if(len == 0)
        {
            continue;
        }
        if(msg[0] != 0x01)
        {
            ONLINE_Read(msg, 1);
            continue;
        }
        len = ONLINE_RxLen();
        if(len < 8)
        {
            continue;
        }
        ONLINE_Peer(msg, 3);
        lencmd = (msg[1] << 8) + msg[2];
        if(lencmd < 4)
        {
            ONLINE_Read(msg, 1);
            continue;
        }
        lencmd += 4;
        len = ONLINE_RxLen();
        if(lencmd > len)
        {
            continue;
        }
        len = ONLINE_Peer(msg, lencmd);
        if(len < lencmd)
        {
            continue;
        }
        if(check_sum(msg))
        {
            len = ONLINE_Read(msg, lencmd);
            proc_cmd(msg);
        }
        else
        {
            ONLINE_Read(msg, 1);
            continue;
        }
        OS_CRITICAL_EXIT();
    }
}
