# $Id: Makefile,v 1.2 2011-01-25 19:07:51-08 - - $

MKFILE      = Makefile
DEPSFILE    = ${MKFILE}.deps
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
UNAME      ?= ${shell uname -s}

COMPILECCC  = g++ -g -O0 -Wall -Wextra
MAKEDEPSCCC = g++ -MM

CCHEADER    = interp.h numbers.h object.h trace.h util.h util-inst.h
CCSOURCE    = main.cc interp.cc numbers.cc object.cc trace.cc util.cc
EXECBIN     = draw
OBJECTS     = ${CCSOURCE:.cc=.o}
OTHERS      = ${MKFILE} README
ALLSOURCES  = ${CCHEADER} ${CCSOURCE} ${OTHERS}
LISTFILES   = ${ALLSOURCES} ${DEPSFILE}

LISTING     = ../asg3-draw.code.ps
CLASS       = cmps109-wm.w10
PROJECT     = asg3

all : ${EXECBIN}
	- checksource ${ALLSOURCES}

${EXECBIN} : ${OBJECTS}
	${COMPILECCC} -o $@ ${OBJECTS}

%.o : %.cc
	${COMPILECCC} -c $<

ci : ${ALLSOURCES}
	@ - checksource ${ALLSOURCES}
	cid + ${ALLSOURCES}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${LISTFILES}

clean :
	- rm ${OBJECTS} ${DEPSFILE} core ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} 

submit : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	submit ${CLASS} ${PROJECT} ${ALLSOURCES}
	testsubmit ${CLASS} ${PROJECT} ${ALLSOURCES}

deps : ${CCSOURCE} ${CCHEADER}
	@ echo "# ${DEPSFILE} created `LC_TIME=C date`" >${DEPSFILE}
	${MAKEDEPSCCC} ${CCSOURCE} | sort | uniq >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${GMAKE} deps

again :
	${GMAKE} spotless deps ci all lis

ifeq (${NEEDINCL}, )
include ${DEPSFILE}
endif

