appname = fir
fred_dir = /fredsys/fred_lib
srcs = $(wildcard ./src/*.c)
# cpp compilation is not working yet
cxx_srcs += $(wildcard ./src/*.cpp)
objs = $(srcs:.c=.o) $(cxx_srcs:.cpp=.o)
deps = $(objs:.o=.d)

CFLAGS += -I $(fred_dir) -I $(fred_dir)/fred_support -g  # -Wall  -Werror
CXXFLAGS += $(CFLAGS)
LDFLAGS += -L $(fred_dir) -lfred

$(appname): $(objs)
	make -C $(fred_dir) CC=$(CC)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	make -C $(fred_dir) clean
	rm -f $(objs) $(deps) $(appname)
