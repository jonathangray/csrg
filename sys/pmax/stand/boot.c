/*
 * Copyright (c) 1992 Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
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
 *	@(#)boot.c	7.4 (Berkeley) 10/11/92
 */

#include <sys/param.h>
#include <sys/reboot.h>
#include <sys/exec.h>

#ifndef TEST
#define DEF_MONFUNCS
#include <machine/machMon.h>
#endif

char	line[1024];

/*
 * This gets arguments from the PROM, calls other routines to open
 * and load the program to boot, and then transfers execution to that
 * new program.
 * Argv[0] should be something like "rz(0,0,0)vmunix" on a DECstation 3100.
 * Argv[0,1] should be something like "boot 5/rz0/vmunix" on a DECstation 5000.
 * The argument "-a" means vmunix should do an automatic reboot.
 */
void
main(argc, argv, argenv)
	int argc;
	char **argv;
	char **argenv;
{
	register char *cp;
	int howto, entry;
	char *boot = "boot";

#ifdef JUSTASK
	howto = RB_ASKNAME;
#else
	if (argc > 0 && strcmp(argv[0], boot) == 0) {
		argc--;
		argv++;
		argv[0] = getenv(boot);
	}
	howto = 0;
	for (cp = argv[0]; *cp; cp++) {
		if (*cp == ')' && cp[1]) {
			cp = argv[0];
			goto fnd;
		}
	}
	howto |= RB_ASKNAME;
fnd:
	;
#endif
	for (;;) {
		if (howto & RB_ASKNAME) {
			printf("Boot: ");
			gets(line);
			if (line[0] == '\0')
				continue;
			cp = line;
			argv[0] = cp;
			argc = 1;
		} else
			printf("Boot: %s\n", cp);
		entry = loadfile(cp);
		if (entry != -1)
			break;
		howto = RB_ASKNAME;
	}
#ifndef TEST
	Boot_Transfer(argc, argv, argenv, entry);
#endif
}

/*
 * Open 'filename', read in program and return the entry point or -1 if error.
 */
loadfile(fname)
	register char *fname;
{
	register struct devices *dp;
	register int fd, i, n;
	struct exec aout;

	if ((fd = Open(fname, 0)) < 0)
		goto err;

	/* read the COFF header */
	i = Read(fd, (char *)&aout, sizeof(aout));
	if (i != sizeof(aout)) {
		printf("No a.out header\n");
		goto cerr;
	} else if (aout.a_magic != OMAGIC) {
		printf("A.out? magic 0%o size %d+%d+%d\n", aout.a_magic,
			aout.a_text, aout.a_data, aout.a_bss);
		goto cerr;
	}

	/* read the code and initialized data */
	printf("Size: %d+%d", aout.a_text, aout.a_data);
	if (Lseek(fd, N_TXTOFF(aout), 0) < 0) {
		printf("\nSeek error\n");
		goto cerr;
	}
	i = aout.a_text + aout.a_data;
#ifndef TEST
	n = Read(fd, (char *)aout.ex_aout.codeStart, i);
#else
	n = i;
#endif
	(void) Close(fd);
	if (n < 0) {
		printf("\nRead error\n");
		goto err;
	} else if (n != i) {
		printf("\nShort read (%d)\n", n);
		goto err;
	}

	/* kernel will zero out its own bss */
	n = aout.a_bss;
	printf("+%d\n", n);

	return ((int)aout.a_entry);

cerr:
	(void) Close(fd);
err:
	return (-1);
}
