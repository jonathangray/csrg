/*-
 * Copyright (c) 1992 The Regents of the University of California.
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
static char sccsid[] = "@(#)qdivrem.c	5.3 (Berkeley) 05/12/92";
#endif /* LIBC_SCCS and not lint */

#include "longlong.h"

static int bshift ();

/* Divide a by b, producing quotient q and remainder r.

       sizeof a is m
       sizeof b is n
       sizeof q is m - n
       sizeof r is n

   The quotient must fit in m - n bytes, i.e., the most significant
   n digits of a must be less than b, and m must be greater than n.  */

/* The name of this used to be __div_internal,
   but that is too long for SYSV.  */

void 
__bdiv (a, b, q, r, m, n)
     unsigned short *a, *b, *q, *r;
     size_t m, n;
{
  unsigned long qhat, rhat;
  unsigned long acc;
  unsigned short *u = (unsigned short *) alloca (m);
  unsigned short *v = (unsigned short *) alloca (n);
  unsigned short *u0, *u1, *u2;
  unsigned short *v0;
  int d, qn;
  int i, j;

  m /= sizeof *a;
  n /= sizeof *b;
  qn = m - n;

  /* Remove leading zero digits from divisor, and the same number of
     digits (which must be zero) from dividend.  */

  while (b[big_end (n)] == 0)
    {
      r[big_end (n)] = 0;

      a += little_end (2);
      b += little_end (2);
      r += little_end (2);
      m--;
      n--;

      /* Check for zero divisor.  */
      if (n == 0)
      {
	*r /= n;	/* force a divide-by-zero trap */
	return;
      }
    }
      
  /* If divisor is a single digit, do short division.  */

  if (n == 1)
    {
      acc = a[big_end (m)];
      a += little_end (2);
      for (j = big_end (qn); is_not_lsd (j, qn); j = next_lsd (j))
	{
	  acc = (acc << 16) | a[j];
	  q[j] = acc / *b;
	  acc = acc % *b;
	}
      *r = acc;
      return;
    }

  /* No such luck, must do long division. Shift divisor and dividend
     left until the high bit of the divisor is 1.  */

  for (d = 0; d < 16; d++)
    if (b[big_end (n)] & (1 << (16 - 1 - d)))
      break;

  bshift (a, d, u, 0, m);
  bshift (b, d, v, 0, n);

  /* Get pointers to the high dividend and divisor digits.  */

  u0 = u + big_end (m) - big_end (qn);
  u1 = next_lsd (u0);
  u2 = next_lsd (u1);
  u += little_end (2);

  v0 = v + big_end (n);

  /* Main loop: find a quotient digit, multiply it by the divisor,
     and subtract that from the dividend, shifted over the right amount. */

  for (j = big_end (qn); is_not_lsd (j, qn); j = next_lsd (j))
    {
      /* Quotient digit initial guess: high 2 dividend digits over high
	 divisor digit.  */

      if (u0[j] == *v0)
	{
	  qhat = B - 1;
	  rhat = (unsigned long) *v0 + u1[j];
	}
      else
	{
	  unsigned long numerator = ((unsigned long) u0[j] << 16) | u1[j];
	  qhat = numerator / *v0;
	  rhat = numerator % *v0;
	}

      /* Now get the quotient right for high 3 dividend digits over
	 high 2 divisor digits.  */

      while (rhat < B && qhat * *next_lsd (v0) > ((rhat << 16) | u2[j]))
	{
	  qhat -= 1;
	  rhat += *v0;
	}
	    
      /* Multiply quotient by divisor, subtract from dividend.  */

      acc = 0;
      for (i = little_end (n); is_not_msd (i, n); i = next_msd (i))
	{
	  acc += (unsigned long) (u + j)[i] - v[i] * qhat;
	  (u + j)[i] = acc & low16;
	  if (acc < B)
	    acc = 0;
	  else
	    acc = (acc >> 16) | -B;
	}

      q[j] = qhat;

      /* Quotient may have been too high by 1.  If dividend went negative,
	 decrement the quotient by 1 and add the divisor back.  */

      if ((signed long) (acc + u0[j]) < 0)
	{
	  q[j] -= 1;
	  acc = 0;
	  for (i = little_end (n); is_not_msd (i, n); i = next_msd (i))
	    {
	      acc += (unsigned long) (u + j)[i] + v[i];
	      (u + j)[i] = acc & low16;
	      acc = acc >> 16;
	    }
	}
    }

  /* Now the remainder is what's left of the dividend, shifted right
     by the amount of the normalizing left shift at the top.  */

  r[big_end (n)] = bshift (u + 1 + little_end (j - 1),
			   16 - d,
			   r + little_end (2),
			   u[little_end (m - 1)] >> d,
			   n - 1);
}

/* Left shift U by K giving W; fill the introduced low-order bits with
   CARRY_IN.  Length of U and W is N.  Return carry out.  K must be
   in 0 .. 16.  */

static int
bshift (u, k, w, carry_in, n)
     unsigned short *u, *w, carry_in;
     int k, n;
{
  unsigned long acc;
  int i;

  if (k == 0)
    {
      while (n-- > 0)
        *w++ = *u++;
      return 0;
    }

  acc = carry_in;
  for (i = little_end (n); is_not_msd (i, n); i = next_msd (i))
    {
      acc |= (unsigned long) u[i] << k;
      w[i] = acc & low16;
      acc = acc >> 16;
    }
  return acc;
}
