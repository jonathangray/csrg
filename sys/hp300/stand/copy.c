/*
 * Copyright (c) 1982, 1986, 1990 The Regents of the University of California.
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
 *	@(#)copy.c	7.1 (Berkeley) 05/08/90
 */

/*
 * Copy from to in 10K units.
 * Intended for use in system
 * installation.
 */
main()
{
	int from, to;
	char fbuf[50], tbuf[50];
	char buffer[10240];
	register int record;
	extern int errno;

	from = getdev("From", fbuf, 0);
	to = getdev("To", tbuf, 1);
	for (record = 0; ; record++) {
		int rcc, wcc;

		rcc = read(from, buffer, sizeof (buffer));
		if (rcc == 0)
			break;
		if (rcc < 0) {
			printf("Record %d: read error, errno=%d\n",
				record, errno);
			break;
		}
		if (rcc < sizeof (buffer))
			printf("Record %d: read short; expected %d, got %d\n",
				record, sizeof (buffer), rcc);
		/*
		 * For bug in ht driver.
		 */
		if (rcc > sizeof (buffer))
			rcc = sizeof (buffer);
		wcc = write(to, buffer, rcc);
		if (wcc < 0) {
			printf("Record %d: write error: errno=%d\n",
				record, errno);
			break;
		}
		if (wcc < rcc) {
			printf("Record %d: write short; expected %d, got %d\n",
				record, rcc, wcc);
			break;
		}
	}
	printf("Copy completed: %d records copied\n", record);
	/* can't call exit here */
}

getdev(prompt, buf, mode)
	char *prompt, *buf;
	int mode;
{
	register int i;

	do {
		printf("%s: ", prompt);
		gets(buf);
		i = open(buf, mode);
	} while (i <= 0);
	return (i);
}
