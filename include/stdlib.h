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
 *	@(#)stdlib.h	5.7 (Berkeley) 02/18/91
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <machine/types.h>

#ifdef	_SIZE_T_
typedef	_SIZE_T_	size_t;
#undef	_SIZE_T_
#endif

#ifdef	_WCHAR_T_
typedef	_WCHAR_T_	wchar_t;
#undef	_WCHAR_T_
#endif

typedef struct {
	int quot;		/* quotient */
	int rem;		/* remainder */
} div_t;
typedef struct {
	long quot;		/* quotient */
	long rem;		/* remainder */
} ldiv_t;

#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0

#define	RAND_MAX	0x7ffffffff

#define	MB_CUR_MAX	1	/* XXX */

#include <sys/cdefs.h>

__BEGIN_DECLS
void	 abort __P((void));
int	 abs __P((int));
int	 atexit __P((void (*)(void)));
double	 atof __P((const char *_nptr));
int	 atoi __P((const char *_nptr));
long	 atol __P((const char *_nptr));
void	*bsearch __P((const void *_key, const void *_base, size_t _nmemb,
	    size_t _size, int (*_compar)(const void *, const void *)));
void	*calloc __P((size_t _nmemb, size_t _size));
div_t	 div __P((int _numer, int _denom));
void	 exit __P((int _status));
void	 free __P((void *_ptr));
char	*getenv __P((const char *_string));
long	 labs __P((long));
ldiv_t	 ldiv __P((long _numer, long _denom));
void	*malloc __P((size_t _size));
void	 qsort __P((void *_base, size_t _nmemb, size_t _size,
	    int (*_compar)(const void *, const void *)));
int	 rand __P((void));
void	*realloc __P((void *_ptr, size_t _size));
void	 srand __P((unsigned _seed));
long	 strtol __P((const char *_nptr, char **_endptr, int _base));
unsigned long
	 strtoul __P((const char *_nptr, char **_endptr, int _base));
int	 system __P((const char *_string));

/* these are currently just stubs */
int	mblen __P((const char *_s, size_t _n));
size_t	mbstowcs __P((wchar_t *_pwcs, const char *_s, size_t _n));
int	wctomb __P((char *_s, wchar_t _wchar));
int	mbtowc __P((wchar_t *_pwc, const char *_s, size_t _n));
size_t	wcstombs __P((char *_s, const wchar_t *_pwcs, size_t _n));

#ifndef _ANSI_SOURCE
void	 cfree __P((void *_ptr));
int	putenv __P((const char *_string));
int	setenv __P((const char *_string, const char *_value, int _overwrite));
#endif

#ifdef NOT_YET_IMPLEMENTED
double	strtod __P((const char *_nptr, char **_endptr));
#endif
__END_DECLS

#endif /* _STDLIB_H_ */
