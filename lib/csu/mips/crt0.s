/*
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
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
 *	@(#)crt0.s	5.3 (Berkeley) 07/27/92
 */

#include <machine/regdef.h>
#include <machine/machAsmDefs.h>

	.comm	environ, 4

NON_LEAF(__start, 24, ra)
	li	v0, 45		# illegal ULTRIX system call to switch to BSD
	syscall			#   system call numbers.
	lw	a0, 0(sp)	# get argc from stack
	la	gp, _gp		# init gp pointer
	addu	a1, sp, 4	# get pointer to argv
	addu	a2, a1, 4	# skip null pointer on stack
	sll	v0, a0, 2	# add number of argv pointers
	addu	a2, a2, v0	# final pointer to environment list
	sw	a2, environ	# save environment pointer
	subu	sp, sp, 24	# allocate standard frame
	.mask	0x80000000, -4
	sw	zero, 20(sp)	# clear return address for debugging
	jal	main		# v0 = main(argc, argv, env);
	move	a0, v0
	jal	exit		# exit(v0);
	break	0
END(__start)

LEAF(moncontrol)
	j	ra
END(moncontrol)

LEAF(_mcount)
	j	ra
END(_mcount)
