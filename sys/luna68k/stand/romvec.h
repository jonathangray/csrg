/*
 * Copyright (c) 1992 OMRON Corporation.
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * OMRON Corporation.
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
 *	@(#)romvec.h	7.1 (Berkeley) 12/13/92
 */

/* romvec.h Oct-22-1991 */


#define RVPtr	((struct romvec *) 0x41000000)

#define ROM_memsize	(*((int *) *RVPtr->vec03))
#define	ROM_getchar	(*RVPtr->vec06)
#define	ROM_putchar	(*RVPtr->vec07)
#define	ROM_abort	(*RVPtr->vec25)

struct romvec {
    int     (*vec00)();    /* 00 - Cold Boot Entry */
    int     (*vec01)();    /* 01 */
    int     (*vec02)();    /* 02 */
    int     (*vec03)();    /* 03 - memsize : Memory Size */
    int     (*vec04)();    /* 04 */
    int     (*vec05)();    /* 05 */
    int     (*vec06)();    /* 06 - getchar : get 1 charactor from console	*/
    int     (*vec07)();    /* 07 - putchar : put 1 charactor to console		*/
    int     (*vec08)();    /* 08 */
    int     (*vec09)();    /* 09 */
    int     (*vec10)();    /* 10 */
    int     (*vec11)();    /* 11 */
    int     (*vec12)();    /* 12 */
    int     (*vec13)();    /* 13 */
    int     (*vec14)();    /* 14 */
    int     (*vec15)();    /* 15 */
    int     (*vec16)();    /* 16 */
    int     (*vec17)();    /* 17 */
    int     (*vec18)();    /* 18 */
    int     (*vec19)();    /* 19 */
    int     (*vec20)();    /* 20 */
    int     (*vec21)();    /* 21 */
    int     (*vec22)();    /* 22 */
    int     (*vec23)();    /* 23 */
    int     (*vec24)();    /* 24 */
    int     (*vec25)();    /* 25 - abort : back to ROM Monitor */
    int     (*vec26)();    /* 26 */
    int     (*vec27)();    /* 27 */
    int     (*vec28)();    /* 28 */
    int     (*vec29)();    /* 29 */
    int     (*vec30)();    /* 30 */
    int     (*vec31)();    /* 31 */
    int     (*vec32)();    /* 32 */
    int     (*vec33)();    /* 33 */
    int     (*vec34)();    /* 34 */
    int     (*vec35)();    /* 35 */
    int     (*vec36)();    /* 36 */
    int     (*vec37)();    /* 37 */
    int     (*vec38)();    /* 38 */
    int     (*vec39)();    /* 39 */
    int     (*vec40)();    /* 40 */
    int     (*vec41)();    /* 41 */
    int     (*vec42)();    /* 42 */
    int     (*vec43)();    /* 43 */
    int     (*vec44)();    /* 44 */
    int     (*vec45)();    /* 45 */
    int     (*vec46)();    /* 46 */
    int     (*vec47)();    /* 47 */
    int     (*vec48)();    /* 48 */
    int     (*vec49)();    /* 49 */
    int     (*vec50)();    /* 50 */
    int     (*vec51)();    /* 51 */
    int     (*vec52)();    /* 52 */
    int     (*vec53)();    /* 53 */
    int     (*vec54)();    /* 54 */
    int     (*vec55)();    /* 55 */
    int     (*vec56)();    /* 56 */
    int     (*vec57)();    /* 57 */
    int     (*vec58)();    /* 58 */
    int     (*vec59)();    /* 59 */
    int     (*vec60)();    /* 60 */
    int     (*vec61)();    /* 61 */
    int     (*vec62)();    /* 62 */
    int     (*vec63)();    /* 63 */
    int     (*vec64)();    /* 64 */
    int     (*vec65)();    /* 65 */
    int     (*vec66)();    /* 66 */
    int     (*vec67)();    /* 67 */
    int     (*vec68)();    /* 68 */
    int     (*vec69)();    /* 69 */
};


