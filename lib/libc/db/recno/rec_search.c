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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)rec_search.c	5.1 (Berkeley) 09/04/91";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <errno.h>
#include <db.h>
#include <stdio.h>
#include "../btree/btree.h"

/*
 * __REC_SEARCH -- Search a btree for a key.
 *
 * Parameters:
 *	t:	tree to search
 *	key:	key to find
 *	exactp:	pointer to exact match flag
 *
 * Returns:
 *	EPG for matching record, if any, or the EPG for the location of the
 *	key, if it were inserted into the tree.
 *
 * Warnings:
 *	The EPG returned is in static memory, and will be overwritten by the
 *	next search of any kind in any tree.
 */
EPG *
__rec_search(t, recno, exactp)
	BTREE *t;
	recno_t recno;
	int *exactp;
{
	static EPG e;
	register index_t index;
	register PAGE *h;
	RINTERNAL *r;
	pgno_t pg;
	index_t top;
	recno_t total;

	for (pg = P_ROOT, total = 0;;) {
		if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL)
			return (NULL);
		if (h->flags & P_RLEAF) {
			e.page = h;
			e.index = recno - total;
			top = NEXTINDEX(h);

			if (e.index > top) {
				mpool_put(t->bt_mp, h, 0);
				errno = EINVAL;
				return (NULL);
			}

			*exactp = e.index < top ? 1 : 0;
			return (&e);
		}

		for (index = 0, top = NEXTINDEX(h);;) {
			r = GETRINTERNAL(h, index);
			if (++index == top || total + r->nrecs >= recno)
				break;
			total += r->nrecs;
		}

		if (bt_push(t, h->pgno, index - 1) == RET_ERROR)
			return (NULL);

		pg = r->pgno;
		mpool_put(t->bt_mp, h, 0);
	}
}
