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
static char sccsid[] = "@(#)input_lines.c	5.2 (Berkeley) 01/23/93";
#endif /* not lint */

#include <sys/types.h>

#include <db.h>
#include <regex.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ed.h"
#include "extern.h"

/*
 * This central function gets text from some file, which can (and most
 * oft is) stdin. This flexability allows any text inputing function
 * to use it.
 */
long
input_lines(fp, errnum)
	FILE *fp;
	int *errnum;
{
	register long l_nn = 0;
	register int l_ss = ss;
	register char *l_text = text;
	LINE *l_temp_line, *l_temp1;
	long l_ttl = 0;
	int l_nn_max = nn_max;
	char *l_text2;

	if (End_default)
		start = current;
	else
		start = End;
	start_default = End_default = 0;

	/* start == NULL means line 0 which is legal for this function only. */
	nn_max_end = l_temp_line = start;
	if (start == NULL) {
		l_temp1 = top;
		u_add_stk(&(top->above));
	} else {
		u_add_stk(&(start->below));
		l_temp1 = start->below;
	}

	for (;;) {
		/* If NULL or bit-8 high not text; chuck. */
		if ((l_ss = getc(fp)) == EOF)
			break;
		if ((!l_ss) || (l_ss > 127))
			continue;
		if (sigint_flag)
			goto point;
		l_text[l_nn++] = (char) l_ss;
		if (l_ss == '\n') {
			l_text[l_nn - 1] = '\0';
			if ((l_nn == 2) && (l_text[0] == '.') && add_flag)
				break;
			nn_max_end = malloc(sizeof(LINE));
			if (nn_max_end == NULL) {
				*errnum = -1;
				strcpy(help_msg, "out of memory error");
				return (0L);
			}
			(nn_max_end->len) = l_nn - 1;
			(nn_max_end->handle) = add_line(l_text, l_nn - 1);
			(nn_max_end->above) = l_temp_line;
			(nn_max_end->below) = NULL;
			if (l_temp_line)
				(l_temp_line->below) = nn_max_end;
			else
				top = nn_max_end;
			l_temp_line = nn_max_end;

			l_ttl += l_nn;
			l_nn = 0;
		} else
			if (l_nn > l_nn_max) {
				l_nn_max += 512;
				nn_max = l_nn_max;
				l_text2 = l_text;
				l_text = text =
				    calloc(l_nn_max + 2, sizeof(char));
				if (text == NULL) {
					*errnum = -1;
					strcpy(help_msg, "out of memory error");
					return (0L);
				}
				bcopy(l_text2, l_text, l_nn);
				free(l_text2);
			}
	}

point:	current = nn_max_end;
	if (current == NULL)
		current = top;
	if (l_temp1 == NULL)
		bottom = nn_max_end;
	else
		if (nn_max_end != NULL) {
			(nn_max_end->below) = l_temp1;
			u_add_stk(&(l_temp1->above));
			(l_temp1->above) = nn_max_end;
		}
	change_flag = 1;
	*errnum = 1;
	ss = l_ss;
	return (l_ttl);
}
