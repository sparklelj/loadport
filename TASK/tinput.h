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
bool is_sf_dradsrd(void);
bool is_sf_fpundki(void);
bool is_sf_fpundkd(void);
bool is_sf_clamulki(void);
bool is_sf_clamulkd(void);
bool is_sf_clambwdi(void);
bool is_sf_clambwdd(void);
bool is_sf_clamdwni(void);
bool is_sf_clamdwnd(void);

bool is_obstacle(void);

bool is_low(void);

#endif
