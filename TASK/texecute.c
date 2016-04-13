#include "tmotion.h"
#include "motor.h"
#include "texecute.h"
#include "includes.h"
#include "tinput.h"
#include "output.h"
#include "tcmd.h"

#define WAFER_THICK 100
#define WTHICK_MARG 10

#define WPOS_MARGIN   10
#define WPOS_START    2000
#define WPOS_INTERVAL 1000

#define WAFER_NUM 25

#define INSLOT_HH  4
#define INSLOT_HI  3
#define INSLOT_LI  2
#define INSLOT_LL  1
#define INSLOT_ERR 99

#define PROC_ERR 0
#define PROC_ING 1
#define PROC_END 2
#define PROC_UNS 4

u8 scan_mode = SCAN_UPP;
u8 gCur_action = CMD_ACTION_PODOP;
u8 gCur_pause = 0;
u8 gCur_stop = 0;
u8 gCur_abort = 0;
u8 gCur_retry = 0;
u8 gErr_no = 0;
u8 gMap_status = 0;

bool gErr_mod = false;
bool gIs_init = false;
bool gis_scan = false;



u8 exe_clamup(bool bforce)
{
    if((!is_sf_setfoupd()) && (!bforce) && (!is_sf_clamupd()) && (!is_sf_clamupi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL01B_1);
    OUTPUT_SetOne(CS_O_1,SOL01A_1);
    if(is_sf_clamupd())
    {
        return PROC_END;
    }
    if(is_sf_clamupi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_clamfwd(bool bforce)
{
    if((!is_sf_clamupd()) && (!bforce) && (!is_sf_clanfwdd()) && (!is_sf_clanfwdi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL02B_1);
    OUTPUT_SetOne(CS_O_1,SOL02A_1);
    if(is_sf_clanfwdd())
    {
        return PROC_END;
    }
    if(is_sf_clanfwdi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_clamlck(bool bforce)
{
    if((!is_sf_clanfwdd()) && (!bforce) && (!is_sf_clamlckd()) && (!is_sf_clamlcki()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL01A_1);
    OUTPUT_SetOne(CS_O_1,SOL01B_1);
    if(is_sf_clamlckd())
    {
        return PROC_END;
    }
    if(is_sf_clamlcki())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_fpdck(bool bforce)
{
    if((!is_sf_clamlckd()) && (!bforce) && (!is_sf_fpdckd()) && (!is_sf_fpdcki()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL03B_1);
    OUTPUT_SetOne(CS_O_1,SOL03A_1);
    if(is_sf_fpdckd())
    {
        return PROC_END;
    }
    if(is_sf_fpdcki())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_dradsp(bool bforce)
{
    if((!is_sf_fpdckd()) && (!bforce) && (!is_sf_dradspd()) && (!is_sf_dradspi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_2,SOL09B_2);
    OUTPUT_SetOne(CS_O_2,SOL09A_2);
    if(is_sf_dradspd())
    {
        return PROC_END;
    }
    if(is_sf_dradspi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drunlt(bool bforce)
{
    if((!is_sf_dradspd()) && (!bforce) && (!is_sf_drunltd()) && (!is_sf_drunlti()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL04B_1);
    OUTPUT_SetOne(CS_O_1,SOL04A_1);
    if(is_sf_drunltd())
    {
        return PROC_END;
    }
    if(is_sf_drunlti())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drop(bool bforce)
{
    if((!is_sf_drunltd()) && (!bforce) && (!is_sf_dropd()) && (!is_sf_dropi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_2,SOL05B_2);
    OUTPUT_SetOne(CS_O_2,SOL05A_2);
    if(is_sf_dropd())
    {
        return PROC_END;
    }
    if(is_sf_dropi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drdwns(bool bforce)
{
    if((!is_sf_dropd()) && (!bforce) && (!is_sf_drdwnsd()) && (!is_sf_drdwnsi()))
    {
        return PROC_UNS;
    }
    START_Motion(M_STRMP, M_VEL);
    if(is_sf_drdwnsd())
    {
        return PROC_END;
    }
    if(is_sf_drdwnsi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_mpaop(bool bforce)
{
    if((!is_sf_drdwnsd()) && (!bforce) && (!is_sf_mpaopd()) && (!is_sf_mpaopi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_0,SOL08B_0);
    OUTPUT_SetOne(CS_O_0,SOL08A_0);
    if(is_sf_mpaopd())
    {
        return PROC_END;
    }
    if(is_sf_mpaopi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_stpon(bool bforce)
{
    if((!is_sf_mpaopd()) && (!bforce) && (!is_sf_stpond()) && (!is_sf_stponi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_0,SOL07B_0);
    OUTPUT_SetOne(CS_O_0,SOL07A_0);
    if(is_sf_stpond())
    {
        return PROC_END;
    }
    if(is_sf_stponi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drdwne(bool bforce)
{
    if((!is_sf_stpond()) && (!bforce) && (!is_sf_drdwned()) && (!is_sf_drdwnei()))
    {
        return PROC_UNS;
    }
    START_Motion(M_STPMP, M_VEL);
    if(is_sf_drdwned())
    {
        return PROC_END;
    }
    if(is_sf_drdwnei())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_mpac(bool bforce)
{
    if((!is_sf_drdwned()) && (!bforce) && (!is_sf_mpacd()) && (!is_sf_mpaci()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_0,SOL08A_0);
    OUTPUT_SetOne(CS_O_0,SOL08B_0);
    if(is_sf_mpacd())
    {
        return PROC_END;
    }
    if(is_sf_mpaci())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_stpoff(bool bforce)
{
    if((!is_sf_mpacd()) && (!bforce) && (!is_sf_stpoffd()) && (!is_sf_stpoffi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_0,SOL07A_0);
    OUTPUT_SetOne(CS_O_0,SOL07B_0);
    if(is_sf_stpoffd())
    {
        return PROC_END;
    }
    if(is_sf_stpoffi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drdwnl(bool bforce)
{
    if((!is_sf_stpoffd()) && (!bforce) && (!is_sf_drdwnld()) && (!is_sf_drdwnli()))
    {
        return PROC_UNS;
    }
    START_Motion(M_DNLMT, M_VEL);
    if(is_sf_drdwnld())
    {
        return PROC_END;
    }
    if(is_sf_drdwnli())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drupl(bool bforce)
{
    if((!is_sf_drdwnld()) && (!bforce) && (!is_sf_drupld()) && (!is_sf_drupli()))
    {
        return PROC_UNS;
    }
    START_Motion(M_UPLMT, M_VEL);
    if(is_sf_drupld())
    {
        return PROC_END;
    }
    if(is_sf_drupli())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drcls(bool bforce)
{
    if((!is_sf_drupld()) && (!bforce) && (!is_sf_drclsd()) && (!is_sf_drclsi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_2,SOL05A_2);
    OUTPUT_SetOne(CS_O_2,SOL05B_2);
    if(is_sf_drclsd())
    {
        return PROC_END;
    }
    if(is_sf_drclsi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_drlt(bool bforce)
{
    if((!is_sf_drclsd()) && (!bforce) && (!is_sf_drltd()) && (!is_sf_drlti()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL04A_1);
    OUTPUT_SetOne(CS_O_1,SOL04B_1);
    if(is_sf_drltd())
    {
        return PROC_END;
    }
    if(is_sf_drlti())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_dradsr(bool bforce)
{
    if((!is_sf_drltd()) && (!bforce) && (!is_sf_drclsd()) && (!is_sf_drclsi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_2,SOL09B_2);
    OUTPUT_SetOne(CS_O_2,SOL09A_2);
    if(is_sf_dradsrd())
    {
        return PROC_END;
    }
    if(is_sf_drclsi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_fpundk(bool bforce)
{
    if((!is_sf_dradsrd()) && (!bforce) && (!is_sf_fpundkd()) && (!is_sf_fpundki()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL03A_1);
    OUTPUT_SetOne(CS_O_1,SOL03B_1);
    if(is_sf_fpundkd())
    {
        return PROC_END;
    }
    if(is_sf_fpundki())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_clamulk(bool bforce)
{
    if((!is_sf_fpundkd()) && (!bforce) && (!is_sf_clamulkd()) && (!is_sf_clamulki()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL01B_1);
    OUTPUT_SetOne(CS_O_1,SOL01A_1);
    if(is_sf_clamulkd())
    {
        return PROC_END;
    }
    if(is_sf_clamulki())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_clambwd(bool bforce)
{
    if((!is_sf_clamulkd()) && (!bforce) && (!is_sf_clambwdd()) && (!is_sf_clambwdi()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL02A_1);
    OUTPUT_SetOne(CS_O_1,SOL02B_1);
    if(is_sf_clambwdd())
    {
        return PROC_END;
    }
    if(is_sf_clambwdi())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 exe_clamdwn(bool bforce)
{
    if((!is_sf_clambwdd()) && (!bforce) && (!is_sf_clamdwnd()) && (!is_sf_clamdwni()))
    {
        return PROC_UNS;
    }
    OUTPUT_ResetOne(CS_O_1,SOL01A_1);
    OUTPUT_SetOne(CS_O_1,SOL01B_1);
    if(is_sf_clamdwnd())
    {
        return PROC_END;
    }
    if(is_sf_clamdwni())
    {
        return PROC_ING;
    }
    return PROC_ERR;
}

u8 run_clamup(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL01B_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL01A_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL01A_1);
    }
    return 0;
}

u8 run_clamfwd(bool pause)
{

    OUTPUT_ResetOne(CS_O_1,SOL02B_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL02A_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL02A_1);
    }
    return 0;
}

u8 run_clamlck(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL01A_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL01B_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL01B_1);
    }
    return 0;
}

u8 run_fpdck(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL03B_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL03A_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL03A_1);
    }
    return 0;
}

u8 run_dradsp(bool pause)
{
    OUTPUT_ResetOne(CS_O_2,SOL09B_2);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_2,SOL09A_2);
    }
    else
    {
        OUTPUT_SetOne(CS_O_2,SOL09A_2);
    }
    return 0;
}

u8 run_drunlt(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL04B_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL04A_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL04A_1);
    }
    return 0;
}

u8 run_drop(bool pause)
{
    OUTPUT_ResetOne(CS_O_2,SOL05B_2);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_2,SOL05A_2);
    }
    else
    {
        OUTPUT_SetOne(CS_O_2,SOL05A_2);
    }
    return 0;
}

u8 run_drdwns(bool pause)
{
    if(pause)
    {
    }
    else
    {
        START_Motion(M_STRMP, M_VEL);
    }
    return 0;
}

u8 run_mpaop(bool pause)
{
    OUTPUT_ResetOne(CS_O_0,SOL08B_0);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_0,SOL08A_0);
    }
    else
    {
        OUTPUT_SetOne(CS_O_0,SOL08A_0);
    }
    return 0;
}

u8 run_stpon(bool pause)
{
    OUTPUT_ResetOne(CS_O_0,SOL07B_0);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_0,SOL07A_0);
    }
    else
    {
        OUTPUT_SetOne(CS_O_0,SOL07A_0);
    }
    return 0;
}

u8 run_drdwne(bool pause)
{
    if(pause)
    {
    }
    else
    {
        START_Motion(M_STPMP, M_VEL);
    }
    return 0;
}

u8 run_mpac(bool pause)
{
    OUTPUT_ResetOne(CS_O_0,SOL08A_0);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_0,SOL08B_0);
    }
    else
    {
        OUTPUT_SetOne(CS_O_0,SOL08B_0);
    }
    return 0;
}

u8 run_stpoff(bool pause)
{
    OUTPUT_ResetOne(CS_O_0,SOL07A_0);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_0,SOL07B_0);
    }
    else
    {
        OUTPUT_SetOne(CS_O_0,SOL07B_0);
    }
    return 0;
}

u8 run_drdwnl(bool pause)
{
    if(pause)
    {
    }
    else
    {
        START_Motion(M_DNLMT, M_VEL);
    }
    return 0;
}

u8 run_drupl(bool pause)
{
    if(pause)
    {
    }
    else
    {
        START_Motion(M_UPLMT, M_VEL);
    }
    return 0;
}

u8 run_drcls(bool pause)
{
    OUTPUT_ResetOne(CS_O_2,SOL05A_2);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_2,SOL05B_2);
    }
    else
    {
        OUTPUT_SetOne(CS_O_2,SOL05B_2);
    }
    return 0;
}

u8 run_drlt(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL04A_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL04B_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL04B_1);
    }
    return 0;
}

u8 run_dradsr(bool pause)
{
    OUTPUT_ResetOne(CS_O_2,SOL09B_2);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_2,SOL09A_2);
    }
    else
    {
        OUTPUT_SetOne(CS_O_2,SOL09A_2);
    }
    return 0;
}

u8 run_fpundk(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL03A_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL03B_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL03B_1);
    }
    return 0;
}

u8 run_clamulk(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL01B_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL01A_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL01A_1);
    }
    return 0;
}

u8 run_clambwd(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL02A_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL02B_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL02B_1);
    }
    return 0;
}

u8 run_clamdwn(bool pause)
{
    OUTPUT_ResetOne(CS_O_1,SOL01A_1);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_1,SOL01B_1);
    }
    else
    {
        OUTPUT_SetOne(CS_O_1,SOL01B_1);
    }
    return 0;
}

u8 get_init(void)
{
    if(gIs_init == true)
    {
        return '1';
    }
    else
    {
        return '0';
    }
}

u8 get_oper(void)
{
    if(gCUr_status == G_CUR_STA_RUN || \
            gCUr_status == G_CUR_STA_PAU || \
            gCUr_status == G_CUR_STA_RSM || \
            gCUr_status == G_CUR_STA_INT)
    {
        return '1';
    }
    else
    {
        return '0';
    }
}

u8 get_mapsts(void)
{
    if(gMap_status == 0x01)
    {
        return '1';
    }
    if(gMap_status == 0x02)
    {
        return '2';
    }
    return '0';
}

u8 is_inslot(s32 cpos, u8 slot)
{
    if(cpos > (WPOS_START + WPOS_INTERVAL * (slot - 1) + WAFER_THICK + WTHICK_MARG))
    {
        return INSLOT_HH;
    }

    if((cpos > (WPOS_START + WPOS_INTERVAL * (slot - 1) + WAFER_THICK - WTHICK_MARG)) && (cpos < (WPOS_START + WPOS_INTERVAL * (slot - 1) + WAFER_THICK + WTHICK_MARG)))
    {
        return INSLOT_HI;
    }
    if((cpos > (WPOS_START + WPOS_INTERVAL * (slot - 1) - WPOS_MARGIN)) && (cpos < (WPOS_START + WPOS_INTERVAL * (slot - 1) + WPOS_MARGIN)))
    {
        return INSLOT_LI;
    }
    if(cpos < (WPOS_START + WPOS_INTERVAL * (slot - 1) - WPOS_MARGIN))
    {
        return INSLOT_LL;
    }
    return INSLOT_ERR;
}

u8 is_inthick(s32 height)
{
    if(height > WAFER_THICK + WTHICK_MARG)
    {
        return INSLOT_HH;
    }
    if(height < WAFER_THICK - WTHICK_MARG)
    {
        return INSLOT_LL;
    }
    return INSLOT_HI;
}
bool Analyze_Scan(u8* result)
{
    u8 cntp = 0;
    u8 cnts = 0;
//	u8 p1,p2,p3,p4,p5;
    u8 p1,p2,p4,p5;
    memset(result, 0, WAFER_NUM);
    if(gis_scan == false)
    {
 //      return false;
    }
    while(cntp < gScan_num)
    {
        p1 = is_inslot(gScan_pos[cntp], WAFER_NUM - cnts);
        p2 = is_inslot(gScan_pos[cntp+1], WAFER_NUM - cnts);
        p5 = is_inthick(gScan_pos[cntp] - gScan_pos[cntp+1]);
        if(cnts < WAFER_NUM)
        {
//			p3 = is_inslot(gScan_pos[cntp+2], WAFER_NUM - cnts - 1);
            p4 = is_inslot(gScan_pos[cntp+3], WAFER_NUM - cnts - 1);
        }
        if((p1 >= INSLOT_HI) && (p2 == INSLOT_LI))
        {
            if(p5 == INSLOT_HI)
            {
                if(*(result+cnts) == 0)
                {
                    *(result+cnts) = 1;// one wafer
                }
                else
                {
                    *(result+cnts) = 5;
                }
                cntp += 2;
                cnts += 1;
                continue;
            }
            if(p5 == INSLOT_HH)
            {
                if(*(result+cnts) == 0)
                {
                    *(result+cnts) = 2;// two wafers
                }
                else
                {
                    *(result+cnts) = 5;
                }
                cntp += 2;
                cnts += 1;
                continue;
            }
            if(p5 == INSLOT_LL)
            {
                *(result+cnts) = 5;
                cntp += 2;
                cnts += 1;
                continue;
            }
        }
        if((p1 == INSLOT_LL) && (p2 == INSLOT_LL))
        {
            if(cnts < WAFER_NUM - 1)
            {
//				p3 = is_inslot(gScan_pos[cntp+2], WAFER_NUM - cnts - 1);
                p4 = is_inslot(gScan_pos[cntp+1], WAFER_NUM - cnts - 1);
                if(p4 > INSLOT_LI)
                {
                    *(result+cnts) = 3; //cross
                    *(result+cnts+1) = 3;
                    cntp += 2;
                    cnts += 2;
                    continue;
                }
                else
                {
                    *(result+cnts) = 0; //no wafer
//                   cntp += 2;
                    cnts += 1;
                    continue;
                }
            }
            else
            {
                *(result+cnts) = 3; //cross
                cntp += 2;
                cnts += 1;
                continue;
            }
        }
        cntp += 2;
    }
    if(cnts < WAFER_NUM - 1)
    {
        memset(result+cnts, 0, WAFER_NUM - cnts - 1);
    }
    return true;
}


void set_errno(u8 cmd, u8 errno)
{
    switch(errno)
    {
    case 0x27:
    case 0x12:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0xA1:
    case 0xA2:
    case 0xA3:
    case 0xA4:
    case 0xFC:
    case 0xFD:
    case 0xFE:
    case 0xFF:
        gErr_no = errno;
        return;
    }
    if(gErr_mod)
    {
        switch (cmd)
        {
        case CMD_ACTION_PODOP:
            gErr_no = errno;
            break;
        }
    }
    else
    {
        switch (cmd)
        {
        case CMD_ACTION_PODOP:
            gErr_no =0x21;
            break;
        }
    }
}

#define ACT_ERR  0x00
#define ACT_END  0x01
#define ACT_STP  0x02
#define ACT_ABT  0x03

u8 podop_action(u8* error)
{
    u8 time = 30;
    u8 seq = 0;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(podop_running(error) == true)
        {
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_clamulk(gCur_pause);
            *error = 0x61;
            if(is_clampup())
            {
                seq = 0x02;
            }
            break;
        case 0x02:
            run_clambwd(gCur_pause);
            *error = 0x21;
            if(is_clampbwd())
            {
                seq = 0x03;
            }
            break;
        case 0x03:
            run_clamdwn(gCur_pause);
            *error = 0x62;
            if(is_clampdown())
            {
                return ACT_END;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {

            OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}

u8 podcl_action(u8* error)
{
    u8 time = 30;
    u8 seq = 0;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(podcl_running(error) == true)
        {
            return 0x00;
        }
        switch (seq)
        {
        case 0x01:
            run_clamup(gCur_pause);
            if(is_clampup())
            {
                seq = 0x02;
            }
            break;
        case 0x02:
            run_clamfwd(gCur_pause);
            if(is_clampfwd())
            {
                seq = 0x03;
            }
            break;
        case 0x03:
            run_clamlck(gCur_pause);
            if(is_clamplock())
            {
                return 0x01;
            }
            break;
        }
        if(gCur_stop == 0x01)
        {
            return 0x01;
        }
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            if(gCur_stop == 0x01)
            {
                return 0x01;
            }
            OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
        }
    }
    return 0x00;
}

void tExe_Action(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    u8 error;
    u8 ret = 0;
    u8 param[10];
    while(1)
    {
        if(gCUr_status == G_CUR_STA_ERR || \
                gCUr_status == G_CUR_STA_INT || \
                gCUr_status == G_CUR_STA_NAC || \
                gCUr_status == G_CUR_STA_UNI || \
                gCUr_status == G_CUR_STA_ABO || \
                gCUr_status == G_CUR_STA_STP)
        {
            OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
            continue;
        }
        gCur_action = gCmd_action;
        switch (gCur_action)
        {
        case CMD_ACTION_NOACT:
            break;
        case CMD_ACTION_PODOP:
            ret = podop_action(&error);
            if(ret == ACT_END)
            {
                send_msg(gCom_mod & BCAK_FIN, (char*)"PODOP", (u8*)NULL, 0);
                gCUr_status = G_CUR_STA_END;
                gEnd_act = CMD_ACTION_PODOP;
            }
            else if(ret == ACT_ERR)
            {
                switch (error)
                {
                case 0xFF:
                    memcpy(param, (char*)"/SAFTY", 6);
                    break;
                case 0x27:
                    memcpy(param, (char*)"/AIRSN", 6);
                    break;
                case 0xFC:
                    memcpy(param, (char*)"/FNAST", 6);
                    break;
                case 0x61:
                case 0x21:
                case 0x62:
                    memcpy(param, (char*)"/CLOPS", 6);
                    break;
                }
                set_errno(CMD_ACTION_PODOP, error);
                send_msg(gCom_mod & BCAK_ABS, (char*)"PODOP", param, 6);
                gCUr_status = G_CUR_STA_ERR;
                gEnd_act = CMD_ACTION_NOACT;
            }
            else if(ret == ACT_ABT)
            {
                send_msg(gCom_mod & BCAK_FIN, (char*)"PODOP", param, 6);
                gCUr_status = G_CUR_STA_ABO;
                gEnd_act = CMD_ACTION_NOACT;
            }
            else if(ret == ACT_STP)
            {
                send_msg(gCom_mod & BCAK_FIN, (char*)"PODOP", param, 6);
                gCUr_status = G_CUR_STA_STP;
                gEnd_act = CMD_ACTION_NOACT;
            }
            break;
        case CMD_ACTION_PODCL:
            podcl_action(&error);
            break;
        }
    }
}
