#!/bin/sh -
#
# This module is believed to contain source code proprietary to AT&T.
# Use and redistribution is subject to the Berkeley Software License
# Agreement and your Software Agreement with AT&T (Western Electric).
#
#	@(#)explain.sh	4.7 (Berkeley) 04/17/91
#

D=/usr/share/dict/explain.d
while	echo 'phrase?'
	read x
do
	case $x in
	[a-z]*)	sed -n /"$x"'.*	/s/\(.*\)	\(.*\)/use "\2" for "\1"/p' $D
	esac
done
