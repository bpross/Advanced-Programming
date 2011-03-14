# Makefile for Dining Philosophers

MAINCLASS  = DiningPhilosophers
JAVAC 	   = javac
JAVASRC    = $(wildcard *.java)
SOURCES    = $(JAVASRC) makefile README PARTNER
CLASSES    = $(patsubst %.java, %.class, $(JAVASRC))
JARCLASSES = $(patsubst %.class, %*.class, $(CLASSES))
JARFILE    = $(MAINCLASS)

all: $(JARFILE)

$(JARFILE): $(CLASSES)
	echo Main-class: $(MAINCLASS) > Manifest
	jar cvfm $(JARFILE) Manifest $(JARCLASSES)
	chmod +x $(JARFILE)
	rm Manifest

%.class: %.java
	$(JAVAC) $<

clean:
	rm -f *.class $(JARFILE)

submit:
	submit cmps109-wm.w11 asg5 $(SOURCES)
	
