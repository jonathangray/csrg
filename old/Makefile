#	@(#)Makefile	5.31 (Berkeley) 02/24/93

SUBDIR=	compact hostid lex make yacc

.if   (${MACHINE} == "hp300")
SUBDIR+=as.hpux
.elif (${MACHINE} == "tahoe")
SUBDIR+=as.tahoe dbx pcc
.elif ${MACHINE} == "vax"
SUBDIR+=arff as.vax dbx flcopy pcc rxformat talk tp
.endif

.include <bsd.subdir.mk>
