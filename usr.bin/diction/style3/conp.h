/*-
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 *
 *	@(#)conp.h	4.4 (Berkeley) 04/17/91
 */

#define SLENG 250
#define SCHAR 1500
extern struct ss {char *sp,ic,cc; int leng;} sent[SLENG];
extern struct ss *sentp;
extern comma,j,i;
extern int nsleng;
extern question;
int must;
int be;
int sav;
int prep;
int aflg,bflg,subty,verb,verbty;
int hflg;
int iverb,pverb,done;
