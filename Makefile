CFLAGS ?= -g -O2 -I/usr/local/include/nginx
XCFLAGS =
override CFLAGS += -Wall -D_GNU_SOURCE

all_targets += main

all: $(all_targets)

test_srcs := main.c

test_objs := $(patsubst %.c,%.ol,$(test_srcs))

%: %.c
	$(QUIET_CC)$(CC) $(CFLAGS) -o $@ $< -L/usr/local/lib -lnginx -ldl -lpthread -lcrypt -lpcre -lz -Wl,-E $(XCFLAGS)

clean:
	@rm -f $(all_targets) $(test_objs)
