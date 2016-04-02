#include "tinput.h"
#include "input.h"
#include "includes.h"
#include "tmotion.h"



u8 gStatus_scan[INPUT_NUM];

/*
u8 status_ready[STATUS_NUM][(INPUT_NUM<<1)+1] = {{0x01,0x4A,0x97,0x42,0x01,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x4A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{0x01,0x0A,0x97,0x42,0x11,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
{}};

*/

void tInput_Scan(void *p_arg)
{
    u8 cnt = 0;
    OS_ERR err;
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
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
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


bool is_no_foup(void)
{
    if((INPUT_ReadOne(CS_I_15,PH01_15) == 0x00) &&\
            (INPUT_ReadOne(CS_I_15,PH02_15) == 0x00) && \
            (INPUT_ReadOne(CS_I_15,PH03_15) == 0x00) && \
            (INPUT_ReadOne(CS_I_12,ES01_12) == 0x00))
    {
        return true;
    }
    return false;
}
bool is_foup_place(void)
{
    if((INPUT_ReadOne(CS_I_15,PH01_15) == 0x01) &&\
            (INPUT_ReadOne(CS_I_15,PH02_15) == 0x01) && \
            (INPUT_ReadOne(CS_I_15,PH03_15) == 0x01) && \
            (INPUT_ReadOne(CS_I_12,ES01_12) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_obstacle(void)
{
    if((INPUT_ReadOne(CS_I_12,ES04_12) == 0x01) &&\
            (INPUT_ReadOne(CS_I_12,ES05_12) == 0x01) && \
            (INPUT_ReadOne(CS_I_12,ES06_12) == 0x01))
    {
        return true;
    }
    return false;
}
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
}
bool is_mapstart(void)
{
}
bool is_mapend(void)
{
}
bool is_drdwlmt(void)
{
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
bool is_air(void)
{
    if((INPUT_ReadOne(CS_I_10,AS01_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_error(void)
{
}
bool is_busy(void)
{
}

u8 podop_before(void)
{
}
u8 podop_running(void);
u8 podcl_before(void);
u8 podcl_running(void);
u8 vacon_before(void);
u8 vacon_running(void);
u8 dorop_before(void);
u8 dorop_running(void);
u8 dorcl_before(void);
u8 dorcl_running(void);
u8 zdrup_before(void);
u8 zdrup_running(void);
u8 zdrmp_before(void);
u8 zdrmp_running(void);
u8 zdrdw_before(void);
u8 zdrdw_running(void);
u8 ywait_before(void);
u8 ywait_running(void);
u8 ydoor_before(void);
u8 ydoor_running(void);
u8 dorbk_before(void);
u8 dorbk_running(void);
u8 dorfw_before(void);
u8 dorfw_running(void);
u8 mapop_before(void);
u8 mapop_running(void);
u8 mapcl_before(void);
u8 mapcl_running(void);
u8 zmpst_before(void);
u8 zmpst_running(void);
u8 zmped_before(void);
u8 zmped_running(void);
u8 mston_before(void);
u8 mston_running(void);
u8 mstof_before(void);
u8 mstof_running(void);

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


