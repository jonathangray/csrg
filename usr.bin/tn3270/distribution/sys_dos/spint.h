/*
 * The 'spint' (spawn and interrupt) routines use this structure.
 *
 * Note that spint_asm.asm contains an Assembly language version of
 * the following, so keep changes in synch!
 *
 * @(#)spint.h	3.1 (Berkeley) 08/11/87
 */

typedef struct {
    union REGS		regs;
    struct SREGS	sregs;
    int			int_no;	/* Which interrupt to wait on */
    int			done;	/* Are we done, or just took an interrupt? */
    int			rc;	/* return code */
} Spint;
