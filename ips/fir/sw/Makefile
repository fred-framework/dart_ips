libname = fred
srcs = $(wildcard *.c) $(wildcard **/*.c)
objs = $(srcs:.c=.o)
deps = $(objs:.o=.d)

CFLAGS += -std=gnu99 -g -Wall -Werror -fpic
LDFLAGS += -shared

$(libname).so: $(objs)
	$(CC) -o $@ $^ $(LDFLAGS)

# include all dep
-include $(deps)

# Build header includes dependencies using c preprocessor
%.d: %.c
	$(CPP) $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	rm -f $(libname).so $(objs) $(deps)

