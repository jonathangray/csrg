/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
 */

#ifndef lint
static char sccsid[] = "@(#)odsyntax.c	5.2 (Berkeley) 05/25/90";
#endif /* not lint */

#include <sys/types.h>
#include <stdio.h>
#include "hexdump.h"

int deprecated;

oldsyntax(argc, argvp)
	int argc;
	char ***argvp;
{
	extern enum _vflag vflag;
	extern FS *fshead;
	extern char *optarg;
	extern int length, optind;
	int ch, first;
	char **argv;

	deprecated = 1;
	first = 0;
	argv = *argvp;
	while ((ch = getopt(argc, argv, "aBbcDdeFfHhIiLlOoPpswvXx")) != EOF)
		switch (ch) {
		case 'a':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("16/1 \"%3_u \" \"\\n\"");
			break;
		case 'B':
		case 'o':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("8/2 \" %06o \" \"\\n\"");
			break;
		case 'b':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("16/1 \"%03o \" \"\\n\"");
			break;
		case 'c':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("16/1 \"%3_c \" \"\\n\"");
			break;
		case 'd':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("8/2 \"  %05u \" \"\\n\"");
			break;
		case 'D':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("4/4 \"     %010u \" \"\\n\"");
			break;
		case 'e':		/* undocumented in od */
		case 'F':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("2/8 \"          %21.14e \" \"\\n\"");
			break;
			
		case 'f':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("4/4 \" %14.7e \" \"\\n\"");
			break;
		case 'H':
		case 'X':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("4/4 \"       %08x \" \"\\n\"");
			break;
		case 'h':
		case 'x':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("8/2 \"   %04x \" \"\\n\"");
			break;
		case 'I':
		case 'L':
		case 'l':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("4/4 \"    %11d \" \"\\n\"");
			break;
		case 'i':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("8/2 \" %6d \" \"\\n\"");
			break;
		case 'O':
			if (!first++) {
				add("\"%07.7_Ao\n\"");
				add("\"%07.7_ao  \"");
			} else
				add("\"         \"");
			add("4/4 \"    %011o \" \"\\n\"");
			break;
		case 'v':
			vflag = ALL;
			break;
		case 'P':
		case 'p':
		case 's':
		case 'w':
		case '?':
		default:
			(void)fprintf(stderr,
			    "od: od(1) has been deprecated for hexdump(1).\n");
			if (ch != '?')
				(void)fprintf(stderr,
"od: hexdump(1) compatibility doesn't support the -%c option%s\n",
				    ch, ch == 's' ? "; see strings(1)." : ".");
			usage();
		}

	if (!fshead) {
		add("\"%07.7_Ao\n\"");
		add("\"%07.7_ao  \" 8/2 \"%06o \" \"\\n\"");
	}

	*argvp += optind;
}
