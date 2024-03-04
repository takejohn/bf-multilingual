CC       = gcc
CFLAGS   = -Wall -std=c99 -g -O0
RM       = rm -rf
MKDIR    = mkdir -p
NAME     = bf
BUILDDIR = build/

SRCS     = $(wildcard src/*.c)
OBJS     = $(SRCS:%.c=$(BUILDDIR)%.o)
INCLDIR  = include
BFSRC    = ../../examples/hello.bf

.PHONY: run build clean

run: $(BUILDDIR)$(NAME)
	./$(BUILDDIR)$(NAME) $(BFSRC)

build: $(BUILDDIR)$(NAME)

clean:
	$(RM) $(BUILDDIR)

$(BUILDDIR)$(NAME): $(BUILDDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(BUILDDIR)%.o: %.c
	$(MKDIR) $(BUILDDIR)$(dir $<)
	$(CC) $(CFLAGS) -I $(INCLDIR) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $@
