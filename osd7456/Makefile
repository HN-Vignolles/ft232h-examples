# (Copilot)

CC := gcc
AR := ar

# probe libftdi with pkg-config (try libftdi1 then libftdi), fallback to common defaults
PKG_CFLAGS := $(shell pkg-config --cflags libftdi1 2>/dev/null || pkg-config --cflags libftdi 2>/dev/null || echo "-I/usr/include")
PKG_LIBS   := $(shell pkg-config --libs libftdi1 2>/dev/null || pkg-config --libs libftdi 2>/dev/null || echo "-lftdi")

CFLAGS := -g -O2 -Wall -I. -I../libmpsse/src -I../libmpsse/include $(PKG_CFLAGS)
LDFLAGS := $(PKG_LIBS)

LIBDIR := ../libmpsse
LIBBUILD := $(LIBDIR)/build
LIBA := $(LIBBUILD)/libmpsse.a

LIB_SRCS := $(wildcard $(LIBDIR)/src/*.c)
LIB_OBJS := $(patsubst $(LIBDIR)/src/%.c,$(LIBBUILD)/%.o,$(LIB_SRCS))

SRCS := main.c max7456.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET := osd7456

.PHONY: all clean

all: $(LIBA) $(TARGET)

# build static lib from libmpsse sources
$(LIBA): $(LIB_OBJS)
	mkdir -p $(LIBBUILD)
	$(AR) rcs $@ $(LIB_OBJS)

$(LIBBUILD)/%.o: $(LIBDIR)/src/%.c
	mkdir -p $(LIBBUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# build example
$(TARGET): $(OBJS) $(LIBA)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBA) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB_OBJS) $(LIBA) $(TARGET)
	rm -rf $(LIBBUILD)