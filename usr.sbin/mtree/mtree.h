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
 *
 *	@(#)mtree.h	5.3 (Berkeley) 05/25/90
 */

#include <string.h>
#include <stdlib.h>

typedef struct _info {
#define	F_BLOCK	0x001				/* block special */
#define	F_CHAR	0x002				/* char special */
#define	F_DIR	0x004				/* directory */
#define	F_FILE	0x008				/* regular file */
#define	F_LINK	0x010				/* symbolic link */
#define	F_SOCK	0x020				/* socket */
#define	F_NONE	0x040				/* unknown */
	u_short	type;				/* file type */

#define	F_CKSUM	0x001				/* check sum */
#define	F_GROUP	0x002				/* group */
#define	F_IGN	0x004				/* ignore */
#define	F_MODE	0x008				/* mode */
#define	F_NLINK	0x010				/* number of links */
#define	F_OWNER	0x020				/* owner */
#define	F_SIZE	0x040				/* size */
#define	F_SLINK	0x080				/* link count */
#define	F_TYPE	0x100				/* file type */
	u_short	flags;				/* items set */

	off_t	st_size;
	u_long	cksum;				/* check sum */
	uid_t	st_uid;
	gid_t	st_gid;
#define	MBITS	(S_ISUID|S_ISGID|S_ISTXT|S_IRWXU|S_IRWXG|S_IRWXO)
	mode_t	st_mode;
	nlink_t	st_nlink;
	char	*slink;				/* symbolic link reference */
} INFO;

typedef struct _entry {
	struct _entry	*child, *parent;	/* up, down */
	struct _entry	*next, *prev;		/* left, right */
	INFO	info;				/* node info structure */
#define	F_DONE	0x01				/* directory done */
#define	F_VISIT	0x02				/* visited this node */
	u_char	flags;				/* flags */
	char	*name;				/* node name */
} ENTRY;

#define	RP(p)	(p->fts_path + 2)
