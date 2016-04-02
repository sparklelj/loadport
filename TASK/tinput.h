#ifndef __TINPUT_H
#define __TINPUT_H

#include "sys.h"

#define SI_READY 1

#define STATUS_NUM 200
#define INPUT_NUM 8
extern u8 gStatus[INPUT_NUM];

#define SF_UNREADY  0
#define SF_READY    1
#define SF_SETFOUPI 2
#define SF_SETFOUPD 3
#define SF_CLAMUPI  4
#define SF_CLAMUPD  5
#define SF_CLANFWDI 6
#define SF_CLANFWDD 7
#define SF_CLAMLCKI 8
#define SF_CLAMLCKD 9
#define SF_FPDCKI   19
#define SF_FPDCKD   20
#define SF_DRADSPI  21
#define SF_DRADSPD  22
#define SF_DRUNLTI  23
#define SF_DRUNLTD  24
#define SF_DROPI    25
#define SF_DROPD    26
#define SF_DRDWNSI  27
#define SF_DRDWNSD  28
#define SF_MPAOPI   29
#define SF_MPAOPD   30
#define SF_STPONI   31
#define SF_STPOND   32
#define SF_DRDWNEI  33
#define SF_DRDWNED  34
#define SF_MPACI    35
#define SF_MPACD    36
#define SF_STPOFFI  37
#define SF_STPOFFD  38
#define SF_DRDWNLI  39
#define SF_DRDWNLD  40
#define SF_DRUPLI   41
#define SF_DRUPLD   42
#define SF_DRCLSI   43
#define SF_DRCLSD   44
#define SF_DRLTI    45
#define SF_DRLTD    46
#define SF_DRADSRI  47
#define SF_DRADSRD  48
#define SF_FPUNDKI  49
#define SF_FPUNDKD  50
#define SF_CLAMULKI 51
#define SF_CLAMULKD 52
#define SF_CLAMBWDI 53
#define SF_CLAMBWDD 54
#define SF_CLAMDWNI 55
#define SF_CLAMDWND 56

bool is_sf_ready(void);
bool is_sf_setfoupi(void);
bool is_sf_setfoupd(void);
bool is_sf_clamupi(void);
bool is_sf_clamupd(void);
bool is_sf_clanfwdi(void);
bool is_sf_clanfwdd(void);
bool is_sf_clamlcki(void);
bool is_sf_clamlckd(void);
bool is_sf_fpdcki(void);
bool is_sf_fpdckd(void);
bool is_sf_dradspi(void);
bool is_sf_dradspd(void);
bool is_sf_drunlti(void);
bool is_sf_drunltd(void);
bool is_sf_dropi(void);
bool is_sf_dropd(void);
bool is_sf_drdwnsi(void);
bool is_sf_drdwnsd(void);
bool is_sf_mpaopi(void);
bool is_sf_mpaopd(void);
bool is_sf_stponi(void);
bool is_sf_stpond(void);
bool is_sf_drdwnei(void);
bool is_sf_drdwned(void);
bool is_sf_mpaci(void);
bool is_sf_mpacd(void);
bool is_sf_stpoffi(void);
bool is_sf_stpoffd(void);
bool is_sf_drdwnli(void);
bool is_sf_drdwnld(void);
bool is_sf_drupli(void);
bool is_sf_drupld(void);
bool is_sf_drclsi(void);
bool is_sf_drclsd(void);
bool is_sf_drlti(void);
bool is_sf_drltd(void);
bool is_sf_dradsri(void);
bool is_sf_dradsrd(void);
bool is_sf_fpundki(void);
bool is_sf_fpundkd(void);
bool is_sf_clamulki(void);
bool is_sf_clamulkd(void);
bool is_sf_clambwdi(void);
bool is_sf_clambwdd(void);
bool is_sf_clamdwni(void);
bool is_sf_clamdwnd(void);

bool is_no_foup(void);
bool is_foup_place(void);
bool is_obstacle(void);
bool is_protrusion(void); 
bool is_clampup(void);
bool is_clampdown(void);
bool is_clamplock(void);
bool is_clampfwd(void);
bool is_clampbwd(void);
bool is_dock(void);
bool is_undock(void);
bool is_vacuumon(void);
bool is_vacuumoff(void);
bool is_latch(void);
bool is_unlatch(void);
bool is_dropen(void);
bool is_drclose(void);
bool is_druplmt(void);
bool is_mapstart(void);
bool is_mapend(void);
bool is_drdwlmt(void);
bool is_mapopen(void);
bool is_mapclose(void);
bool is_stopperon(void);
bool is_stopperoff(void);
bool is_air(void);
bool is_error(void);
bool is_busy(void);

u8 podop_before(void);
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
u8 orgsh_before(void);
u8 orgsh_running(void);
u8 aborg_before(void);
u8 aborg_running(void);
u8 cload_before(void);
u8 cload_running(void);
u8 clddk_before(void);
u8 clddk_running(void);
u8 cldop_before(void);
u8 cldop_running(void);
u8 cldmp_before(void);
u8 cldmp_running(void);
u8 clmpo_before(void);
u8 clmpo_running(void);
u8 culod_before(void);
u8 culod_running(void);
u8 culdk_before(void);
u8 culdk_running(void);
u8 culyd_before(void);
u8 culyd_running(void);
u8 cudcl_before(void);
u8 cudcl_running(void);
u8 cudnc_before(void);
u8 cudnc_running(void);
u8 culfc_before(void);
u8 cuflc_running(void);
u8 mapod_before(void);
u8 mapod_running(void);
u8 remap_before(void);
u8 remap_running(void);
u8 cudmp_before(void);
u8 cudmp_running(void);
u8 cumdk_before(void);
u8 cumdk_running(void);
u8 cumfc_before(void);
u8 cumfc_running(void);


u8 clam_sta(void);
u8 latch_sta(void);
u8 vac_sta(void);
u8 dr_pos(void);
u8 is_block(void);
u8 Z_pos(void);
u8 Y_pos(void);
u8 map_apos(void);
u8 map_stp(void);
bool is_low(void);

#endif
