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
static char sccsid[] = "@(#)add_line.c	5.1 (Berkeley) 01/23/93";
#endif /* not lint */

#include "ed.h"

/*
 * This is where the lines actually are put into the buffer. Notice
 * all the #ifdef's for the various methods of buffering - POSIX said
 * no "no attempt is made to imply a specific implementation". So,
 * you get your choice: standard I/O, core memory, or a database.
 */

#ifdef STDIO
long
add_line(p, len)
#endif
#ifdef DBI
recno_t
add_line(p, len)
#endif
#ifdef MEMORY
char
*add_line(p, len)
#endif

char *p;
long len;

{
#ifdef STDIO
  long l_key;
  extern int file_loc;

  if (file_seek)  /* x-ref to get_line for what this does */
    {
      file_seek = 0;
      fseek(fhtmp, 0L, 2); /* set to end-to-file */
    }
  l_key = ftell(fhtmp);
  file_loc = l_key + fwrite(p, sizeof(char), len, fhtmp); /* keeps user time down 20%approx */
  return(l_key);
#endif

#ifdef DBI
  DBT db_key, db_data;
  static recno_t l_key=0;

  l_key++;
  (db_key.data) = &l_key;
  (db_key.size) = sizeof(recno_t);
  (db_data.data) = p;
  (db_data.size) = len;
  (dbhtmp->put)(dbhtmp, &db_key, &db_data, (u_int)(R_NOOVERWRITE));
  return(l_key);
#endif

#ifdef MEMORY
  char *tmp;
  tmp = (char *)calloc(len+1, sizeof(char));
  if (tmp)
    {
      bcopy(p, tmp, len);
      tmp[len] = '\0';
    }
  return(tmp);
#endif

} /* end-add_line */
