/*
 * Copyright (c) 1983 Eric P. Allman
 * Copyright (c) 1988 Regents of the University of California.
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
static char sccsid [] = "@(#)udb.c	5.2 (Berkeley) 09/23/91";
#endif

#include "sendmail.h"

#ifdef USERDB

#include <sys/file.h>
#include <db.h>

/*
**  UDBEXPAND -- look up user in database and expand
**
**	Parameters:
**		a -- address to expand.
**		sendq -- pointer to head of sendq to put the expansions in.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Modifies sendq.
*/

void
udbexpand(a, sendq)
	register ADDRESS *a;
	ADDRESS **sendq;
{
	int i;
	register char *p;
	auto char *class;
	auto char *list;
	DBT key;
	DBT info;
	static DB *dbp = NULL;
	register char *bp;
	char buf[8192];

	if (tTd(28, 1))
		printf("expand(%s)\n", a->q_paddr);

	/* make certain we are supposed to send to this address */
	if (bitset(QDONTSEND, a->q_flags))
		return;
	CurEnv->e_to = a->q_paddr;

	/* if necessary, open the database */
	if (dbp == NULL)
	{
		if (UdbFileName == NULL || UdbFileName[0] == '\0')
		{
			if (tTd(28, 4))
				printf("no userdb specified\n");
			return;
		}
		dbp = dbopen(UdbFileName, O_RDONLY, 0644, DB_HASH, NULL);
		if (dbp == NULL)
		{
			extern int errno;

			if (tTd(28, 2))
				printf("cannot open %s: %d\n", UdbFileName, errno);
			return;
		}
	}

	key.data = a->q_user;
	key.size = strlen(key.data);
	i = dbp->get(dbp, &key, &info, R_NOOVERWRITE);
	if (i != 0 || info.size <= 0)
	{
		if (i < 0)
			syserr("udbexpand: db-get stat %s");
		if (tTd(28, 2))
			printf("expand: no match on %s\n", key.data);
		return;
	}

	/* extract the class (first string) and data (second string) */
	i = strlen((char *) info.data) + 1;
	p = (char *) info.data + i;
	i = info.size - i;

	/* use internal buffer if it will fit; otherwise malloc */
	if (i < sizeof buf)
		bp = buf;
	else
		bp = xalloc(i + 1);
	bcopy(p, bp, i);
	bp[i] = '\0';

	if (tTd(28, 1))
		printf("Class %s: %s\n", info.data, bp);

	/* do special processing based on class */
	if (strcmp((char *) info.data, "user") == 0)
	{
		message(Arpa_Info, "expanded to (%s) %s", info.data, bp);
		AliasLevel++;
		sendtolist(bp, a, sendq);
		AliasLevel--;
	}

	/* free memory if we allocated it */
	if (bp != buf)
		free(bp);
}

#endif /* USERDB */
