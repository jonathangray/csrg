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
static char sccsid[] = "@(#)rec_close.c	5.3 (Berkeley) 06/23/92";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>
#include <sys/uio.h>
#include <errno.h>
#include <db.h>
#include <unistd.h>
#include <stdio.h>
#include "recno.h"

/*
 * __REC_CLOSE -- Close a recno tree.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS
 */
int
__rec_close(dbp)
	DB *dbp;
{
	BTREE *t;
	int rval;

	if (__rec_sync(dbp) == RET_ERROR)
		return (RET_ERROR);

	/* Committed to closing. */
	t = dbp->internal;
	rval = t->bt_rfp == NULL ? close(t->bt_rfd) : fclose(t->bt_rfp);

	if (__bt_close(dbp) == RET_ERROR)
		return (RET_ERROR);

	return (rval ? RET_ERROR : RET_SUCCESS);
}

/*
 * __REC_SYNC -- sync the recno tree to disk.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *
 * Returns:
 *	RET_SUCCESS, RET_ERROR.
 *
 * XXX
 * Currently don't handle a key marked for deletion when the tree is synced.
 * Should copy the page and write it out instead of the real page.
 */
int
__rec_sync(dbp)
	const DB *dbp;
{
	struct iovec iov[2];
	BTREE *t;
	DBT data, key;
	off_t off;
	recno_t scursor;
	int status;

	t = dbp->internal;

	if (ISSET(t, BTF_INMEM) || NOTSET(t, BTF_MODIFIED))
		return (RET_SUCCESS);

	if (ISSET(t, BTF_RDONLY)) {
		errno = EPERM;
		return (RET_ERROR);
	}

	/* Suck any remaining records into the tree. */
	if (t->bt_irec(t, MAX_REC_NUMBER) == RET_ERROR)
		return (RET_ERROR);

	/* Rewind the file descriptor. */
	if (lseek(t->bt_rfd, 0L, SEEK_SET) != 0L)
		return (RET_ERROR);

	iov[1].iov_base = "\n";
	iov[1].iov_len = 1;
	scursor = t->bt_rcursor;

	status = (dbp->seq)(dbp, &key, &data, R_FIRST);
        while (status == RET_SUCCESS) {
		iov[0].iov_base = data.data;
		iov[0].iov_len = data.size;
		if (writev(t->bt_rfd, iov, 2) != data.size + 1)
			return (RET_ERROR);
                status = (dbp->seq)(dbp, &key, &data, R_NEXT);
        }
	t->bt_rcursor = scursor;
	if (status == RET_ERROR)
		return (RET_ERROR);
	if ((off = lseek(t->bt_rfd, 0L, SEEK_CUR)) == -1)
		return (RET_ERROR);
	if (ftruncate(t->bt_rfd, off))
		return (RET_ERROR);
	UNSET(t, BTF_MODIFIED);
	return (RET_SUCCESS);
}
