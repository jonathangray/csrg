/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)conf.c	7.6 (Berkeley) 06/28/90
 */

#include "param.h"
#include "systm.h"
#include "buf.h"
#include "ioctl.h"
#include "tty.h"
#include "conf.h"

int nulldev(), nodev(), rawread(), rawwrite(), swstrategy();

#include "dk.h"
#if NVD > 0
int	vdopen(),vdclose(),vdstrategy(),vdioctl();
int	vddump(),vdsize();
#else
#define	vdopen		nodev
#define	vdclose		nodev
#define	vdstrategy	nodev
#define	vdioctl		nodev
#define	vddump		nodev
#define	vdsize		0
#endif

#include "hd.h"
#if NHD > 0
int	hdopen(),hdclose(),hdstrategy(),hdioctl();
int	hddump(),hdsize();
#else
#define	hdopen		nodev
#define	hdclose		nodev
#define	hdstrategy	nodev
#define	hdioctl		nodev
#define	hddump		nodev
#define	hdsize		0
#endif

#include "yc.h"
#if NCY > 0
int	cyopen(),cyclose(),cystrategy(),cydump();
int	cyioctl(),cyreset();
#else
#define	cyopen		nodev
#define	cyclose		nodev
#define	cystrategy	nodev
#define	cydump		nodev
#define	cyioctl		nodev
#define	cyreset		nulldev
#endif

struct bdevsw	bdevsw[] =
{
	{ nodev,	nulldev,	nodev,		nodev,		/*0*/
	  nodev,	0,		0 },
	{ vdopen,	vdclose,	vdstrategy,	vdioctl,	/*1*/
	  vddump,	vdsize,		0 },
	{ hdopen,	hdclose,	hdstrategy,	hdioctl,	/*2*/
	  hddump,	hdsize,		0 },
	{ cyopen,	cyclose,	cystrategy,	cyioctl,	/*3*/
	  cydump,	0,		B_TAPE },
	{ nodev,	nodev,		swstrategy,	nodev,		/*4*/
	  nodev,	0,		0 },
};
int	nblkdev = sizeof (bdevsw) / sizeof (bdevsw[0]);

int	cnopen(),cnclose(),cnread(),cnwrite(),cnioctl();
extern	struct tty cons;

#include "vx.h"
#if NVX == 0
#define	vxopen	nodev
#define	vxclose	nodev
#define	vxread	nodev
#define	vxwrite	nodev
#define	vxioctl	nodev
#define	vxstop	nodev
#define	vxreset	nulldev
#define	vx_tty	0
#else
int	vxopen(),vxclose(),vxread(),vxwrite(),vxioctl(),vxstop(),vxreset();
struct	tty vx_tty[];
#endif

int	syopen(),syread(),sywrite(),syioctl(),syselect();

int 	mmrw();
#define	mmselect	seltrue

#include "pty.h"
#if NPTY > 0
int	ptsopen(),ptsclose(),ptsread(),ptswrite(),ptsstop();
int	ptcopen(),ptcclose(),ptcread(),ptcwrite(),ptcselect();
int	ptyioctl();
struct	tty pt_tty[];
#else
#define ptsopen		nodev
#define ptsclose	nodev
#define ptsread		nodev
#define ptswrite	nodev
#define ptcopen		nodev
#define ptcclose	nodev
#define ptcread		nodev
#define ptcwrite	nodev
#define ptyioctl	nodev
#define	pt_tty		0
#define	ptcselect	nodev
#define	ptsstop		nulldev
#endif

#include "mp.h"
#if NMP > 0
int	mpopen(), mpclose(), mpread(), mpwrite(), mpioctl(), mpstop();
int	mpdlopen(), mpdlclose(), mpdlwrite(), mpdlioctl();
extern	struct tty mp_tty[];
#else
#define	mpopen		nodev
#define	mpclose		nodev
#define	mpread		nodev
#define	mpwrite		nodev
#define	mpioctl		nodev
#define	mpstop		nodev
#define	mpdlopen	nodev
#define	mpdlclose	nodev
#define	mpdlwrite	nodev
#define	mpdlioctl	nodev
#define	mp_tty		0
#endif

#if NII > 0
int	iiioctl(), iiclose(), iiopen();
#else
#define	iiopen	nodev
#define	iiclose	nodev
#define	iiioctl	nodev
#endif

#include "enp.h"
#if NENP > 0
int	enpr_open(), enpr_close(), enpr_read(), enpr_write(), enpr_ioctl();
#else
#define enpr_open	nodev
#define enpr_close	nodev
#define enpr_read	nodev
#define enpr_write	nodev
#define enpr_ioctl	nodev
#endif

#include "dr.h"
#if NDR > 0
int     dropen(),drclose(),drread(),drwrite(),drioctl(),drreset();
#else
#define dropen nodev
#define drclose nodev
#define drread nodev
#define drwrite nodev
#define drioctl nodev
#define drreset nodev
#endif

#include "ik.h"
#if NIK > 0
int     ikopen(),ikclose(),ikread(),ikwrite(),ikioctl();
#else
#define ikopen nodev
#define ikclose nodev
#define ikread nodev
#define ikwrite nodev
#define ikioctl nodev
#endif

int	logopen(),logclose(),logread(),logioctl(),logselect();

int	fdopen();

int	ttselect(), seltrue();

struct cdevsw	cdevsw[] =
{
	cnopen,		cnclose,	cnread,		cnwrite,	/*0*/
	cnioctl,	nulldev,	nulldev,	&cons,
	ttselect,	nodev,		NULL,
	vxopen,		vxclose,	vxread,		vxwrite,	/*1*/
	vxioctl,	vxstop,		vxreset,	vx_tty,
	ttselect,	nodev,		NULL,
	syopen,		nulldev,	syread,		sywrite,	/*2*/
	syioctl,	nulldev,	nulldev,	NULL,
	syselect,	nodev,		NULL,
	nulldev,	nulldev,	mmrw,		mmrw,		/*3*/
	nodev,		nulldev,	nulldev,	NULL,
	mmselect,	nodev,		NULL,
	nodev,		nulldev,	nodev,		nodev,		/*4*/
	nodev,		nodev,		nulldev,	NULL,
	seltrue,	nodev,		NULL,
	vdopen,		vdclose,	rawread,	rawwrite,	/*5*/
	vdioctl,	nodev,		nulldev,	NULL,
	seltrue,	nodev,		vdstrategy,
	hdopen,		hdclose,	rawread,	rawwrite,	/*6*/
	hdioctl,	nodev,		nulldev,	NULL,
	seltrue,	nodev,		hdstrategy,
	cyopen,		cyclose,	rawread,	rawwrite,	/*7*/
	cyioctl,	nodev,		cyreset,	NULL,
	seltrue,	nodev,		cystrategy,
	nulldev,	nulldev,	rawread,	rawwrite,	/*8*/
	nodev,		nodev,		nulldev,	NULL,
	nodev,		nodev,		swstrategy,
	ptsopen,	ptsclose,	ptsread,	ptswrite,	/*9*/
	ptyioctl,	ptsstop,	nodev,		pt_tty,
	ttselect,	nodev,		NULL,
	ptcopen,	ptcclose,	ptcread,	ptcwrite,	/*10*/
	ptyioctl,	nulldev,	nodev,		pt_tty,
	ptcselect,	nodev,		NULL,
	mpdlopen,	mpdlclose,	nodev,		mpdlwrite,	/*11*/
	mpdlioctl,	nodev,		nulldev,	NULL,
	seltrue,	nodev,		NULL,
	mpopen,		mpclose,	mpread,		mpwrite,	/*12*/
	mpioctl,	mpstop,		nulldev,	mp_tty,
	ttselect,	nodev,		NULL,
	nodev,		nodev,		nodev,		nodev,		/*13*/
	nodev,		nodev,		nulldev,	NULL,
	nodev,		nodev,		NULL,
	iiopen,		iiclose,	nulldev,	nulldev,	/*14*/
	iiioctl,	nulldev,	nulldev,	NULL,
	seltrue,	nodev,		NULL,
	logopen,	logclose,	logread,	nodev,		/*15*/
	logioctl,	nodev,		nulldev,	NULL,
	logselect,	nodev,		NULL,
	enpr_open,	enpr_close,	enpr_read,	enpr_write,	/*16*/
	enpr_ioctl,	nodev,		nulldev,	NULL,
	nodev,		nodev,		NULL,
	nodev,		nodev,		nodev,		nodev,		/*17*/
	nodev,		nodev,		nulldev,	NULL,
	nodev,		nodev,		NULL,
	dropen,		drclose,	drread,		drwrite,	/*18*/
	drioctl,	nodev,		drreset,	NULL,
	nodev,		nodev,		NULL,
	nodev,		nodev,		nodev,		nodev,		/*19*/
	nodev,		nodev,		nulldev,	NULL,
	nodev,		nodev,		NULL,
/* 20-30 are reserved for local use */
	ikopen,		ikclose,	ikread,		ikwrite,	/*20*/
	ikioctl,	nodev,		nulldev,	NULL,
	nodev,		nodev,		NULL,
	fdopen,		nodev,		nodev,		nodev,		/*21*/
	nodev,		nodev,		nodev,		NULL,
	nodev,		nodev,		NULL,
};
int	nchrdev = sizeof (cdevsw) / sizeof (cdevsw[0]);

int	mem_no = 3; 	/* major device number of memory special file */

/*
 * Swapdev is a fake device implemented
 * in sw.c used only internally to get to swstrategy.
 * It cannot be provided to the users, because the
 * swstrategy routine munches the b_dev and b_blkno entries
 * before calling the appropriate driver.  This would horribly
 * confuse, e.g. the hashing routines. Instead, /dev/drum is
 * provided as a character (raw) device.
 */
dev_t	swapdev = makedev(4, 0);
