
bin_PROGRAMS = picoc

picoc_SOURCES = picoc.c table.c lex.c parse.c expression.c heap.c type.c \
	interface.c module.c variable.c clibrary.c platform.c include.c \
	attributes.c debug.c \
	platform/platform_unix.c platform/library_unix.c \
	cstdlib/stdio.c cstdlib/math.c cstdlib/string.c cstdlib/stdlib.c \
	cstdlib/time.c cstdlib/errno.c cstdlib/ctype.c cstdlib/stdbool.c \
	cstdlib/unistd.c

picoc_LDADD = @LOCAL_LIBS@

count:
	@echo "Core:"
	@cat picoc.h interpreter.h tokens.h picoc.c table.c lex.c parse.c expression.c platform.c heap.c type.c variable.c include.c debug.c | grep -v '^[ 	]*/\*' | grep -v '^[ 	]*$$' | wc
	@echo ""
	@echo "Everything:"
	@cat $(picoc_SOURCES) *.h */*.h | wc

