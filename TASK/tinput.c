#include "tinput.h"
#include "input.h"
#include "includes.h"
#include "tmotion.h"
#include "tcmd.h"
#include "motor.h"
#include "texecute.h"
#include "tled.h"

#define DIS_SLAP 300
u8 gStatus_scan[INPUT_NUM];
u8 gErrorLevel = ERR_REC;

void init_error(void) {
    gIsError = false;
    set_led(LED_ALARM,LED_OFF);
}

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

//有光动作
bool is_no_foup(void)
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
    if(INPUT_ReadOne(CS_I_12,ES01_12) == 0x00)
    {
        return true;
    }
    return false;
}
//有光动作
bool is_obstacle(void)
{
    if((INPUT_ReadOne(CS_I_12,ES04_12) == 0x00) ||\
            (INPUT_ReadOne(CS_I_12,ES05_12) == 0x00) || \
            (INPUT_ReadOne(CS_I_12,ES06_12) == 0x00))
    {
        return true;
    }
    return false;
}
//有光动作
bool is_protrusion(void)
{
    return false;
    if((INPUT_ReadOne(CS_I_12,FS01_12) == 0x00))
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
    if((INPUT_ReadOne(CS_I_12,VS01_12) == 0x01))
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
bool is_drondr(void)
{
    if((INPUT_ReadOne(CS_I_10,PH14_10) == 0x00))
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
	if(INPUT_ReadOne(CS_I_11,PH08_11) == 0x01)
	{
		return true;
	}
    if(is_stop() && ( gCurPos == M_STRMP) && (INPUT_ReadOne(CS_I_11,PH08_11) == 0x01) && (is_indis(gPos_num, M_STRMP<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_mapend(void)
{
    if(is_stop() && ( gCurPos == M_STPMP) && (INPUT_ReadOne(CS_I_11,PH07_11) == 0x01) && (is_indis(gPos_num, M_STPMP<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_drdwlmt(void)
{
    if(is_stop() && ( gCurPos == M_DNLMT) && (INPUT_ReadOne(CS_I_11,CLS06A_11) == 0x01) && (is_indis(gPos_num, M_DNLMT<<1, DIS_SLAP)))
    {
        return true;
    }
    return false;
}
bool is_mapopen(void)
{
    if((INPUT_ReadOne(CS_I_10,CLS08A_10) == 0x01))
    {
        return true;
    }
    return false;
}
bool is_mapclose(void)
{
    if((INPUT_ReadOne(CS_I_10,CLS08B_10) == 0x01))
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
    if((INPUT_ReadOne(CS_I_12,AS01_12) == 0x00))
    {
        return true;
    }
    return false;
}
bool is_error(void)
{
    if(gIsError == true || gCur_status == G_CUR_STA_ERR || PFin(M_ERR) == 0x01)
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
        *error = B_ERROR; //ERROR
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY; //BUSY
        return true;
    }
    if(is_dropen())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
    return false;
}
u8 podop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 podcl_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    return false;
}
u8 podcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 vacon_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    return false;
}
u8 vacon_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 vacof_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    return false;
}
u8 vacof_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 dorop_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_drondr())
    {
        *error = B_ERROR;
        return true;
    }
    return false;
}
u8 dorop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 dorcl_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    return false;
}
u8 dorcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 zdrup_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG;
        return true;
    }
    if(!is_mapclose())
    {
        *error = B_MPARM; //MPARM
        return true;
    }
    /*
    if(!is_stopperoff())
    {
    *error = B_MPSTP;
    return true;
    }
    */
    return false;
}
u8 zdrup_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(!is_dropen())
    {
        *error = R_DLMITC; //DLMIT
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    return false;
}
u8 zdrmp_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    if(!is_mapopen())
    {
        *error = B_MPARM;
        return true;
    }
    if(!is_mapstart())
    {
        *error = B_ZPOSI;
        return true;
    }
    return false;
}
u8 zdrmp_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    return false;
}
u8 zdrdw_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    if(!is_mapclose())
    {
        *error = B_MPARM; //MPARM
        return true;
    }
    /*
    if(!is_stopperoff())
    {
    *error = B_MPSTP; //MPSTP
    return true;
    }
    */
    return false;
}
u8 zdrdw_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x48; //DLMIT
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    return false;
}
u8 ywait_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_foup_place() && is_drclose() && is_vacuumon())
    {
        *error = B_DVACM; // DVACM
        return true;
    }
    if(is_foup_place() && is_drclose() && is_unlatch())
    {
        *error = B_LTCHU; //UN LATCH
        return true;
    }
    return false;
}
u8 ywait_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 motin_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(!is_dropen())
    {
        *error = 0x48; //DLMIT
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    if(gParkErr)
    {
        *error = 0xB2;
        return true;
    }
    return false;
}
u8 ydoor_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG;
        return true;
    }
    if(is_foup_place() && is_drclose() && is_vacuumoff() && (!is_latch()))
    {
        *error = B_LTCHU;
        return true;
    }
    return false;
}
u8 ydoor_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 dorbk_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_foup_presence() && is_vacuumon() && (!is_unlatch()))
    {
        *error = B_LTCHU;
        return true;
    }
    return false;
}
u8 dorbk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 dorfw_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG;
        return true;
    }
    if(is_vacuumon() && is_latch())
    {
        *error = B_LTCHU;
        return true;
    }
    if(is_foup_place() && is_dock() && is_vacuumoff() && is_latch())
    {
        *error = B_LTCHU;
        return true;
    }
    if(!is_druplmt())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
    return false;
}
u8 dorfw_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 mapop_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    if(!is_mapstart())
    {
        *error = B_MPARM;
        return true;
    }
    return false;
}
u8 mapop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 mapcl_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    return false;
}
u8 mapcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    return false;
}
u8 zmpst_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    if(!is_mapclose())
    {
        *error = B_MPARM;
        return true;
    }
    return false;
}
u8 zmpst_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    return false;
}
u8 zmped_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI;
        return true;
    }
    if(!is_mapclose())
    {
        *error = B_MPARM;
        return true;
    }
    return false;
}
u8 zmped_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if(is_protrusion())
    {
        *error = R_PROTS;
        return true;
    }
    return false;
}
u8 mston_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(!is_druplmt() && (!is_mapstart()))
    {
        *error = B_ZPOSI;
        return true;
    }
    return false;
}
u8 mston_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 mstof_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    return false;
}
u8 mstof_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 orgsh_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR; //ERROR
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN; //SYSIN
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY; //BUSY
        return true;
    }
    /*
    if(is_vacuumon())
    {
        *error = B_DVACM; // DVACM
        return true;
    }
    	*/
    if(!is_foup_place() && (!(is_no_foup() && is_drclose())))
    {
        *error = B_LTCHU; //UN LATCH
        return true;
    }
    return false;
}

u8 sysin_before(u8* error)
{
    init_error();
    init_error();
    if(is_error())
    {
        *error = B_ERROR; //ERROR
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY; //BUSY
        return true;
    }
    return false;
}

u8 sysin_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}

u8 orgsh_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 aborg_before(u8* error)
{
    init_error();
    if(is_error())
    {
        *error = B_ERROR; //ERROR
        return true;
    }
    if(gissysinit == false)
    {
        *error = B_SYSIN;
//       return true;
    }
    return false;
}
u8 aborg_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    return false;
}
u8 cload_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = B_LTCHU; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
    return false;
}
u8 cload_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 clddk_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = B_LTCHU; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
    return false;
}
u8 clddk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 cldyd_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = B_LTCHU; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
    return false;
}
u8 cldyd_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 cldop_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(gEnd_act != CMD_ACTION_CLDDK)
    {
        *error = B_CLDDK; //CLDDK
        return true;
    }
    return false;
}
u8 cldop_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 cldmp_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_latch())
    {
        *error = B_LTCHU; //UN LATCH
        return true;
    }
    if(!is_drclose())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
    if(!is_druplmt())
    {
        *error = B_YPOSI; //ZPOSI up limit
        return true;
    }
    return false;
}
u8 cldmp_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 clmpo_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(gEnd_act != CMD_ACTION_CLDDK)
    {
        *error = B_CLDDK; //CLDDK
        return true;
    }
    if(!is_druplmt())
    {
        *error = B_ZPOSI; //ZPOSI up limit
        return true;
    }
    return false;
}
u8 clmpo_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 culod_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!(is_dock() || is_undock()))
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!(is_unlatch() || is_latch()))
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    return false;
}
u8 culod_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 culdk_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    return false;
}
u8 culdk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 culyd_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    return false;
}
u8 culyd_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 cudcl_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    if(gEnd_act != CMD_ACTION_CULDK)
    {
        *error = B_CULDK; //CULDK
        return true;
    }
    return false;
}
u8 cudcl_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 cudnc_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    if(gEnd_act != CMD_ACTION_CULDK)
    {
        *error = B_CULDK; //CILDK
        return true;
    }
    return false;
}
u8 cudnc_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 culfc_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    return false;
}
u8 culfc_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 mapdo_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(gEnd_act != CMD_ACTION_CLOAD)
    {
        *error = B_CLOAD; //CLOAD
        return true;
    }
    return false;
}
u8 mapdo_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 remap_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(0)
    {
        *error = B_RMPOS; //RMPOS
        return true;
    }
    return false;
}
u8 remap_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTOP;
        return true;
    }
    return false;
}
u8 cudmp_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
//    if(!is_mapopen())
    if(!is_mapclose())
    {
        *error = B_MPARM; //MPARM
        return true;
    }
    return false;
}
u8 cudmp_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 cumdk_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
//    if(!is_mapopen())
    if(!is_mapclose())
    {
        *error = B_MPARM; //MPARM
        return true;
    }
    return false;
}
u8 cumdk_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}
u8 cumfc_before(u8* error)
{
    if(is_error())
    {
        *error = B_ERROR;
        return true;
    }
    if(is_busy())
    {
        *error = B_CBUSY;
        return true;
    }
    if(is_origin == false)
    {
        *error = B_ORGYT; //ORGYT
        return true;
    }
    if(!is_foup_place())
    {
        *error = B_FPILG; //FPILG
        return true;
    }
    if(!is_dock())
    {
        *error = B_YPOSI; //YPOSI undock
        return true;
    }
    if(!is_unlatch())
    {
        *error = B_LTCHU; //LATCH
        return true;
    }
    if(!is_dropen())
    {
        *error = B_DPOSI; //DPOSI
        return true;
    }
//   if(!is_mapopen())
    if(!is_mapclose())
    {
        *error = B_MPARM; //MPARM
        return true;
    }
    return false;
}
u8 cumfc_running(u8* error)
{
    if(is_obstacle())
    {
        *error = R_SAFTY;
        return true;
    }
    if(is_noair())
    {
        *error = R_AIRSN;
        return true;
    }
    if(is_fanerr())
    {
        *error = R_FNAST;
        return true;
    }
    if((!is_no_foup()) && (!is_foup_place()))
    {
        *error = R_INTCL;
        return true;
    }
    return false;
}

u8 clam_sta(void)
{
    if(is_clampup())
    {
        return '1';
    }
    if(is_clampdown())
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
    if(INPUT_ReadOne(CS_I_12, VS01_12) == 1)
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
    if(is_druplmt() == true)
    {
        return '1';
    }
    if(is_drdwlmt() == true)
    {
        return '0';
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
    if(gIsError == false)//gCur_status != G_CUR_STA_ERR)
    {
        return '0';
    }
    else
    {
        if(gErrorLevel == ERR_REC)
        {
            return 'E';
        }
        if(gErrorLevel == ERR_FAT)
        {
            return 'A';
        }
    }
}

u8 get_mapzpos(void)
{
    if(is_mapstart() == true)
    {
        return '1';
    }
    if(is_mapend() == true)
    {
        return '0';
    }
    return '?';
}

u8 map_apos(void)
{
    if(INPUT_ReadOne(CS_I_10, CLS08B_10) == 1)
    {
        return '1';
    }
    if(INPUT_ReadOne(CS_I_10, CLS08A_10) == 1)
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
u8 present_st(void)
{
	if(is_foup_presence() && is_foup_place())
	{
		return '1';
	}
	if(is_no_foup())
	{
		return '0';
	}
	return '2';
	
}


