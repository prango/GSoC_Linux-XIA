OFILES=\
	src/hhash.o\
	src/hhset.o\
	src/hhmap.o\

CFILES=\
	src/hhash.c\
	src/mach.c\
	src/hhset.c\
	src/hhmap.c\

HFILES=\
	src/hhash.h\
	src/hhset.h\
	src/hhmap.h\

default: $(OFILES) libhhash.a

$(OFILES): $(CFILES) $(HFILES)

CFLAGS+=-std=c99 -O3

.PHONY: format
format: $(CFILES) $(HFILES)
	clang-format -i $^

libhhash.a: $(OFILES)
	ar rcs $@ $^

.PHONY: clean
clean:
	rm -f $(OFILES) libhhash.a
