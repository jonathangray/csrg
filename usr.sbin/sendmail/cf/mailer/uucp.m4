PUSHDIVERT(-1)
#
# Copyright (c) 1983 Eric P. Allman
# Copyright (c) 1988 The Regents of the University of California.
# All rights reserved.
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
POPDIVERT
#####################################
###   UUCP Mailer specification   ###
#####################################

VERSIONID(@(#)uucp.m4	2.7 (Berkeley) 07/20/92)

Msuucp,	P=ifdef(`UUCP_MAILER', `UUCP_MAILER', /usr/bin/uux), F=mDFMhuU, S=12, R=12, M=100000
	A=uux - -r -z -a$f -gC $h!rmail ($u)

Muucp,	P=ifdef(`UUCP_MAILER', `UUCP_MAILER', /usr/bin/uux), F=DFMhuU, S=12, R=12, M=100000
	A=uux - -r -z -a$f -gC $h!rmail ($u)

S12

# don't qualify list:; syntax
R$* :;				$@ $1 :;

R$* < @ $j >			$1			strip local name
R$* < @ $- . UUCP >		$2 ! $1			convert to UUCP format
R$* < @ $+ >			$2 ! $1			convert to UUCP format
R$+				$: $U ! $1		prepend our name

PUSHDIVERT(4)
# resolve locally connected UUCP links
R< @ $=Y . UUCP > : $+		$#suucp $@ $1 $: $1:$2	@host.UUCP: ...
R< @ $=U . UUCP > : $+		$#uucp $@ $1 $: $1:$2	@host.UUCP: ...
R$+ < @ $=Y . UUCP >		$#suucp $@ $2 $: $1	user@host.UUCP
R$+ < @ $=U . UUCP >		$#uucp $@ $2 $: $1	user@host.UUCP
POPDIVERT
