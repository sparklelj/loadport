#include "includes.h"
#include "uart.h"
#include "texecute.h"
#include "stdio.h"
#include "tinput.h"
#include "tcmd.h"

u8 gAddr[2] = {'0','0'};
u8 gCom_mod = 0x0F;
u8 gLED_status[9] = {0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C};
u8 gVersion[13] = {'/','V','E','R',' ','1','.','0','1',' ',' ',' ',' '};
u8 gCUr_status = G_CUR_STA_UNI; //uninit
u8 gCur_CMD = G_CUR_CMD_NO;

u8 gAction_seq[32];
u8 gCmd_action = CMD_ACTION_NOACT;
u8 gAction_num = 0;
u8 gAction_sta = 0;
u8 gEnd_act = CMD_ACTION_NOACT;


bool is_run(void)
{
    if(gCUr_status == G_CUR_STA_RUN || \
            gCUr_status == G_CUR_STA_PAU || \
            gCUr_status == G_CUR_STA_RSM || \
            gCUr_status == G_CUR_STA_INT)
    {
        return true;
    }
    return false;
}

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
    if((type & 0x0F) == 0x2)
    {
        msg[mlen] = 'I';
        mlen++;
        msg[mlen] = 'N';
        mlen++;
        msg[mlen] = 'F';
        mlen++;
    }
    if((type & 0x0F) == 0x3)
    {
        msg[mlen] = 'A';
        mlen++;
        msg[mlen] = 'B';
        mlen++;
        msg[mlen] = 'S';
        mlen++;
    }
    if((type & 0x0F) == 0x4)
    {
        msg[mlen] = 'R';
        mlen++;
        msg[mlen] = 'I';
        mlen++;
        msg[mlen] = 'F';
        mlen++;
    }
    if((type & 0x0F) == 0x5)
    {
        msg[mlen] = 'R';
        mlen++;
        msg[mlen] = 'A';
        mlen++;
        msg[mlen] = 'S';
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
    for(i=0; i<mlen; i++)
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
    return true;
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
    if(strsum[0] == *(msg+len+1) && strsum[1] == *(msg+len+2))
    {
        return true;
    }
    return false;
}


bool check_fornat(u8* msg)
{
    u16 len;
    len = *(msg + 1);
    len = (len << 8) + *(msg + 2);
    if((msg[0] == 0x01) && (msg[len+3] == 0x03) && (msg[3] == gAddr[0]) && (msg[4] == gAddr[1]))
    {
        return true;
    }
    return false;
}

bool format_state(u8* param)
{
    param[0] = '/';
    param[1] = get_equ();
    param[2] = '0';
    param[3] = get_init();
    param[4] = get_oper() + 0x30;
    param[5] = (gErr_no >> 4) + 0x30;
    param[6] = (gErr_no & 0x0F) + 0x30;
    param[7] = '0';
    param[8] = clam_sta();
    param[9] = latch_sta();
    param[10] = vac_sta();
    param[11] = dr_pos();
    param[12] = is_block();
    param[13] = Z_pos();
    param[14] = Y_pos();
    param[15] = map_apos();
    param[16] = get_mapzpos();
    param[17] = map_stp();
    param[18] = get_mapsts();
    param[19] = '1';
    param[20] = '0';
    return true;
}

bool format_ledst(u8* param)
{
    u8 cnt;
    param[0] = '/';
    for(cnt=0; cnt<9; cnt++)
    {
        if((gLED_status[cnt] & 0xF0) == 0x10)
        {
            switch (gLED_status[cnt] & 0x03)
            {
            case 1:
                param[cnt+1] = '1';
                break;
            case 2:
                param[cnt+1] = '2';
                break;
            case 3:
                param[cnt+1] = '0';
                break;
            }
        }
        else
        {
            switch (gLED_status[cnt] & 0x0C)
            {
            case 0x04:
                param[cnt+1] = '1';
                break;
            case 0x08:
                param[cnt+1] = '2';
                break;
            case 0x0C:
                param[cnt+1] = '0';
                break;
            }
        }
    }
    return true;
}

bool format_mapdt(u8* param)
{
    u8 result[25];
    u8 i;
    param[0] = '/';
    if(Analyze_Scan(result) == true)
    {
        for(i=0; i<25; i++)
        {
            switch (result[i])
            {
            case 0:
                param[i+1] = '0';
                break;
            case 1:
                param[i+1] = '1';
                break;
            case 2:
                param[i+1] = 'W';
                break;
            case 3:
                param[i+1] = '2';
                break;
            }
        }
        return true;
    }
    else
    {
        memset(param+1,'0',25);
        return false;
    }
}

bool format_maprd(u8* param)
{
    u8 result[25];
    u8 i;
    param[0] = '/';
    if(Analyze_Scan(result) == true)
    {
        for(i=0; i<25; i++)
        {
            switch (result[i])
            {
            case 0:
                param[25-i] = '0';
                break;
            case 1:
                param[25-i] = '1';
                break;
            case 2:
                param[25-i] = 'W';
                break;
            case 3:
                param[25-i] = '2';
                break;
            }
        }
        return true;
    }
    else
    {
        memset(param+1,'0',25);
        return false;
    }
}

bool format_wfcnt(u8* param)
{
    u8 result[25];
    u8 i;
    u8 cnt = 0;
    param[0] = '/';
    if(Analyze_Scan(result) == true)
    {
        for(i=0; i<25; i++)
        {
            switch (result[i])
            {
            case 0:
                break;
            case 1:
                cnt += 2;
                break;
            case 2:
                cnt += 4;
                break;
            case 3:
                cnt += 1;
                break;
            }
        }
        cnt = cnt >> 1;
        sprintf((char*)param+1, "%2d", cnt);
        return true;
    }
    else
    {
        param[1] = '?';
        param[2] = '?';
        return true;
    }
}
// type 0 normal 0x80 force
void format_podop(u8 type)
{
    gAction_seq[gAction_num++] = 20 + type;
    gAction_seq[gAction_num++] = 21 + type;
    gAction_seq[gAction_num++] = 22 + type;
}

void format_podcl(u8 type)
{
    gAction_seq[gAction_num++] = 1 + type;
    gAction_seq[gAction_num++] = 2 + type;
    gAction_seq[gAction_num++] = 3 + type;
}

void format_vacon(u8 type)
{
    gAction_seq[gAction_num++] = 5 + type;
}

void format_vacof(u8 type)
{
    gAction_seq[gAction_num++] = 18 + type;
}

void format_dorop(u8 type)
{
    gAction_seq[gAction_num++] = 6 + type;
}

void format_dorcl(u8 type)
{
    gAction_seq[gAction_num++] = 17 + type;
}

void format_mapop(u8 type)
{
    gAction_seq[gAction_num++] = 9 + type;
}

void format_mapcl(u8 type)
{
    gAction_seq[gAction_num++] = 12 + type;
}

void format_zdrup(u8 type)
{
    gAction_seq[gAction_num++] = 15 + type;
}

void format_zdrdw(u8 type)
{
    gAction_seq[gAction_num++] = 14 + type;
}

void format_zdrmp(u8 type)
{
    gAction_seq[gAction_num++] = 11 + type;
}

void format_zmpst(u8 type)
{
    gAction_seq[gAction_num++] = 8 + type;
}

void format_zmped(u8 type)
{
    gAction_seq[gAction_num++] = 15 + type;
}

void format_mston(u8 type)
{
    gAction_seq[gAction_num++] = 10 + type;
}

void format_mstof(u8 type)
{
    gAction_seq[gAction_num++] = 13 + type;
}

void format_ywait(u8 type)
{
    gAction_seq[gAction_num++] = 19 + type;
}

void format_ydoor(u8 type)
{
    gAction_seq[gAction_num++] = 4 + type;
}

void format_dorbk(u8 type)
{
    gAction_seq[gAction_num++] = 7 + type;
}

void format_dorfw(u8 type)
{
    gAction_seq[gAction_num++] = 16 + type;
}

void format_orgsh(u8 type)
{
    format_mapcl(type);
    format_zmped(type);
    format_mstof(type);
    format_dorbk(type);
    format_zdrup(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
    format_podop(type);
}

void format_aborg(u8 type)
{
    format_mapcl(type);
    format_zmped(type);
    format_mstof(type);
    format_dorbk(type);
    format_zdrup(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
    format_podop(type);
}

void format_cload(u8 type)
{
    format_podcl(type);
    format_ydoor(type);
    format_vacon(type);
    format_dorop(type);
    format_dorbk(type);
    format_zdrmp(type);
    format_zdrdw(type);
}

void format_clddk(u8 type)
{
    format_podcl(type);
    format_ydoor(type);
    format_vacon(type);
    format_dorop(type);
}

void format_cldyd(u8 type)
{
    format_podcl(type);
    format_ydoor(type);
}

void format_cldop(u8 type)
{
    format_dorbk(type);
    format_zdrdw(type);
}

void format_cldmp(u8 type)
{
    format_podcl(type);
    format_ydoor(type);
    format_vacon(type);
    format_dorop(type);
    format_dorbk(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrdw(type);
}

void format_clmpo(u8 type)
{
    format_dorbk(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrdw(type);
}

void format_culod(u8 type)
{
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
    format_podop(type);
}

void format_culdk(u8 type)
{
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
}

void format_cudcl(u8 type)
{
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
}

void format_cudnc(u8 type)
{
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
    format_podop(type);
}

void format_culyd(u8 type)
{
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
}

void format_cuflc(u8 type)
{
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
}

void format_cudmp(u8 type)
{
    format_zdrup(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
    format_podop(type);
}

void format_cumdk(u8 type)
{
    format_zdrup(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
}

void format_cumfc(u8 type)
{
    format_zdrup(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrup(type);
    format_zmped(type);
    format_dorfw(type);
    format_dorcl(type);
    format_vacof(type);
    format_ywait(type);
}

void format_mapdo(u8 type)
{
    format_zdrup(type);
    format_dorbk(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrdw(type);
}

void format_remap(u8 type)
{
    format_zdrup(type);
    format_dorbk(type);
    format_zmpst(type);
    format_mapop(type);
    format_mston(type);
    format_zdrmp(type);
    format_mapcl(type);
    format_mstof(type);
    format_zdrdw(type);
}

bool proc_set(u8* cmd_name)
{
	send_msg(gCom_mod & BCAK_ACK, (char*)cmd_name, (u8*)NULL, 0);
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
		send_msg(gCom_mod & BCAK_FIN, (char*)cmd_name, (u8*)NULL, 0);
    return true;
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
    return true;
}

bool proc_get(u8* cmd_name)
{
    if(memcmp(cmd_name, "STATE", 5) == 0)
    {
        u8 param[21];
        format_state(param);
        send_msg(gCom_mod & BCAK_ACK, "VERSN", param, 21);
    }
    if(memcmp(cmd_name, "VERSN", 5) == 0)
    {
        send_msg(gCom_mod & BCAK_ACK, "STATE", gVersion, 13);
    }
    if(memcmp(cmd_name, "LEDST", 5) == 0)
    {
        u8 param[10];
        format_ledst(param);
        send_msg(gCom_mod & BCAK_ACK, "LEDST", param, 10);
    }
    if(memcmp(cmd_name, "MAPDT", 5) == 0)
    {
        u8 param[26];
        format_mapdt(param);
        send_msg(gCom_mod & BCAK_ACK, "MAPDT", param, 26);
    }
    if(memcmp(cmd_name, "MAPRD", 5) == 0)
    {
        u8 param[26];
        format_maprd(param);
        send_msg(gCom_mod & BCAK_ACK, "MAPRD", param, 26);
    }
    if(memcmp(cmd_name, "WFCNT", 5) == 0)
    {
        u8 param[3];
        format_wfcnt(param);
        send_msg(gCom_mod & BCAK_ACK, "WFCNT", param, 3);
    }
    return true;
}

bool proc_fin(u8* cmd_name)
{
    return true;
}

bool proc_mov(u8* cmd_name)
{
    if(memcmp(cmd_name, "ORGSH", 5) == 0)
    {
        gCUr_status = G_CUR_STA_UNI;
    }
    if(memcmp(cmd_name, "ABORG", 5) == 0)
    {
        gCUr_status = G_CUR_STA_UNI;
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
        u8 ret = 0;
        u8 error;
        ret = podop_before(&error);
        if(ret == true)
        {
            switch(error)
            {
            case 0x30:
                send_msg(gCom_mod & BCAK_NAK, (char*)"PODOP", (u8*)"/INTER/ERROR", 12);
                break;
            case 0x31:
                send_msg(gCom_mod & BCAK_NAK, (char*)"PODOP", (u8*)"/INTER/CBUSY", 12);
                break;
            case 0x33:
                send_msg(gCom_mod & BCAK_NAK, (char*)"PODOP", (u8*)"/INTER/DPOSI", 12);
                break;
            }
            //        gCUr_status = G_CUR_STA_INT;
        }
        else
        {
            gCmd_action = CMD_ACTION_PODOP;
            gCUr_status = G_CUR_STA_RUN;
            send_msg(gCom_mod & BCAK_ACK, (char*)"PODOP", (u8*)NULL, 0);
        }
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
        gCur_pause = 0;
        gCur_stop = 0;
        gCur_abort = 0;
        gCur_retry = 1;
    }
    if(memcmp(cmd_name, "STOP_", 5) == 0)
    {
        gCur_pause = 0;
        gCur_stop = 1;
        gCur_abort = 0;
        gCur_retry = 0;
    }
    if(memcmp(cmd_name, "PAUSE", 5) == 0)
    {
        gCur_pause = 1;
        gCur_stop = 0;
        gCur_abort = 0;
        gCur_retry = 0;
    }
    if(memcmp(cmd_name, "ABORT", 5) == 0)
    {
        gCur_pause = 0;
        gCur_stop = 0;
        gCur_abort = 1;
        gCur_retry = 0;
    }
    if(memcmp(cmd_name, "RESUM", 5) == 0)
    {
        gCur_pause = 0;
        gCur_stop = 0;
        gCur_abort = 0;
        gCur_retry = 0;
    }
    return true;
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
    return true;
}

bool proc_rst(u8* cmd_name)
{
    return true;
}

bool proc_rfn(u8* cmd_name)
{
    return true;
}

bool proc_rmv(u8* cmd_name)
{
    return true;
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

    if(!check_sum(msg))
    {
//       send_msg(gCom_mod & BCAK_NAK, (char*)cmd_n, (u8*)"/CKSUM", 6);
//       return false;
    }

    if(memcmp(cmd_t, "SET", 3) == 0)
    {
        res = proc_set(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "MOD", 3) == 0)
    {
        res = proc_mod(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "GET", 3) == 0)
    {
        res = proc_get(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "FIN", 3) == 0)
    {
        res = proc_fin(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "MOV", 3) == 0)
    {
        res = proc_mov(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "EVT", 3) == 0)
    {
        res = proc_evt(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "RST", 3) == 0)
    {
        res = proc_rst(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "RFN", 3) == 0)
    {
        res = proc_rfn(cmd_n);
        if(res)
        {
            return true;
        }
    }

    if(memcmp(cmd_t, "RMV", 3) == 0)
    {
        res = proc_rmv(cmd_n);
        if(res)
        {
            return true;
        }
    }

    send_msg(gCom_mod & BCAK_NAK, (char*)cmd_t, (u8*)"/CMDER", 6);
    return false;
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
        if(lencmd > 100)
        {
            ONLINE_Read(msg, 1);
            continue;
        }
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
        if(check_fornat(msg))
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
