/* Copyright (c) 1979 Regents of the University of California */

static char sccsid[] = "@(#)utilities.c 1.2 01/10/81";

#include	"vars.h"
#include	"panics.h"
#include	"h02opcs.h"

stats()
{
	struct	{
		long	usr_time;
		long	sys_time;
		long	child_usr_time;
		long	child_sys_time;
	} tbuf;
	register double l;
	register long count;
#	ifdef PROFILE
#	define	proffile	"/vb/grad/mckusick/px/profile/pcnt.out"
	struct cntrec {
		double	counts[NUMOPS];	/* instruction counts */
		long	runs;		/* number of interpreter runs */
		long	startdate;	/* date profile started */
		long	usrtime;	/* total user time consumed */
		long	systime;	/* total system time consumed */
		double	stmts;		/* number of pascal stmts executed */
	} profdata;
	FILE *datafile;
#	endif PROFILE

	if (_nodump)
		return(0);
	times(&tbuf);
#	ifdef PROFILE
	datafile = fopen(proffile,"r");
	if (datafile == NULL)
		goto skipprof;
	count = fread(&profdata,1,sizeof(profdata),datafile);
	if (count != sizeof(profdata))
		goto skipprof;
	for (count = 0;  count < NUMOPS;  count++)
		profdata.counts[count] += _profcnts[count];
	profdata.runs += 1;
	profdata.stmts += _stcnt;
	profdata.usrtime += tbuf.usr_time;
	profdata.systime += tbuf.sys_time;
	datafile = freopen(proffile,"w",datafile);
	if (datafile == NULL)
		goto skipprof;
	count = fwrite(&profdata,1,sizeof(profdata),datafile);
	if (count != sizeof(profdata))
		goto skipprof;
	fclose(datafile);
skipprof:
#	endif PROFILE
	l = tbuf.usr_time;
	l = l / HZ;
	fprintf(stderr,
		"\n%1ld statements executed in %04.2f seconds cpu time.\n",
		_stcnt,l);
}

backtrace(errnum)
	long	errnum;
{
	register struct disp *mydp;
	register struct stack *ap;
	register char *cp;
	register long i, linum;
	struct disply disp;

	if (_lino <= 0) {
		fputs("Program was not executed.\n",stderr);
		return;
	}
	disp = _display;
	if (errnum == PINTR)
		fputs("\n\tInterrupted in \"",stderr);
	else if (errnum == PHALT)
		fputs("\n\tHalted in \"",stderr);
	else
		fputs("\n\tError in \"",stderr);
	mydp = _dp;
	linum = _lino;
	for (;;) {
		ap = mydp->stp;
		i = linum - (((ap)->entry)->offset & 0177777);
		fprintf(stderr,"%s\"",(ap->entry)->name);
		if (_nodump == 0)
			fprintf(stderr,"+%1d near line %1d.",i,linum);
		fputc('\n',stderr);
		*mydp = (ap)->odisp;
		if (mydp <= &_display.frame[1]){
			_display = disp;
			psexit(errnum);
		}
		mydp = (ap)->dp;
		linum = (ap)->lino;
		fputs("\tCalled by \"",stderr);
	}
}

psexit(code)

	long	code;
{
	if (_pcpcount != 0)
		PMFLUSH(_cntrs, _rtns, _pcpcount);
	if (_mode == PIX) {
		fputs("Execution terminated",stderr);
		if (code)
			fputs(" abnormally",stderr);
		fputc('.',stderr);
		fputc('\n',stderr);
	}
	stats();
	exit(code);
}
