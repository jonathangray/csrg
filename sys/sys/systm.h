/*-
 * Copyright (c) 1982, 1988, 1991 The Regents of the University of California.
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
 *	@(#)systm.h	7.14 (Berkeley) 05/05/91
 */

extern char *panicstr;		/* panic message */
extern char *version;		/* system version */

extern int nblkdev;		/* number of entries in bdevsw */
extern int nchrdev;		/* number of entries in cdevsw */
extern int nswdev;		/* number of swap devices */
extern int nswap;		/* size of swap space */

extern int selwait;		/* select timeout address */

extern char kmapwnt;		/* kernel map want flag */

extern u_char curpri;		/* priority of current process */

extern int maxmem;		/* max memory per process */
extern int physmem;		/* physical memory */

extern int intstack[];		/* interrupt stack */

extern int icode[];		/* user init code */
extern int szicode;		/* size of icode */

extern dev_t dumpdev;		/* dump device */
extern long dumplo;		/* offset into dumpdev */

extern dev_t rootdev;		/* root device */
extern struct vnode *rootvp;	/* vnode equivalent to above */

extern dev_t swapdev;		/* swapping device */
extern struct vnode *swapdev_vp;/* vnode equivalent to above */

extern dev_t argdev;		/* argument lists device */
extern struct vnode *argdev_vp;	/* vnode equivalent to above */

extern struct sysent {		/* system call table */
	int sy_narg;		/* number of arguments */
	int (*sy_call)();	/* implementing function */
} sysent[];

#ifdef	KADB
extern char *bootesym;		/* end of symbol info from boot */
#endif
extern int boothowto;		/* reboot flags, from console subsystem */

/* casts to keep lint happy */
#define	insque(q,p)	_insque((caddr_t)q,(caddr_t)p)
#define	remque(q)	_remque((caddr_t)q)

/*
 * General function prototypes.
 */
int	enodev __P((void));
int	enoioctl __P((void));
int	enxio __P((void));
int	eopnotsupp __P((void));
int	nullop __P((void));
int	seltrue __P((dev_t dev, int which, struct proc *p));

void	addlog __P((const char *, ...));
void	log __P((int, const char *, ...));
void	panic __P((char *));
void	printf __P((const char *, ...));
void	tablefull __P((char *));
void	ttyprintf __P((struct tty *, const char *, ...));

int	bcmp __P((void *str1, void *str2, u_int len));
void	bcopy __P((void *from, void *to, u_int len));
void	bzero __P((void *buf, u_int len));
void	ovbcopy __P((void *from, void *to, u_int len));
int	strlen __P((char *string));

int	copyin __P((void *udaddr, void *kaddr, u_int len));
int	copyinstr __P((void *udaddr, void *kaddr, u_int len, u_int *done));
int	copyout __P((void *kaddr, void *udaddr, u_int len));
int	copyoutstr __P((void *kaddr, void *udaddr, u_int len, u_int *done));
int	copystr __P((void *kfaddr, void *kdaddr, u_int len, u_int *done));

int	fubyte __P((void *base));
#ifdef notdef
int	fuibyte __P((void *base));
#endif
int	fuiword __P((void *base));
int	fuword __P((void *base));
int	subyte __P((void *base, int byte));
int	suibyte __P((void *base, int byte));
int	suiword __P((void *base, int word));
int	suword __P((void *base, int word));

int	ffs __P((long value));
int	locc __P((int mask, char *cp, unsigned size));
int	scanc __P((unsigned size, u_char *cp, u_char *table, int mask));
int	skpc __P((int mask, int size, char *cp));
