/*-
 * Copyright (c) 1987 The Regents of the University of California.
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
 *	@(#)MAKEHOSTS.sh	5.3 (Berkeley) 05/24/90
 */

# program to link to
RSH=/usr/bin/rsh

# address search pattern to recognize local hosts
LOCALADDR='^128\.32'

# awk pattern for uninteresting (eg, long form) alias
SKIP='/^ucb|\.berkeley\.edu$|^$/'

# Special entries that don't get generated normally
SPECIAL='ucbvax ucbarpa'

rm -rf TMP_HOSTS hosts.bak
mkdir TMP_HOSTS
cd TMP_HOSTS

for i in `egrep "$LOCALADDR" ${DESTDIR}/etc/hosts | awk "\\$2 !~ $SKIP {print \\$2} \\$3 !~ $SKIP {print \\$3} \\$4 !~ $SKIP {print \\$4}"`; do
	ln -s $RSH $i
done

for i in $SPECIAL; do
	ln -s $RSH $i
done

cd ..
mv hosts hosts.bak
mv TMP_HOSTS hosts
rm -rf hosts.bak
