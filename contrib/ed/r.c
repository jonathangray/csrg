/*-
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rodney Ruddock of the University of Guelph.
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
static char sccsid[] = "@(#)r.c	5.5 (Berkeley) 03/01/93";
#endif /* not lint */

#include <sys/types.h>
#include <sys/stat.h>

#include <a.out.h>
#include <errno.h>
#include <regex.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DBI
#include <db.h>
#endif

#include "ed.h"
#include "extern.h"

/*
 * This sets up things for the central input routine to place the
 * incoming text at the proper place in the buffer.
 */
void
r(inputt, errnum)
	FILE *inputt;
	int *errnum;
{
	FILE *l_fp;
	long l_num;
	char *l_filename_read=NULL, *l_temp;

	if (filename_flag == 1) {
		sigspecial++;
		l_filename_read = filename_current;
		filename_flag = 0;
		sigspecial--;
		if (sigint_flag && (!sigspecial))
			SIGINT_ACTION;
	} else {
		l_temp = filename(inputt, errnum);
		if (*errnum == 1)
			l_filename_read = l_temp;
		else
			if (*errnum == -2) {
				while (((ss = getc(inputt)) != '\n') ||
				    (ss == EOF));
				l_filename_read = filename_current;
			} else
				if (*errnum < 0)
					return;
		*errnum = 0;
	}

	if (filename_current == NULL) {
		if (l_filename_read == NULL) {
			strcpy(help_msg, "no filename given");
			*errnum = -1;
			if (ss)
				ungetc('\n', inputt);
			return;
		} else
			filename_current = l_filename_read;
	}

	/*
	 * Determine if the file can be read.  If not set the help message to
	 * something descriptive that the user should understand.
	 * We're now allowing ed to read directory and executable files
	 * for as much as it can, if there are NULL's in the file it
	 * is guaranteed to be different since ed doesn't do NULL's.
	 */
	if ((l_fp = fopen(l_filename_read, "r")) == NULL) {
		strcpy(help_msg, "permission lacking to read file");
		printf("?%s\n", l_filename_read);
		*errnum = 0;
		return;
	}
	fseek(l_fp, (off_t)0, 0);
	if (g_flag == 0)
		u_clr_stk();
	l_num = input_lines(l_fp, errnum);
	if (*errnum < 0)
		return;
	*errnum = 0;

	if (explain_flag)	/* !=0 */
		printf("%ld\n", l_num);
	if (l_filename_read != filename_current)
		free(l_filename_read);

	fclose(l_fp);
	change_flag = 1;
	if (sigint_flag)
		SIGINT_ACTION;
	*errnum = 1;
}
