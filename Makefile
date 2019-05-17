EXE=sc
CC=gcc
CFLAGS=-lm
LDFLAGS=-lm
PREFIX=/usr/bin
CFAST=-s -Os -march=native
LDFAST=-s -Os -march=native
DEBUG=-std=c99 -g3 -ggdb3 -pg -O0 -Wall -Wextra -pedantic  -Wdeclaration-after-statement \
-Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
-Wno-missing-braces -Wno-missing-field-initializers -Wformat=2 \
-Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith \
-Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline \
-Wundef -Wnested-externs -Wcast-qual -Wunreachable-code \
-Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
-Wold-style-definition -Wwrite-strings \
-fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing \
-fprofile-arcs -ftest-coverage -Wp,-D_FORTIFY_SOURCE=2 \
-Winit-self -Wpacked -Wmissing-declarations -Wmissing-format-attribute \
-Wmissing-noreturn -Wnested-externs -Wsign-compare -Wunsafe-loop-optimizations \
-Werror-implicit-function-declaration -Wmissing-include-dirs -Wmissing-field-initializers \
-Wvolatile-register-var -Wdisabled-optimization -Wsign-conversion \
-Wno-long-long -Winvalid-pch -Wlogical-op -Wunused-result -Wdisabled-optimization 
CDEBUG=$(DEBUG)
LDDEBUG=$(DEBUG)
SRC=sc.c lex.c parse.c
OBJ=$(SRC:.c=.o)

all: $(EXE)

debug: CFLAGS += $(CDEBUG)
debug: LDFLAGS += $(LDDEBUG)
debug: clean
debug: $(EXE)

fast: CFLAGS += $(CFAST)
fast: LDFLAGS += $(LDFAST)
fast: clean
fast: $(EXE)

.PHONY: install
install: fast
	mkdir -p $(PREFIX)
	cp $(EXE) $(PREFIX)

.PHONY: remove
remove:
	rm $(PREFIX)/$(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXE) *.gcda *.gcno gmon.out
