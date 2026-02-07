# Minimal Makefile to build libmpsse (from ../libmpsse) as a static lib
# and compile the ssd1306 example. Assumes Linux/gcc and -lftdi is available.
# (Copilot)

CC := gcc
AR := ar

# probe libftdi with pkg-config (try libftdi1 then libftdi), fallback to common defaults
PKG_CFLAGS := $(shell pkg-config --cflags libftdi1 2>/dev/null || pkg-config --cflags libftdi 2>/dev/null || echo "-I/usr/include")
PKG_LIBS   := $(shell pkg-config --libs libftdi1 2>/dev/null || pkg-config --libs libftdi 2>/dev/null || echo "-lftdi")

CFLAGS := -O2 -Wall -I. -I../libmpsse/src -I../libmpsse/include $(PKG_CFLAGS)
LDFLAGS := $(PKG_LIBS)

LIBDIR := ../libmpsse
LIBBUILD := $(LIBDIR)/build
LIBA := $(LIBBUILD)/libmpsse.a

LIB_SRCS := $(wildcard $(LIBDIR)/src/*.c)
LIB_OBJS := $(patsubst $(LIBDIR)/src/%.c,$(LIBBUILD)/%.o,$(LIB_SRCS))

SRCS := main.c ssd1306_ftdi.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET := ssd1306

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