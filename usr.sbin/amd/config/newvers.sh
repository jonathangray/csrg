#!/bin/sh -
#
# $Id: newvers.sh,v 5.2 90/06/23 22:21:21 jsp Rel $
#
# Copyright (c) 1989 Jan-Simon Pendry
# Copyright (c) 1989 Imperial College of Science, Technology & Medicine
# Copyright (c) 1989 The Regents of the University of California.
# All Rights Reserved.
#
# This code is derived from software contributed to Berkeley by
# Jan-Simon Pendry at Imperial College, London.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#	This product includes software developed by the University of
#	California, Berkeley and its contributors.
# 4. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
#	@(#)newvers.sh	5.2 (Berkeley) 07/29/90
#
PATH=/usr/ucb:/bin:/usr/bin
if [ $# -ne 1 ]; then echo "Usage: newvers program" >&2; exit 1; fi
version="version.$1"
if [ ! -r $version ]; then echo 0 > $version; chmod 444 $version; fi
v=`cat $version`
u=${USER-${LOGNAME-root}}
h=`hostname`
#h=`expr "$h" : '\([^.]*\)'`
t=`date`
r=`cat $d../config/RELEASE`
c=`sed 's/$/\\\\n\\\\/' $d../text/COPYRIGHT`
if [ -z "$r" -o -z "$c" ]; then
	echo ERROR: config file missing >&2
	exit 1
fi
rm -f vers.$1.c
cat > vers.$1.c << %%
char version[] = "\\
${c}
$1 ${r} #${v}: ${t}\\n\\
Built by ${u}@${h}";
%%
rm -f $version
/bin/echo `expr ${v} + 1` > $version
chmod 444 $version
