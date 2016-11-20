#include "tinput.h"
#include "input.h"
#include "includes.h"
#include "tmotion.h"
#include "tcmd.h"
#include "motor.h"
#include "texecute.h"

#define DIS_SLAP 300
u8 gStatus_scan[INPUT_NUM];

bool is_indis(s32 num, s32 std, s16 dis)
{
//    return true;
    if((num > (std - dis)) && (num < (std + dis)))
    {
        return true;
    }
    return false;
}

void tInput_Scan(void *p_arg)
{
    u8 cnt = 0;
    OS_ERR err;
    while(true)
    {
        while(cnt < INPUT_NUM)
        {
            if(cnt < INPUT_NUM-1)
            {
                gStatus_scan[cnt] = INPUT_Read(CS_I_9 + cnt);
            }
            else
            {
                gStatus_scan[cnt] = Get_MStatus();
            }
            cnt++;
            OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        cnt = 0;
        OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

bool is_sf_unready(void)
{
    return ~(is_sf_ready());
}
bool is_sf_ready(void)
{
    u8 tmp[INPUT_NUM] = {0x4A,0x52,0x42,0xE9,0x00,0x00,0x00,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_setfoupi(void)
{
    return true;
}
bool is_sf_setfoupd(void)
{
    u8 tmp[INPUT_NUM] = {0x4A,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamupi(void)
{
    u8 tmp[INPUT_NUM] = {0x0A,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamupd(void)
{
    u8 tmp[INPUT_NUM] = {0x8A,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clanfwdi(void)
{
    u8 tmp[INPUT_NUM] = {0x82,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clanfwdd(void)
{
    u8 tmp[INPUT_NUM] = {0x92,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamlcki(void)
{
    u8 tmp[INPUT_NUM] = {0x12,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamlckd(void)
{
    u8 tmp[INPUT_NUM] = {0x32,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_fpdcki(void)
{
    u8 tmp[INPUT_NUM] = {0x30,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_fpdckd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_dradspi(void)
{
    return true;
}
bool is_sf_dradspd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x53,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drunlti(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x13,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drunltd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x93,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_dropi(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x83,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_dropd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drdwnsi(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x02};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drdwnsd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x03};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_mpaopi(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x02,0xF9,0x00,0x00,0x70,0x03};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_mpaopd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x82,0xF9,0x00,0x00,0x70,0x03};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_stponi(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x80,0xF9,0x00,0x00,0x70,0x03};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_stpond(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x84,0xF9,0x00,0x00,0x70,0x03};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drdwnei(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x84,0xF9,0x00,0x00,0x70,0x04};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drdwned(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x84,0xF9,0x00,0x00,0x70,0x05};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_mpaci(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x04,0xF9,0x00,0x00,0x70,0x05};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_mpacd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x44,0xF9,0x00,0x00,0x70,0x05};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_stpoffi(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x40,0xF9,0x00,0x00,0x70,0x05};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_stpoffd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x05};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drdwnli(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x06};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drdwnld(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x07};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drupli(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x08};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drupld(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0xA3,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drclsi(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x83,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drclsd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x93,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drlti(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x13,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_drltd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x53,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_dradsri(void)
{
    return true;
}
bool is_sf_dradsrd(void)
{
    u8 tmp[INPUT_NUM] = {0x34,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_fpundki(void)
{
    u8 tmp[INPUT_NUM] = {0x30,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_fpundkd(void)
{
    u8 tmp[INPUT_NUM] = {0x32,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamulki(void)
{
    u8 tmp[INPUT_NUM] = {0x12,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamulkd(void)
{
    u8 tmp[INPUT_NUM] = {0x92,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clambwdi(void)
{
    u8 tmp[INPUT_NUM] = {0x82,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clambwdd(void)
{
    u8 tmp[INPUT_NUM] = {0x8A,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamdwni(void)
{
    u8 tmp[INPUT_NUM] = {0x0A,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_sf_clamdwnd(void)
{
    u8 tmp[INPUT_NUM] = {0x4A,0x52,0x42,0xF9,0x00,0x00,0x70,0x01};
    u8 base[INPUT_NUM];
    memcpy(base, gStatus_scan, INPUT_NUM);
    if(memcmp(base, tmp, INPUT_NUM) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//有光动作
bool is_no_foup(void)
{
    if((INPUT_ReadOne(CS_I_15,PH01_15) == 0x01) &&\
            (INPUT_ReadOne(CS_I_15,PH02_15) == 0x01) && \
            (INPUT_ReadOne(CS_I_15,PH03_15) == 0x01) && \
            (INPUT_ReadOne(CS_I_12,ES01_12) == 0x00))
    {
        return true;
    }
    return false;
}
//有光动作
bool is_foup_place(void)
{
    if((INPUT_ReadOne(CS_I_15,PH01_15) == 0x00) &&\
            (INPUT_ReadOne(CS_I_15,PH02_15) == 0x00) && \
            (INPUT_ReadOne(CS_I_15,PH03_15) == 0x00))
    {
        return true;
    }
    return false;
}
//有光动作
bool is_foup_presence(void)
{
    if(INPUT_ReadOne(CS_I_12,ES01_12) == 0x01)
    {
        return true;
    }
    return false;
}
//有光动作
bool is_obstacle(void)
{
    if((INPUT_ReadOne(CS_I_12,ES04_12) == 0x01) ||\
            (INPUT_ReadOne(CS_I_12,ES05_12) == 0x01) || \
            (INPUT_ReadOne(CS_I_12,ES06_12) == 0x01))
    {
        return true;
    }
    return false;
}
//有光动作
bool is_protrusion(void)
{
    if((INPUT_ReadOne(CS_I_12,FS01_12) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_clampup(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS01A_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_clampdown(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS01B_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_clamplock(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS01C_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_clampfwd(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS02A_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_clampbwd(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS02B_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_dock(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS03A_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_undock(void)
{
    if((INPUT_ReadOne(CS_I_9,CLS03B_9) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_vacuumon(void)
{
    if((INPUT_ReadOne(CS_I_10,VS01_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_vacuumoff(void)
{
    if((is_vacuumon()))
    {
        return false;
    }
    return true;
}
bool is_latch(void)
{
    if((INPUT_ReadOne(CS_I_10,CLS04B_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_unlatch(void)
{
    if((INPUT_ReadOne(CS_I_10,CLS04A_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_dropen(void)
{
    if((INPUT_ReadOne(CS_I_10,CLS05A_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_drclose(void)
{
    if((INPUT_ReadOne(CS_I_10,CLS05B_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_druplmt(void)
{
    if(is_stop() && ( gCurPos == M_UPLMT) && (is_indis(gPos_num, M_UPLMT<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_mapstart(void)
{
    if(is_stop() && ( gCurPos == M_STRMP) && (INPUT_ReadOne(CS_I_11,PH08_11) == 0x00) && (is_indis(gPos_num, M_STRMP<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_mapend(void)
{
    if(is_stop() && ( gCurPos == M_STPMP) && (INPUT_ReadOne(CS_I_11,PH07_11) == 0x00) && (is_indis(gPos_num, M_STPMP<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_drdwlmt(void)
{
    if(is_stop() && ( gCurPos == M_DNLMT) && (INPUT_ReadOne(CS_I_10,CLS06A_10) == 0x01) && (is_indis(gPos_num, M_DNLMT<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_mapopen(void)
{
    if((INPUT_ReadOne(CS_I_11,CLS08A_11) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_mapclose(void)
{
    if((INPUT_ReadOne(CS_I_11,CLS08B_11) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_stopperon(void)
{
    if((INPUT_ReadOne(CS_I_11,CLS07A_11) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_stopperoff(void)
{
    if((INPUT_ReadOne(CS_I_11,CLS07B_11) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_noair(void)
{
	return false;
    if((INPUT_ReadOne(CS_I_10,AS01_10) == 0x00))
    {
        return true;
    }
    return false;
}
bool is_error(void)
{
    if(gCur_status == G_CUR_STA_ERR)
    {
        return true;
    }
    return false;
}
bool is_busy(void)
{
    if(gCur_status == G_CUR_STA_RUN || \
            gCur_status == G_CUR_STA_PAU || \
            gCur_status == G_CUR_STA_RSM)
    {
        return true;
    }
    return false;
}
bool is_fanerr(void)
{
    return false;
}
u8 podop_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30; //ERROR
        return true;
    }
    if(is_busy())
    {
        *error = 0x31; //BUSY
        return true;
    }
    if(is_dropen())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    return false;
}
u8 podop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 podcl_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x32; //FPILG
        return true;
    }
    return false;
}
u8 podcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 vacon_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    return false;
}
u8 vacon_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 vacof_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    return false;
}
u8 vacof_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 dorop_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    return false;
}
u8 dorop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 dorcl_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_dropen())
    {
        *error = 0x33;
        return true;
    }
    return false;
}
u8 dorcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 zdrup_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x33;
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x32;
        return true;
    }
    if(!is_mapclose())
    {
        *error = 0x34; //MPARM
        return true;
    }
    if(!is_stopperoff())
    {
        *error = 0x35;
        return true;
    }
    return false;
}
u8 zdrup_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x48; //DLMIT
        return true;
    }
    if(is_protrusion())
    {
        *error = 0x07;
        return true;
    }
    return false;
}
u8 zdrmp_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_dropen())
    {
        *error = 0x33;
        return true;
    }
    if(!is_mapopen())
    {
        *error = 0x34;
        return true;
    }
    if(!is_mapstart())
    {
        *error = 0x37;
        return true;
    }
    return false;
}
u8 zdrmp_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if(is_protrusion())
    {
        *error = 0x07;
        return true;
    }
    return false;
}
u8 zdrdw_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_dropen())
    {
        *error = 0x33;
        return true;
    }
    if(!is_mapclose())
    {
        *error = 0x34; //MPARM
        return true;
    }
    if(!is_stopperoff())
    {
        *error = 0x35; //MPSTP
        return true;
    }
    return false;
}
u8 zdrdw_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x48; //DLMIT
        return true;
    }
    if(is_protrusion())
    {
        *error = 0x07;
        return true;
    }
    return false;
}
u8 ywait_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_foup_place() && is_drclose() && is_vacuumon())
    {
        *error = 0x38; // DVACM
        return true;
    }
    if(is_foup_place() && is_drclose() && is_unlatch())
    {
        *error = 0x39; //UN LATCH
        return true;
    }
    return false;
}
u8 ywait_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 ydoor_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x32;
        return true;
    }
    if(is_foup_place() && is_drclose() && is_vacuumoff() && (!is_unlatch()))
    {
        *error = 0x39;
        return true;
    }
    return false;
}
u8 ydoor_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 dorbk_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_foup_presence() && is_vacuumon() && (!is_unlatch()))
    {
        *error = 0x39;
        return true;
    }
    return false;
}
u8 dorbk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 dorfw_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x32;
        return true;
    }
    if(is_vacuumon() && is_latch())
    {
        *error = 0x3A;
        return true;
    }
    if(is_foup_place() && is_dock() && is_vacuumoff() && is_latch())
    {
        *error = 0x4A;  //LATCH
        return true;
    }
    if(!is_druplmt())
    {
        *error = 0x4B; //DPOSI
        return true;
    }
    return false;
}
u8 dorfw_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 mapop_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_dropen())
    {
        *error = 0x33;
        return true;
    }
    if(is_mapstart())
    {
        *error = 0x34;
        return true;
    }
    return false;
}
u8 mapop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 mapcl_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    return false;
}
u8 mapcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if(is_protrusion())
    {
        *error = 0x07;
        return true;
    }
    return false;
}
u8 zmpst_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x33;
        return true;
    }
    if(!is_mapclose())
    {
        *error = 0x34;
        return true;
    }
    return false;
}
u8 zmpst_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if(is_protrusion())
    {
        *error = 0x07;
        return true;
    }
    return false;
}
u8 zmped_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x33;
        return true;
    }
    if(!is_mapclose())
    {
        *error = 0x34;
        return true;
    }
    return false;
}
u8 zmped_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if(is_protrusion())
    {
        *error = 0x07;
        return true;
    }
    return false;
}
u8 mston_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(!is_druplmt() && (!is_mapstart()))
    {
        *error = 0x37;
        return true;
    }
    return false;
}
u8 mston_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 mstof_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    return false;
}
u8 mstof_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 orgsh_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30; //ERROR
        return true;
    }
    if(is_busy())
    {
        *error = 0x31; //BUSY
        return true;
    }
    if(is_vacuumon())
    {
        *error = 0x38; // DVACM
        return true;
    }
    if(!is_foup_place() && (!(is_no_foup() && is_drclose())))
    {
        *error = 0x39; //UN LATCH
        return true;
    }
    return false;
}
u8 orgsh_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 aborg_before(u8* error)
{
    return false;
}
u8 aborg_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    return false;
}
u8 cload_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = 0x39; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    return false;
}
u8 cload_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 clddk_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = 0x39; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    return false;
}
u8 clddk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 cldyd_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = 0x39; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    return false;
}
u8 cldyd_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 cldop_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(gEnd_act != CMD_ACTION_CLDDK)
    {
        *error = 0x81; //CLDDK
        return true;
    }
    return false;
}
u8 cldop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 cldmp_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = 0x39; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    if(!is_druplmt())
    {
        *error = 0x82; //ZPOSI up limit
        return true;
    }
    return false;
}
u8 cldmp_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 clmpo_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(gEnd_act != CMD_ACTION_CLDDK)
    {
        *error = 0x81; //CLDDK
        return true;
    }
    if(!is_druplmt())
    {
        *error = 0x82; //ZPOSI up limit
        return true;
    }
    return false;
}
u8 clmpo_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 culod_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    return false;
}
u8 culod_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 culdk_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    return false;
}
u8 culdk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 culyd_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == true)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    return false;
}
u8 culyd_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 cudcl_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    if(gEnd_act != CMD_ACTION_CULDK)
    {
        *error = 0x84; //CULDK
        return true;
    }
    return false;
}
u8 cudcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 cudnc_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    if(gEnd_act != CMD_ACTION_CULDK)
    {
        *error = 0x84; //CILDK
        return true;
    }
    return false;
}
u8 cudnc_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 culfc_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    return false;
}
u8 culfc_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 mapdo_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(gEnd_act != CMD_ACTION_CLOAD)
    {
        *error = 0x85; //CLOAD
        return true;
    }
    return false;
}
u8 mapdo_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 remap_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(0)
    {
        *error = 0x86; //RMPOS
        return true;
    }
    return false;
}
u8 remap_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA1;
        return true;
    }
    return false;
}
u8 cudmp_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    if(!is_mapopen())
    {
        *error = 0x87; //MPARM
        return true;
    }
    return false;
}
u8 cudmp_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 cumdk_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    if(!is_mapopen())
    {
        *error = 0x87; //MPARM
        return true;
    }
    return false;
}
u8 cumdk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}
u8 cumfc_before(u8* error)
{
    if(is_error())
    {
        *error = 0x30;
        return true;
    }
    if(is_busy())
    {
        *error = 0x31;
        return true;
    }
    if(is_origin == false)
    {
        *error = 0x80; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = 0x4A; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = 0x82; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = 0x83; //LATCH
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x33; //DPOSI
        return true;
    }
    if(!is_mapopen())
    {
        *error = 0x87; //MPARM
        return true;
    }
    return false;
}
u8 cumfc_running(u8* error)
{
    if(is_obstacle())
    {
        *error = 0xFF;
        return true;
    }
    if(is_noair())
    {
        *error = 0x27;
        return true;
    }
    if(is_fanerr())
    {
        *error = 0xFC;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = 0xA2;
        return true;
    }
    return false;
}

u8 clam_sta(void)
{
    if((INPUT_ReadOne(CS_I_9, CLS01C_9) == 1) && (INPUT_ReadOne(CS_I_9, CLS02A_9) == 1))
    {
        return '1';
    }
    if((INPUT_ReadOne(CS_I_9, CLS01B_9) == 1) && (INPUT_ReadOne(CS_I_9, CLS02B_9) == 1))
    {
        return '0';
    }
    return '?';
}

u8 latch_sta(void)
{
    if(INPUT_ReadOne(CS_I_10, CLS04B_10) == 1)
    {
        return '1';
    }
    if(INPUT_ReadOne(CS_I_10, CLS04A_10) == 1)
    {
        return '0';
    }
    return '?';
}

u8 vac_sta(void)
{
    if(INPUT_ReadOne(CS_I_10, VS01_10) == 1)
    {
        return '1';
    }
    else
    {
        return '0';
    }
}

u8 dr_pos(void)
{
    if(INPUT_ReadOne(CS_I_10, CLS05B_10) == 1)
    {
        return '1';
    }
    if(INPUT_ReadOne(CS_I_10, CLS05A_10) == 1)
    {
        return '0';
    }
    return '?';
}

u8 is_block(void)
{
    if(is_obstacle())
    {
        return '0';
    }
    else
    {
        return '1';
    }
}

u8 Z_pos(void)
{
    if(Get_MStatus() == 0x01)
    {
        return '0';
    }
    if(Get_MStatus() == 0x07)
    {
        return '1';
    }
    return '?';
}

u8 Y_pos(void)
{
    if(INPUT_ReadOne(CS_I_9, CLS03A_9) == 1)
    {
        return '1';
    }
    if(INPUT_ReadOne(CS_I_9, CLS03B_9) == 1)
    {
        return '0';
    }
    return '?';
}

u8 get_equ(void)
{
    if(gCur_status != G_CUR_STA_ERR)
    {
        return '0';
    }
    if(1)
    {
        return 'E';
    }
    if(0)
    {
        return 'A';
    }
}

u8 get_mapzpos(void)
{
    if(Get_MStatus() == 0)
    {
        return '?';
    }
    if(Get_MStatus() == 0x01)
    {
        return '0';
    }
    return '1';
}

u8 map_apos(void)
{
    if(INPUT_ReadOne(CS_I_11, CLS08B_11) == 1)
    {
        return '1';
    }
    if(INPUT_ReadOne(CS_I_11, CLS08A_11) == 1)
    {
        return '0';
    }
    return '?';
}

u8 map_stp(void)
{
    if(INPUT_ReadOne(CS_I_11, CLS07B_11) == 1)
    {
        return '1';
    }
    if(INPUT_ReadOne(CS_I_11, CLS07A_11) == 1)
    {
        return '0';
    }
    return '?';
}

bool is_low(void)
{
    if(((gStatus_scan[1]&0x02) == 0x02) && ((gStatus_scan[3]&0x01) == 0x01))
    {
        return false;
    }
    else
    {
        return true;
    }
}


