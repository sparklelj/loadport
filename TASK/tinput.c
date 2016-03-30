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

u8 is_obstacle(void)
{
    if((gStatus_scan[6]&0x70) == 0x70)
    {
        return '1';
    }
    else
    {
        return '0';
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


