/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
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
 *	@(#)pcb.h	5.6 (Berkeley) 01/18/91
 */

/*
 * Intel 386 process control block
 */
#include "tss.h"
#include "npx.h"

struct pcb {
	struct	i386tss pcbtss;
#define	pcb_ksp	pcbtss.tss_esp0
#define	pcb_ptd	pcbtss.tss_cr3
#define	pcb_cr3	pcb_ptd
#define	pcb_pc	pcbtss.tss_eip
#define	pcb_psl	pcbtss.tss_eflags
#define	pcb_usp	pcbtss.tss_esp
#define	pcb_fp	pcbtss.tss_ebp
/*
 * Software pcb (extension)
 */
	int	pcb_flags;
#define	FP_WASUSED	0x1	/* floating point has been used in this proc */
#define	FP_NEEDSSAVE	0x2	/* needs save on next context switch */
#define	FP_NEEDSRESTORE	0x4	/* need restore on next DNA fault */
#define	FP_USESEMC	0x8	/* process uses EMC memory-mapped mode */
	struct	save87	pcb_savefpu;
	struct	emcsts	pcb_saveemc;
	struct	pte	*pcb_p0br;
	struct	pte	*pcb_p1br;
	int	pcb_p0lr;
	int	pcb_p1lr;
	int	pcb_szpt; 	/* number of pages of user page table */
	int	pcb_cmap2;
	int	*pcb_sswap;
	long	pcb_sigc[8];
	int	pcb_iml;	/* interrupt mask level */
};
