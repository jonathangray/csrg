/*-
 * Copyright (c) 1991 The Regents of the University of California.
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
 *	@(#)extern.h	5.2 (Berkeley) 09/08/91
 */

BUFHEAD	*__add_ovflpage __P((BUFHEAD *));
int	 __addel __P((BUFHEAD *, const DBT *, const DBT *));
int	 __big_delete __P((BUFHEAD *));
int	 __big_insert __P((BUFHEAD *, const DBT *, const DBT *));
int	 __big_keydata __P((BUFHEAD *, DBT *, DBT *, int));
int	 __big_return __P((BUFHEAD *, int, DBT *, int));
int	 __big_split __P((BUFHEAD *, BUFHEAD *, BUFHEAD *,
		int, u_int, SPLIT_RETURN *));
int	 __buf_free __P((int, int));
void	 __buf_init __P((int));
u_int	 __call_hash __P((char *, int));
int	 __delpair __P((BUFHEAD *, int));
int	 __expand_table __P((void));
int	 __find_bigpair __P((BUFHEAD *, int, char *, int));
u_short	 __find_last_page __P((BUFHEAD **));
void	 __free_ovflpage __P((BUFHEAD *));
BUFHEAD	*__get_buf __P((u_int, BUFHEAD *, int));
int	 __get_page __P((char *, u_int, int, int, int));
int	 __init_bitmap __P((int, int, int));
u_int	 __log2 __P((u_int));
int	 __put_page __P((char *, u_int, int, int));
void	 __reclaim_buf __P((BUFHEAD *));
int	 __split_page __P((u_int, u_int));

extern HTAB *hashp;
#ifdef HASH_STATISTICS
extern long hash_accesses, hash_collisions, hash_expansions, hash_overflows;
#endif
extern int (*default_hash) __P((u_char *, int));
