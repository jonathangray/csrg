PUSHDIVERT(-1)
#
# Copyright (c) 1983 Eric P. Allman
# Copyright (c) 1988, 1993
#	The Regents of the University of California.  All rights reserved.
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

ifdef(`UUCP_MAILER_PATH',, `define(`UUCP_MAILER_PATH', /usr/bin/uux)')
ifdef(`UUCP_MAILER_ARGS',, `define(`UUCP_MAILER_ARGS', `uux - -r -z -a$f -gC $h!rmail ($u)')')
ifdef(`UUCP_MAILER_FLAGS',, `define(`UUCP_MAILER_FLAGS', `')')
ifdef(`UUCP_MAX_SIZE',, `define(`UUCP_MAX_SIZE', 100000)')
POPDIVERT
#####################################
###   UUCP Mailer specification   ###
#####################################

VERSIONID(`@(#)uucp.m4	8.4 (Berkeley) 07/13/93')

# old UUCP mailer
Muucp,		P=UUCP_MAILER_PATH, F=CONCAT(DFMhuU, UUCP_MAILER_FLAGS), S=12, R=22, M=UUCP_MAX_SIZE,
		A=UUCP_MAILER_ARGS

# smart UUCP mailer (handles multiple addresses)
Msuucp,		P=UUCP_MAILER_PATH, F=CONCAT(mDFMhuU, UUCP_MAILER_FLAGS), S=12, R=22, M=UUCP_MAX_SIZE,
		A=UUCP_MAILER_ARGS

ifdef(`_MAILER_smtp_',
`# domain-ized UUCP mailer
Muucp-dom,	P=UUCP_MAILER_PATH, F=CONCAT(mDFMhu, UUCP_MAILER_FLAGS), S=11, R=21, M=UUCP_MAX_SIZE,
		A=UUCP_MAILER_ARGS')


# sender rewriting
S12

# handle error address as a special case
R<@>				$n			errors to mailer-daemon

# do not qualify list:; syntax
R$* :; <@>			$@ $1 :;

R$* < @ $* . >			$1 < @ $2 >		strip trailing dots
R$* < @ $j >			$1			strip local name
R$* < @ $- . UUCP >		$2 ! $1			convert to UUCP format
R$* < @ $+ >			$2 ! $1			convert to UUCP format
R$+				$: $U ! $1		prepend our name

# recipient rewriting
S22

# don't touch list:; syntax
R$* :; <@>			$@ $1 ;:

R$* < @ $* . >			$1 < @ $2 >		strip trailing dots
R$* < @ $j >			$1			strip local name
R$* < @ $- . UUCP >		$2 ! $1			convert to UUCP format
R$* < @ $+ >			$2 ! $1			convert to UUCP format

PUSHDIVERT(4)
# resolve locally connected UUCP links
R< @ $=Y . UUCP > : $+		$#suucp $@ $1 $: $2	@host.UUCP: ...
R< @ $=U . UUCP > : $+		$#uucp $@ $1 $: $2	@host.UUCP: ...
R$+ < @ $=Y . UUCP >		$#suucp $@ $2 $: $1	user@host.UUCP
R$+ < @ $=U . UUCP >		$#uucp $@ $2 $: $1	user@host.UUCP
POPDIVERT
