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
static char sccsid[] = "@(#)nlist.c	5.1 (Berkeley) 02/06/91";
#endif /* not lint */

#include <sys/param.h>
#include <errno.h>
#include <limits.h>
#include <ndbm.h>
#include <a.out.h>
#include <kvm.h>
#include <unistd.h>
#include <stdio.h>

typedef struct nlist NLIST;
#define	_strx	n_un.n_strx
#define	_name	n_un.n_name

#define	BFLEN		1024
#define	VRS		"_version"

static char *kfile;

create_knlist(name, db)
	char *name;
	DBM *db;
{
	register char *bp;
	register int ch, len;
	struct exec ebuf;
	FILE *fstr, *fsym;
	NLIST nbuf;
	off_t string_off, symbol_off, symbol_size;
	off_t rel_off, vers_off;
	datum key, data;
	char sbuf[BFLEN];

	/* Two pointers, one for symbol table and one for string table. */
	kfile = name;
	if ((fsym = fopen(name, "r")) == NULL ||
	    (fstr = fopen(name, "r")) == NULL)
		error(name);

	if (fread((char *)&ebuf, sizeof(struct exec), 1, fsym) != 1)
		badfmt("no exec header");
	if (N_BADMAG(ebuf))
		badfmt("bad magic number");
		
	symbol_size = ebuf.a_syms;
	if (!symbol_size)
		badfmt("stripped");

	symbol_off = N_SYMOFF(ebuf);
	string_off = symbol_off + symbol_size;

	if (fseek(fsym, symbol_off, SEEK_SET) == -1)
		badfmt("corrupted symbol table");

	key.dptr = sbuf;
	data.dptr = (char *)&nbuf;
	data.dsize = sizeof(NLIST);

	for (; symbol_size; symbol_size -= sizeof(NLIST)) {
		if (fread((char *)&nbuf, sizeof (NLIST), 1, fsym) != 1)
			badfmt("corrupted symbol table");
		if (!nbuf._strx || nbuf.n_type&N_STAB)
			continue;
		if (fseek(fstr, string_off + nbuf._strx, SEEK_SET) == -1)
			badfmt("corrupted string table");

		/* Read string. */
		bp = sbuf;
		for (len = 0; (ch = getc(fstr)) != EOF && ch != '\0';) {
			if (++len == BFLEN) {
				(void)fprintf(stderr,
				    "kvm_mkdb: symbol too long.");
				break;
			}
			*bp++ = ch;
		}
		if (len == BFLEN)
			continue;

		/* Store string. */
		key.dsize = len;
		if (dbm_store(db, key, data, DBM_INSERT) < 0)
			error("dbm_store");

		if (!strncmp(sbuf, VRS, sizeof(VRS) - 1)) {
			/* Offset relative to start of text image in VM. */
#ifdef hp300
			rel_off = nbuf.n_value;
#endif
#ifdef tahoe
			/*
			 * On tahoe, first 0x800 is reserved for communication
			 * with the console processor.
			 */
			rel_off = ((nbuf.n_value & ~KERNBASE) - 0x800);
#endif
#ifdef vax
			rel_off = nbuf.n_value & ~KERNBASE;
#endif
			/*
			 * When loaded, data is rounded to next page cluster
			 * after text, but not in file.
			 */
			rel_off -= CLBYTES - (ebuf.a_text % CLBYTES);
			vers_off = N_TXTOFF(ebuf) + rel_off;
			if (fseek(fstr, vers_off, SEEK_SET) == -1)
				badfmt("corrupted string table");

			/*
			 * Read version string up to, and including newline.
			 * This code assumes that a newline terminates the
			 * version line.
			 */
			if (fgets(sbuf, sizeof(sbuf), fstr) == NULL)
				badfmt("corrupted string table");

			key.dptr = VERSION;
			key.dsize = sizeof(VERSION) - 1;
			data.dptr = sbuf;
			data.dsize = strlen(sbuf);
			if (dbm_store(db, key, data, DBM_INSERT) < 0)
				error("dbm_store");

			/* Restore to original values. */
			key.dptr = sbuf;
			data.dptr = (char *)&nbuf;
			data.dsize = sizeof(NLIST);
		}
	}
	(void)fclose(fstr);
	(void)fclose(fsym);
}

badfmt(p)
{
	(void)fprintf(stderr,
	    "symorder: %s: %s: %s\n", kfile, p, strerror(EFTYPE));
	exit(1);
}
