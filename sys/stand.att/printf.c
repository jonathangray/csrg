/*-
 * Copyright (c) 1991 The Regents of the University of California.
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
 *	@(#)printf.c	5.1 (Berkeley) 05/03/91
 */

#include <sys/param.h>

/*
 * Scaled down version of C Library printf.
 *
 * Used to print diagnostic information directly on the console tty.  Since
 * it is not interrupt driven, all system activities are suspended.  Printf
 * should not be used for chit-chat.
 *
 * One additional format: %b is supported to decode error registers.
 * Its usage is:
 *
 *	printf("reg=%b\n", regval, "<base><arg>*");
 *
 * where <base> is the output base expressed as a control character, e.g.
 * \10 gives octal; \20 gives hex.  Each arg is a sequence of characters,
 * the first of which gives the bit number to be inspected (origin 1), and
 * the next characters (up to a control character, i.e. a character <= 32),
 * give the name of the register.  Thus:
 *
 *	printf("reg=%b\n", 3, "\10\2BITTWO\1BITONE\n");
 *
 * would produce output:
 *
 *	reg=3<BITTWO,BITONE>
 */

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

static void abort(){}				/* Needed by stdarg macros. */
static void number();

void
#if __STDC__
printf(const char *fmt, ...)
#else
printf(fmt, va_alist)
	char *fmt;
        va_dcl
#endif
{
	register char *p;
	register int ch, n;
	unsigned long _ulong;
	int lflag, set;

	va_list ap;
#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	for (;;) {
		while ((ch = *fmt++) != '%') {
			if (ch == '\0')
				return;
			putchar(ch);
		}
		lflag = 0;
reswitch:	switch (ch = *fmt++) {
		case 'l':
			lflag = 1;
			goto reswitch;
		case 'b':
			_ulong = va_arg(ap, int);
			p = va_arg(ap, char *);
			number(_ulong, *p++);

			if (!_ulong)
				break;

			for (set = 0; n = *p++;) {
				if (_ulong & (1 << (n - 1))) {
					putchar(set ? ',' : '<');
					for (; (n = *p) > ' '; ++p)
						putchar(n);
					set = 1;
				} else
					for (; *p > ' '; ++p);
			}
			if (set)
				putchar('>');
			break;
		case 'c':
			ch = va_arg(ap, int);
				putchar(ch & 0x7f);
			break;
		case 's':
			p = va_arg(ap, char *);
			while (ch = *p++)
				putchar(ch);
			break;
		case 'd':
			_ulong = lflag ?
			    va_arg(ap, long) : va_arg(ap, int);
			if ((long)_ulong < 0) {
				putchar('-');
				_ulong = -_ulong;
			}
			number(_ulong, 10);
			break;
		case 'o':
			_ulong = lflag ?
			    va_arg(ap, long) : va_arg(ap, unsigned int);
			number(_ulong, 8);
			break;
		case 'u':
			_ulong = lflag ?
			    va_arg(ap, long) : va_arg(ap, unsigned int);
			number(_ulong, 10);
			break;
		case 'x':
			_ulong = lflag ?
			    va_arg(ap, long) : va_arg(ap, unsigned int);
			number(_ulong, 16);
			break;
		default:
			putchar('%');
			if (lflag)
				putchar('l');
			putchar(ch);
		}
	}
	va_end(ap);
}

static void
number(_ulong, base)
	unsigned long _ulong;
	int base;
{
	char *p, buf[11];			/* hold 2^32 in base 8 */

	p = buf;
	do {
		*p++ = "0123456789abcdef"[_ulong % base];
	} while (_ulong /= base);
	do {
		putchar(*--p);
	} while (p > buf);
}
