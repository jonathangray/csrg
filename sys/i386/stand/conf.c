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
 *	@(#)conf.c	7.2 (Berkeley) 04/28/91
 */

#include "param.h"

#include "saio.h"
#ifdef BOOT
extern exception;
extern int debugflag;
#endif

devread(io)
	register struct iob *io;
{
	int cc;
	char c;

	io->i_flgs |= F_RDDATA;
	io->i_error = 0;
	cc = (*devsw[io->i_dev].dv_strategy)(io, READ);
	io->i_flgs &= ~F_TYPEMASK;

#ifdef BOOT
if(/*io->i_error || */(c=scankbd()))
	_longjmp(&exception,1);
#endif
	return (cc);
}

devwrite(io)
	register struct iob *io;
{
	int cc;
	char c;

	io->i_flgs |= F_WRDATA;
	io->i_error = 0;
	cc = (*devsw[io->i_dev].dv_strategy)(io, WRITE);
	io->i_flgs &= ~F_TYPEMASK;
#ifdef BOOT
if(/* io->i_error || */ (c=scankbd()))
	_longjmp(&exception,1);
#endif
	return (cc);
}

devopen(io)
	register struct iob *io;
{

	(*devsw[io->i_dev].dv_open)(io);
}

devclose(io)
	register struct iob *io;
{

	(*devsw[io->i_dev].dv_close)(io);
}

devioctl(io, cmd, arg)
	register struct iob *io;
	int cmd;
	caddr_t arg;
{

	return ((*devsw[io->i_dev].dv_ioctl)(io, cmd, arg));
}

/*ARGSUSED*/
nullsys(io)
	struct iob *io;
{

	;
}

/*ARGSUSED*/
nullioctl(io, cmd, arg)
	struct iob *io;
	int cmd;
	caddr_t arg;
{

	return (ECMD);
}

int	nullsys(), nullioctl();
int	wdstrategy(), wdopen()/*, wdioctl()*/;
int	fdstrategy(), fdopen()/*, fdioctl()*/;

struct devsw devsw[] = {
	/*{ "xx",	xxstrategy,	xxopen,		nullsys, nullioctl },*/
	{ "wd",	wdstrategy,	wdopen,		nullsys,/*wdioctl*/ nullioctl },
	{ "", 0, 0, 0, 0 }, /* swapdev place holder */
	{ "fd",	fdstrategy,	fdopen,		nullsys,/*fdioctl*/ nullioctl },
	{ 0, 0, 0, 0, 0 },
};
int ndevs = 3 ;
