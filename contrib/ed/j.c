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
static char sccsid[] = "@(#)j.c	5.2 (Berkeley) 01/23/93";
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
 * Join the spec'd lines onto one line. Make the new line from the
 * old lines and then delete the old ones (undo-able).
 */

void
j(inputt, errnum)
	FILE *inputt;
	int *errnum;
{
	LINE *l_ptr, *l_temp_line;
	long l_ttl = 0;
	char *l_temp1;

	if (start_default && End_default) {
		start = current;
		*errnum = 1;
		if ((start->below) != NULL)
			End = start->below;
		else
			return;
	} else
		if (start_default) {
			if (rol(inputt, errnum))
				return;

			if (End == NULL) {
				strcpy(help_msg, "bad address");
				*errnum = -1;
			} else {
#ifdef BSD
				/*
				 * For BSD a 'j' with one address sets
				 * "current" to that line
				 */
				if (start)
					current = start;
#endif
				*errnum = 1;
			}
			return;
		}
	if (start == NULL) {
		strcpy(help_msg, "bad address");
		*errnum = -1;
		ungetc('\n', inputt);
		return;
	}
	start_default = End_default = 0;
	if (sigint_flag)
		SIGINT_ACTION;

	if (rol(inputt, errnum))
		return;

	if (g_flag == 0)
		u_clr_stk();
	u_set = 1;		/* set for d */

	/* Figure out what the length of the joined lines will be. */
	for (l_ptr = start; l_ptr != (End->below); l_ptr = (l_ptr->below))
		l_ttl = l_ptr->len + l_ttl;

	if (l_ttl > nn_max) {
		/*
		 * The new line is bigger than any so far, so make more
		 * space.
		 */
		free(text);
		nn_max = l_ttl;
		text = calloc(l_ttl + 2, sizeof(char));
		if (text == NULL) {
			*errnum = -1;
			strcpy(help_msg, "out of memory error");
			return;
		}
	}
	if (sigint_flag)
		SIGINT_ACTION;
	l_temp1 = calloc(l_ttl + 2, sizeof(char));
	if (text == NULL) {
		*errnum = -1;
		strcpy(help_msg, "out of memory error");
		return;
	}
	l_temp1[0] = '\0';

	l_ptr = start;
	for (;;) {
		/* Get each line and catenate. */
		if (sigint_flag)
			goto point;
		get_line(l_ptr->handle, l_ptr->len);
		strcat(l_temp1, text);
		l_ptr = l_ptr->below;
		if (l_ptr == End->below)
			break;
	}

	l_temp_line = malloc(sizeof(LINE));
	if (text == NULL) {
		*errnum = -1;
		strcpy(help_msg, "out of memory error");
		return;
	}
	(l_temp_line->len) = l_ttl;
	/* Add the new line to the buffer. */
	(l_temp_line->handle) = add_line(l_temp1, l_ttl);
	if (start == top) {
		top = l_temp_line;
		(l_temp_line->above) = NULL;
	} else {
		(l_temp_line->above) = start->above;
		u_add_stk(&(l_temp_line->above->below));
		(l_temp_line->above->below) = l_temp_line;
	}
	(l_temp_line->below) = start;
	u_add_stk(&(start->above));
	(start->above) = l_temp_line;

	ungetc(ss, inputt);
	/* Delete the lines used to make the joined line. */
	d(inputt, errnum);
	if (*errnum < 0)
		return;
	*errnum = 0;
	current = l_temp_line;

	*errnum = 1;

point:	u_set = 0;
	free(l_temp1);
}
