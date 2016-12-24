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
#define WPOS_FIRST    1000
#define WPOS_START    32000
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

#define ACT_ERR  0x00
#define ACT_END  0x01
#define ACT_STP  0x02
#define ACT_ABT  0x03
#define ACT_NAT  0x04
#define ACT_STA  0x05

#define CIRCLR_TIME 900

u8 scan_mode = SCAN_UPP;
u8 gCur_action = CMD_ACTION_PODOP;
u8 gCur_pause = 0;
u8 gCur_stop = 0;
u8 gCur_abort = 0;
u8 gCur_retry = 0;
u8 gErr_no = 0;
u8 gMap_status = 0;
u8 pod_s = CMD_ACTION_NOACT;

bool gIsError = false;
bool gErr_mod = false;
bool gIs_init = false;
bool gis_scan = false;
bool is_origin = false;
//bool is_origin = true;
bool is_aborg = false;


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

u8 run_clamldwn(bool pause)
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
        PAUSE_Motion(M_STRMP);
    }
    else
    {
        MOTOR_GO(MW_UPM, M_VEL);
//       START_Motion(M_STRMP, M_VEL);
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
        PAUSE_Motion(M_STPMP);
    }
    else
    {
        MOTOR_GO(MW_DNM, M_VEL);
//       START_Motion(M_STPMP, M_VEL);
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
        PAUSE_Motion(M_DNLMT);
    }
    else
    {
        MOTOR_GO(MW_DNL, M_VEL);
//        START_Motion(M_DNLMT, M_VEL);
    }
    return 0;
}

u8 run_drupl(bool pause)
{
    if(pause)
    {
        PAUSE_Motion(M_UPLMT);
    }
    else
    {
        MOTOR_GO(MW_UPL, M_VEL);
//       START_Motion(M_UPLMT, M_VEL);
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
    OUTPUT_ResetOne(CS_O_2,SOL09A_2);
    if(pause)
    {
        OUTPUT_ResetOne(CS_O_2,SOL09B_2);
    }
    else
    {
        OUTPUT_SetOne(CS_O_2,SOL09B_2);
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
u8 run_motin(bool pause)
{
    OS_ERR err;
    if(pause)
    {
        gstopmotin = true;
//			gMotor_state = MS_UNINIT;
//       PAUSE_Motion(M_UPLMT);
    }
    else
    {       
        if(gismoting == false) {
					gismotinit = false;
            OSTaskResume(&MINIT_TaskTCB,&err);
        }
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
    if(gCur_status == G_CUR_STA_RUN || \
            gCur_status == G_CUR_STA_PAU || \
            gCur_status == G_CUR_STA_RSM || \
            gCur_status == G_CUR_STA_INT)
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

bool slot_dect(s32 up, s32 down, u8* result)
{
    u8 i = 0;
    for(i=0; i<WAFER_NUM; i++)
    {
        if((up < (WPOS_START + WPOS_FIRST + WPOS_INTERVAL * (WAFER_NUM - i)))) //&& (down > (WPOS_START + WPOS_FIRST + WPOS_INTERVAL * (WAFER_NUM - i -1)) - WPOS_MARGIN))
        {
            if(down > (WPOS_START + WPOS_FIRST + WPOS_INTERVAL * (WAFER_NUM - i -1)) + WAFER_THICK + WTHICK_MARG)
            {
                if(i > 0)
                {
                    *(result + WAFER_NUM - i - 1) = 3; //cross
                    *(result + WAFER_NUM - i) = 3;
                }
                else
                {
                    *(result + WAFER_NUM - i - 1) = 3;
                }
                return true;
            }
            if(down > (WPOS_START + WPOS_FIRST + WPOS_INTERVAL * (WAFER_NUM - i -1)) - WPOS_MARGIN)
            {
                if((up - down) > (WAFER_THICK + WTHICK_MARG))
                {
                    *(result + WAFER_NUM - i - 1) = 2;// two wafers
                }
                else
                {
                    *(result  + WAFER_NUM - i - 1) = 1;// one wafer
                }
                return true;
            }
            *(result + WAFER_NUM - i - 1) = 3; //cross
            *(result + WAFER_NUM - i - 2) = 3; //cross
            return false;
        }
        if(up < (WPOS_START + WPOS_FIRST))
        {
            *(result + WAFER_NUM - i) = 3; //cross
            return false;
        }
    }
    return false;
}

bool Analyze_Scan(u8* result)
{
    u8 cntp = 0;
    u8 cnts = 0;
//	u8 p1,p2,p3,p4,p5;
    u8 p1,p2,p4,p5;
    bool ret = false;
    memset(result, 0, WAFER_NUM);
    if(gis_scan == false)
    {
//      return false;
    }
    while(cntp < gScan_num)
    {
        ret = slot_dect(gScan_pos[cntp], gScan_pos[cntp+1], result);
        cntp += 2;
    }
    return ret;
}

bool Analyze_ScanBK(u8* result)
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
    case R_PROTS:
    case R_AIRSN:
    case R_MPEDL:
    case R_DROPS:
    case R_DRCLS:
    case R_VACCS:
    case R_VACOS:
    case R_INTOP:
    case R_INTCL:
    case R_RESVE:
    case R_PARER:
    case R_FNAST:
    case R_INLKE:
    case R_COMER:
    case R_SAFTY:
		case R_SYSIN:
        gErr_no = errno;
        return;
    }
    gErr_no = errno;

    if(!gErr_mod)
    {
        switch (gErr_no)
        {
        case R_ZLMITU:
            gErr_no = R_ZLMITD;
            break;
        case R_DLMITC:
            gErr_no = R_DLMITO;
            break;
        case R_MPBARC:
            gErr_no = R_MPBARO;
            break;
        case R_MPZLMU:
            gErr_no = R_MPZLMD;
            break;
        case R_MPSTPF:
            gErr_no = R_MPSTPN;
            break;
        case R_CLOPSU:
            if(pod_s == CMD_ACTION_PODOP)
            {
                gErr_no = R_CLOPS;
            }
            if(pod_s == CMD_ACTION_PODCL)
            {
                gErr_no = R_CLCLS;
            }
            break;
        case R_CLOPSD:
            if(pod_s == CMD_ACTION_PODOP)
            {
                gErr_no = R_CLOPS;
            }
            if(pod_s == CMD_ACTION_PODCL)
            {
                gErr_no = R_CLCLS;
            }
            break;
        case CMD_ACTION_YWAIT:
            gErr_no = R_YLMITD;
            break;
        case CMD_ACTION_MSTON:
            gErr_no = R_MPSTPN;
            break;
        case CMD_ACTION_MSTOF:
            gErr_no = R_MPSTPN;
            break;
        case CMD_ACTION_ZDRUP:
            gErr_no = R_ZLMITD;
            break;
        case CMD_ACTION_ZDRDW:
            gErr_no = R_ZLMITD;
            break;
        case CMD_ACTION_ZDRMP:
            gErr_no = R_MPZLMD;
            break;
        case CMD_ACTION_ZMPST:
            gErr_no = R_MPZLMD;
            break;
        }
    }
}

u8 noact_action(u8* error)
{
    OS_ERR err;
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
    return ACT_NAT;
}


u8 podop_action(u8* error)
{
    u8 time = 30;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    pod_s = CMD_ACTION_PODOP;
    while(time--)
    {
        if(podop_running(error) == true)
        {
            run_clamldwn(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_clamldwn(gCur_pause);
            *error = R_CLOPSU;
            if(is_clampdown())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_clamldwn(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            switch (seq)
            {
            case 0x01:
                run_clamldwn(gCur_pause);
                break;
            }
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            switch (seq)
            {
            case 0x01:
                run_clamldwn(gCur_pause);
                break;
            }
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            switch (seq)
            {
            case 0x01:
                run_clamldwn(gCur_pause);
                break;
            }
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}


u8 podcl_action(u8* error)
{
    u8 time = 30;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    pod_s = CMD_ACTION_PODCL;
    while(time--)
    {
        if(podcl_running(error) == true)
        {
            run_clamup(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_clamup(gCur_pause);
            *error = R_CLOPSU;
            if(is_clampup())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_clamup(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            switch (seq)
            {
            case 0x01:
                run_clamup(gCur_pause);
                break;
            }
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            switch (seq)
            {
            case 0x01:
                run_clamup(gCur_pause);
                break;
            }
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            switch (seq)
            {
            case 0x01:
                run_clamup(gCur_pause);
                break;
            }
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}

u8 vacon_action(u8* error)
{
    u8 time = 50;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(vacon_running(error) == true)
        {
            run_dradsp(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_dradsp(gCur_pause);
            *error = R_VACCS;
            if(is_vacuumon())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_dradsp(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_dradsp(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_dradsp(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_dradsp(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}

u8 vacof_action(u8* error)
{
    u8 time = 50;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(vacof_running(error) == true)
        {
            run_dradsr(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_dradsr(gCur_pause);
            *error = R_VACOS;
            if(is_vacuumoff())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_dradsr(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_dradsr(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_dradsr(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_dradsr(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}

u8 dorop_action(u8* error)
{
    u8 time = 30;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(dorop_running(error) == true)
        {
            run_drunlt(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drunlt(gCur_pause);
            *error = R_DROPS;
            if(is_unlatch())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drunlt(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drunlt(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drunlt(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drunlt(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 dorcl_action(u8* error)
{
    u8 time = 30;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(dorcl_running(error) == true)
        {
            run_drlt(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drlt(gCur_pause);
            *error = R_DRCLS;
            if(is_latch())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drlt(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drlt(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drlt(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drlt(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 mapop_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(mapop_running(error) == true)
        {
            run_mpaop(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_mpaop(gCur_pause);
            *error = R_MPBARO;
            if(is_mapopen())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_mpaop(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_mpaop(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_mpaop(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_mpaop(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 mapcl_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(mapcl_running(error) == true)
        {
            run_mpac(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_mpac(gCur_pause);
            *error = R_MPBARC;
            if(is_mapclose())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_mpac(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_mpac(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_mpac(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_mpac(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 dorbk_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(dorbk_running(error) == true)
        {
            run_drop(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drop(gCur_pause);
            *error = R_DLMITO;
            if(is_dropen())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drop(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drop(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drop(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drop(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 dorfw_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(dorfw_running(error) == true)
        {
            run_drcls(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drcls(gCur_pause);
            *error = R_DLMITC;
            if(is_drclose())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drcls(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drcls(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drcls(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drcls(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 ydoor_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(ydoor_running(error) == true)
        {
            run_fpdck(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_fpdck(gCur_pause);
            *error = R_YLMITD;
            if(is_dock() && is_drondr())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_fpdck(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_fpdck(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_fpdck(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_fpdck(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 ywait_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(ywait_running(error) == true)
        {
            run_fpundk(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_fpundk(gCur_pause);
            *error = R_YLMITU;
            if(is_undock())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_fpundk(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_fpundk(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_fpundk(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_fpundk(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 mston_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(mston_running(error) == true)
        {
            run_stpon(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_stpon(gCur_pause);
            *error = R_MPSTPN;
            if(is_stopperon())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_stpon(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_stpon(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_stpon(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_stpon(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 mstof_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(mstof_running(error) == true)
        {
            run_stpoff(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_stpoff(gCur_pause);
            *error = R_MPSTPF;
            if(is_stopperoff())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_stpoff(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_stpoff(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_stpoff(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_stpoff(gCur_abort);
            return ACT_ABT;
        }
        return ACT_ABT;
    }
    return ACT_ERR;
}
u8 zmpst_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(zmpst_running(error) == true)
        {
            run_drdwns(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drdwns(gCur_pause);
            *error = R_MPZLMU;
            if(is_mapstart())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drdwns(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drdwns(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drdwns(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drdwns(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 zmped_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(zmped_running(error) == true)
        {
            run_drupl(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drupl(gCur_pause);
            *error = R_MPEDL;
            if(is_druplmt())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drupl(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drupl(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drupl(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drupl(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 zdrmp_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(zdrmp_running(error) == true)
        {
            run_drdwne(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drdwne(gCur_pause);
            *error = R_MPZLMD;
            if(is_mapend())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drdwne(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drdwne(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drdwne(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drdwne(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 zdrdw_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(zdrdw_running(error) == true)
        {
            run_drdwnl(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drdwnl(gCur_pause);
            *error = R_ZLMITD;
            if(is_drdwlmt())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drdwnl(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drdwnl(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drdwnl(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drdwnl(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 zdrup_action(u8* error)
{
    u8 time = 100;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(zdrup_running(error) == true)
        {
            run_drupl(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_drupl(gCur_pause);
            *error = R_ZLMITU;
            if(is_druplmt())
            {
                return ACT_END;
            }
            if(time == 0) {
                run_drupl(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_drupl(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_drupl(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_drupl(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}
u8 motin_action(u8* error)
{
    u8 time = 200;
    u8 seq = 0;
    bool errstop = true;
    OS_ERR err;
    seq = 0x01;
    while(time--)
    {
        if(motin_running(error) == true)
        {
            run_motin(errstop);
            return ACT_ERR;
        }
        switch (seq)
        {
        case 0x01:
            run_motin(gCur_pause);
            *error = R_SYSIN;
            if(gismotinit == true)
            {
                return ACT_END;
            }
            if(time == 0) {
                run_motin(errstop);
                return ACT_ERR;
            }
            break;
        }
        OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        while(gCur_pause == 0x01)
        {
            run_motin(gCur_pause);
            OSTimeDlyHMSM(0,0,0,CIRCLR_TIME,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        if(gCur_stop == 0x01)
        {
            run_motin(gCur_stop);
            return ACT_STP;
        }
        if(gCur_abort == 0x01)
        {
            run_motin(gCur_abort);
            return ACT_ABT;
        }
    }
    return ACT_ERR;
}

u8 orgsh_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(orgsh_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = mapcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zmped_action(error);
            seq = 0x04;
            break;
        case 0x03:
            ret = mstof_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = dorbk_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = zdrup_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = dorfw_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = dorcl_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = vacof_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = ywait_action(error);
            seq = 0x10;
            break;
        case 0x10:
            ret = podop_action(error);
            return ret;
        }
    }
    return ret;
}
u8 aborg_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(aborg_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = mapcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zmped_action(error);
            seq = 0x04;
            break;
        case 0x03:
            ret = mstof_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = dorbk_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = zdrup_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = dorfw_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = dorcl_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = vacof_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = ywait_action(error);
            seq = 0x10;
            break;
        case 0x10:
            ret = podop_action(error);
            return ret;
        }
    }
    return ret;
}

u8 sysin_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;

    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(sysin_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x00:
            ret = mapcl_action(error);
            printf("mapcl_action  \r\n");
            if(is_drondr()) {
                if(is_foup_presence())
                {
                    seq = 0x01;
                }
                else
                {
                    seq = 0x03;
                }
            }
            else
            {
                seq = 0x05;
            }

            break;
        case 0x01:
            ret = podcl_action(error);
            printf("podcl_action \r\n");
            seq = 0x02;
            break;
        case 0x02:
            ret = ydoor_action(error);
            printf("ydoor_action \r\n");
            seq = 0x03;
            break;
        case 0x03:
            ret = vacon_action(error);
            printf("vacon_action \r\n");
            seq = 0x04;
            break;
        case 0x04:
            ret = dorop_action(error);
            printf("dorop_action \r\n");
            seq = 0x05;
            break;
        case 0x05:
            ret = dorbk_action(error);
            printf("dorbk_action \r\n");
            seq = 0x06;
            break;
        case 0x06:
            ret = motin_action(error);
            printf("motin_action \r\n");
            seq = 0x07;
            break;
        case 0x07:
            ret = dorfw_action(error);
            printf("dorfw_action \r\n");
            seq = 0x08;
            break;
        case 0x08:
            ret = dorcl_action(error);
            printf("dorcl_action \r\n");
            seq = 0x09;
            break;
        case 0x09:
            ret = vacof_action(error);
            printf("vacof_action \r\n");
            seq = 0x10;
            break;
        case 0x10:
            ret = ywait_action(error);
            printf("ywait_action \r\n");
            podop_action(error);
            seq = 0x11;
            break;
        case 0x11:
            ret = podop_action(error);
            printf("podop_action \r\n");
            return ret;
        }
    }
    return ret;
}

u8 cload_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cload_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = podcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = ydoor_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = vacon_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = dorop_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = dorbk_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = zdrmp_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = zdrdw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cldmp_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cldmp_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = podcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = ydoor_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = vacon_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = dorop_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = dorbk_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = zmpst_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = mapop_action(error);
            seq = 0x09;
            break;
        case 0x08:
            ret = mston_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = zdrmp_action(error);
            seq = 0x10;
            break;
        case 0x10:
            ret = mapcl_action(error);
            seq = 0x12;
            break;
        case 0x11:
            ret = mstof_action(error);
            seq = 0x12;
            break;
        case 0x12:
            ret = zdrdw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 clddk_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(clddk_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = podcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = ydoor_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = vacon_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = dorop_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cldyd_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cldyd_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = podcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = ydoor_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cldop_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cldop_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = dorbk_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zdrdw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 clmpo_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(clmpo_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = dorbk_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zmpst_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = mapop_action(error);
            seq = 0x05;
            break;
        case 0x04:
            ret = mston_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = zdrmp_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = mapcl_action(error);
            seq = 0x08;
            break;
        case 0x07:
            ret = mstof_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = zdrdw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 mapdo_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(mapdo_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {

        case 0x01:
            ret = zdrup_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = dorbk_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = zmpst_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = mapop_action(error);
            seq = 0x06;
            break;
        case 0x05:
            ret = mston_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = zdrmp_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = mapcl_action(error);
            seq = 0x09;
            break;
        case 0x08:
            ret = mstof_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = zdrdw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 remap_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(remap_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {

        case 0x01:
            ret = zdrup_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = dorbk_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = zmpst_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = mapop_action(error);
            seq = 0x06;
            break;
        case 0x05:
            ret = mston_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = zdrmp_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = mapcl_action(error);
            seq = 0x09;
            break;
        case 0x08:
            ret = mstof_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = zdrdw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 culod_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(culod_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = mapcl_action(error);
            seq = 0x03;
            break;
        case 0x02:
            ret = mstof_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = zdrup_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = zmped_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = dorfw_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = dorcl_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = vacof_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = ywait_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = podop_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cudmp_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cudmp_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = zdrup_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zmpst_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = mapop_action(error);
            seq = 0x05;
            break;
        case 0x04:
            ret = mston_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = zdrmp_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = mapcl_action(error);
            seq = 0x08;
            break;
        case 0x07:
            ret = mstof_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = zdrup_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = zmped_action(error);
            seq = 0x10;
            break;
        case 0x10:
            ret = dorfw_action(error);
            seq = 0x11;
            break;
        case 0x11:
            ret = dorcl_action(error);
            seq = 0x12;
            break;
        case 0x12:
            ret = vacof_action(error);
            seq = 0x13;
            break;
        case 0x13:
            ret = ywait_action(error);
            seq = 0x14;
            break;
        case 0x14:
            ret = podop_action(error);
            return ret;
        }
    }
    return ret;
}
u8 culfc_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(culfc_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = mapcl_action(error);
            seq = 0x03;
            break;
        case 0x02:
            ret = mstof_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = zdrup_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = zmped_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = dorfw_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = dorcl_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = vacof_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = ywait_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cumfc_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cumfc_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = zdrup_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zmpst_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = mapop_action(error);
            seq = 0x05;
            break;
        case 0x04:
            ret = mston_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = zdrmp_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = mapcl_action(error);
            seq = 0x08;
            break;
        case 0x07:
            ret = mstof_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = zdrup_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = zmped_action(error);
            seq = 0x10;
            break;
        case 0x10:
            ret = dorfw_action(error);
            seq = 0x11;
            break;
        case 0x11:
            ret = dorcl_action(error);
            seq = 0x12;
            break;
        case 0x12:
            ret = vacof_action(error);
            seq = 0x13;
            break;
        case 0x13:
            ret = ywait_action(error);
            return ret;
        }
    }
    return ret;
}
u8 culyd_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(culyd_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = mapcl_action(error);
            seq = 0x03;
            break;
        case 0x02:
            ret = mstof_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = zdrup_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = zmped_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = dorfw_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = dorcl_action(error);
            seq = 0x07;
            break;
        case 0x07:
            ret = vacof_action(error);
            return ret;
        }
    }
    return ret;
}
u8 culdk_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(culdk_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = mapcl_action(error);
            seq = 0x03;
            break;
        case 0x02:
            ret = mstof_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = zdrup_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = zmped_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = dorfw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cumdk_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cumdk_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = zdrup_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = zmpst_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = mapop_action(error);
            seq = 0x05;
            break;
        case 0x04:
            ret = mston_action(error);
            seq = 0x05;
            break;
        case 0x05:
            ret = zdrmp_action(error);
            seq = 0x06;
            break;
        case 0x06:
            ret = mapcl_action(error);
            seq = 0x08;
            break;
        case 0x07:
            ret = mstof_action(error);
            seq = 0x08;
            break;
        case 0x08:
            ret = zdrup_action(error);
            seq = 0x09;
            break;
        case 0x09:
            ret = zmped_action(error);
            seq = 0x10;
            break;
        case 0x10:
            ret = dorfw_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cudnc_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cudnc_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = dorcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = vacof_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = ywait_action(error);
            seq = 0x04;
            break;
        case 0x04:
            ret = podop_action(error);
            return ret;
        }
    }
    return ret;
}
u8 cudcl_action(u8* error)
{
    u8 seq = 0;
    u8 ret = ACT_STA;
    seq = 0x01;
    while((ret == ACT_STA) || (ret == ACT_END))
    {
        if(cudcl_running(error) == true)
        {
            return ACT_ERR;
        }
        switch(seq)
        {
        case 0x01:
            ret = dorcl_action(error);
            seq = 0x02;
            break;
        case 0x02:
            ret = vacof_action(error);
            seq = 0x03;
            break;
        case 0x03:
            ret = ywait_action(error);
            return ret;
        }
    }
    return ret;
}

bool proc_result(u8 cmd, u8 rtype, u8 error)
{
    u8 chcmd[6];
    u8 param[7];
    switch (cmd)
    {
    case CMD_ACTION_NOACT:
        if(rtype == ACT_STP)
        {
            memcpy(chcmd, "STOP_", 5);
        }
        if(rtype == ACT_ABT)
        {
            memcpy(chcmd, "ABORT", 5);
        }
        break;
    case CMD_ACTION_PODOP:
        memcpy(chcmd, "PODOP", 5);
        break;
    case CMD_ACTION_PODCL:
        memcpy(chcmd, "PODCL", 5);
        break;
    case CMD_ACTION_VACON:
        memcpy(chcmd, "VACON", 5);
        break;
    case CMD_ACTION_VACOF:
        memcpy(chcmd, "VACOF", 5);
        break;
    case CMD_ACTION_DOROP:
        memcpy(chcmd, "DOROP", 5);
        break;
    case CMD_ACTION_DORCL:
        memcpy(chcmd, "DORCL", 5);
        break;
    case CMD_ACTION_MAPOP:
        memcpy(chcmd, "MAPOP", 5);
        break;
    case CMD_ACTION_MAPCL:
        memcpy(chcmd, "MAPCL", 5);
        break;
    case CMD_ACTION_DORBK:
        memcpy(chcmd, "DORBK", 5);
        break;
    case CMD_ACTION_DORFW:
        memcpy(chcmd, "DORFW", 5);
        break;
    case CMD_ACTION_YDOOR:
        memcpy(chcmd, "YDOOR", 5);
        break;
    case CMD_ACTION_YWAIT:
        memcpy(chcmd, "YWAIT", 5);
        break;
    case CMD_ACTION_MSTON:
        memcpy(chcmd, "MSTON", 5);
        break;
    case CMD_ACTION_MSTOF:
        memcpy(chcmd, "MSTOF", 5);
        break;
    case CMD_ACTION_ZMPST:
        memcpy(chcmd, "ZMPST", 5);
        break;
    case CMD_ACTION_ZMPED:
        memcpy(chcmd, "ZMPED", 5);
        break;
    case CMD_ACTION_ZDRMP:
        memcpy(chcmd, "ZDRMP", 5);
        break;
    case CMD_ACTION_ZDRDW:
        memcpy(chcmd, "ZDRDW", 5);
        break;
    case CMD_ACTION_ZDRUP:
        memcpy(chcmd, "ZDRUP", 5);
        break;
    case CMD_ACTION_SYSIN:
        memcpy(chcmd, "SYSIN", 5);
        break;
    case CMD_ACTION_ORGSH:
        memcpy(chcmd, "ORGSH", 5);
        break;
    case CMD_ACTION_ABORG:
        memcpy(chcmd, "ABORG", 5);
        break;
    case CMD_ACTION_CLOAD:
        memcpy(chcmd, "CLOAD", 5);
        break;
    case CMD_ACTION_CLDDK:
        memcpy(chcmd, "CLDDK", 5);
        break;
    case CMD_ACTION_CLDYD:
        memcpy(chcmd, "CLDYD", 5);
        break;
    case CMD_ACTION_CLDOP:
        memcpy(chcmd, "CLDOP", 5);
        break;
    case CMD_ACTION_CLDMP:
        memcpy(chcmd, "CLDMP", 5);
        break;
    case CMD_ACTION_CLMPO:
        memcpy(chcmd, "CLMPO", 5);
        break;
    case CMD_ACTION_CULOD:
        memcpy(chcmd, "CULOD", 5);
        break;
    case CMD_ACTION_CULDK:
        memcpy(chcmd, "CULDK", 5);
        break;
    case CMD_ACTION_CUDCL:
        memcpy(chcmd, "CUDCL", 5);
        break;
    case CMD_ACTION_CUDNC:
        memcpy(chcmd, "CUDNC", 5);
        break;
    case CMD_ACTION_CULYD:
        memcpy(chcmd, "CULYD", 5);
        break;
    case CMD_ACTION_CULFC:
        memcpy(chcmd, "CULFC", 5);
        break;
    case CMD_ACTION_CUDMP:
        memcpy(chcmd, "CUDMP", 5);
        break;
    case CMD_ACTION_CUMDK:
        memcpy(chcmd, "CUMDK", 5);
        break;
    case CMD_ACTION_CUMFC:
        memcpy(chcmd, "CUMFC", 5);
        break;
    case CMD_ACTION_MAPDO:
        memcpy(chcmd, "MAPDO", 5);
        break;
    case CMD_ACTION_REMAP:
        memcpy(chcmd, "REMAP", 5);
        break;
    }
    if(rtype == ACT_END)
    {
        send_msg(gCom_mod & BCAK_INF, (char*)chcmd, (u8*)NULL, 0);
        gCur_status = G_CUR_STA_END;
        gEnd_act = cmd;
        if(cmd == CMD_ACTION_ORGSH || cmd == CMD_ACTION_ABORG)
        {
            is_origin = true;
        }
        if(cmd == CMD_ACTION_SYSIN)
        {
            gissysinit = true;
            is_origin = true;
        }
    }
    else if(rtype == ACT_ERR)
    {
        switch(error)
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
        case 0x02:
            memcpy(param, (char*)"/ZLMIT", 6);
            break;
        case 0x42:
            memcpy(param, (char*)"/ZLMIT", 6);
            break;
        case 0x04:
            memcpy(param, (char*)"/YLMIT", 6);
            break;
        case 0x44:
            memcpy(param, (char*)"/YLMIT", 6);
            break;
        case 0x07:
            memcpy(param, (char*)"/PROTS", 6);
            break;
        case 0x08:
            memcpy(param, (char*)"/DLMIT", 6);
            break;
        case 0x48:
            memcpy(param, (char*)"/DLMIT", 6);
            break;
        case 0x09:
            memcpy(param, (char*)"/MPBAR", 6);
            break;
        case 0x49:
            memcpy(param, (char*)"/MPBAR", 6);
            break;
        case 0x10:
            memcpy(param, (char*)"/MPZLM", 6);
            break;
        case 0x50:
            memcpy(param, (char*)"/MPZLM", 6);
            break;
        case 0x11:
            memcpy(param, (char*)"/MPSTP", 6);
            break;
        case 0x51:
            memcpy(param, (char*)"/MPSTP", 6);
            break;
        case 0x12:
            memcpy(param, (char*)"/MPEDL", 6);
            break;
        case 0x21:
            memcpy(param, (char*)"/CLOPS", 6);
            break;
        case 0x22:
            memcpy(param, (char*)"/CLCLS", 6);
            break;
        case 0x61:
        case 0x62:
            if(pod_s == CMD_ACTION_PODOP)
            {
                memcpy(param, (char*)"/CLOPS", 6);
            }
            if(pod_s == CMD_ACTION_PODCL)
            {
                memcpy(param, (char*)"/CLCLS", 6);
            }
            pod_s = CMD_ACTION_NOACT;
            break;
        case 0x23:
            memcpy(param, (char*)"/DROPS", 6);
            break;
        case 0x24:
            memcpy(param, (char*)"/DRCLS", 6);
            break;
        case 0x25:
            memcpy(param, (char*)"/VACCS", 6);
            break;
        case 0x26:
            memcpy(param, (char*)"/VACOS", 6);
            break;
        case 0xA1:
            memcpy(param, (char*)"/INTOP", 6);
            break;
        case 0xA2:
            memcpy(param, (char*)"/INTCL", 6);
            break;
				case 0xB1:
            memcpy(param, (char*)"/SYSIN", 6);
            break;
        default:
            memcpy(param, (char*)"/UNDIF", 6);
            break;

        }
        set_errno(cmd, error);
        send_msg(gCom_mod & BCAK_ABS, (char*)chcmd, param, 6);
        gPre_status = gCur_status;
//       gCur_status = G_CUR_STA_ERR;
        gCur_status = G_CUR_STA_STP;
//				gIsError = true;
        gEnd_act = CMD_ACTION_NOACT;
    }
    else if(rtype == ACT_ABT)
    {
        if(is_aborg == false)
        {
            send_msg(gCom_mod & BCAK_INF, (char*)chcmd, param, 6);
            gCur_status = G_CUR_STA_ABO;
            gEnd_act = CMD_ACTION_NOACT;
        }
        else
        {
            is_aborg = false;
        }

    }
    else if(rtype == ACT_STP)
    {
        send_msg(gCom_mod & BCAK_INF, (char*)chcmd, param, 6);
        gCur_status = G_CUR_STA_STP;
        gEnd_act = CMD_ACTION_NOACT;
    }
    return true;
}

void tExe_Action(void *p_arg)
{
    OS_ERR err;
    u8 error;
    u8 ret = 0;
    while(1)
    {
        if(gCur_status == G_CUR_STA_ERR || \
                gCur_status == G_CUR_STA_INT || \
                gCur_status == G_CUR_STA_NAC || \
                gCur_status == G_CUR_STA_UNI || \
                gCur_status == G_CUR_STA_ABO || \
                gCur_status == G_CUR_STA_END || \
                gCur_status == G_CUR_STA_STP ||\
                gIsError)
        {
            if(gIsError)
            {
                gCur_status = G_CUR_STA_ERR;
            }
            OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
            continue;
        }
        OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
        gCur_action = gCmd_action;
        switch (gCur_action)
        {
        case CMD_ACTION_NOACT:
            ret = noact_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_PODOP:
            ret = podop_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_PODCL:
            ret = podcl_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_VACON:
            ret = vacon_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_VACOF:
            ret = vacof_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_DOROP:
            ret = dorop_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_DORCL:
            ret = dorcl_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_MAPOP:
            ret = mapop_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_MAPCL:
            ret = mapcl_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_DORBK:
            ret = dorbk_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_DORFW:
            ret = dorfw_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_YDOOR:
            ret = ydoor_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_YWAIT:
            ret = ywait_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_MSTON:
            ret = mston_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_MSTOF:
            ret = mstof_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ZMPST:
            ret = zmpst_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ZMPED:
            ret = zmped_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ZDRMP:
            ret = zdrmp_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ZDRDW:
            ret = zdrdw_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ZDRUP:
            ret = zdrup_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ORGSH:
            ret = orgsh_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_SYSIN:
            ret = sysin_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_ABORG:
            ret = aborg_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CLOAD:
            ret = cload_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CLDDK:
            ret = clddk_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CLDYD:
            ret = cldyd_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CLDOP:
            ret = cldop_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CLDMP:
            ret = cldmp_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CLMPO:
            ret = clmpo_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CULOD:
            ret = culod_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CULDK:
            ret = culdk_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CUDCL:
            ret = cudcl_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CUDNC:
            ret = cudnc_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CULYD:
            ret = culyd_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CULFC:
            ret = culfc_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CUDMP:
            ret = cudmp_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CUMDK:
            ret = cumdk_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_CUMFC:
            ret = cumfc_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_MAPDO:
            ret = mapdo_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        case CMD_ACTION_REMAP:
            ret = remap_action(&error);
            proc_result(gCur_action, ret, error);
            break;
        default:
            break;
        }
        gCur_pause = 0;
        gCur_stop = 0;
        gCur_abort = 0;
        gCur_retry = 0;
    }
}
