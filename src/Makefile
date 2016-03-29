include ../rules.mk

inc  = -I../include
libs = 

srcs = $(wildcard *.cpp)
objs = $(srcs:.cpp=.o)
bin  = $(BIN_DIR)/prestore

.PHONY: all
all: $(bin)
	@echo "all made!"

$(bin): $(objs)
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(libs)

.cpp.o:
	$(CPP) $(CPP_FLAGS) $(inc) -c $<

clean:
	@echo "deleting $(bin)"
	@$(RM) $(bin)
	@echo "deleting $(objs)"
	@$(RM) $(objs)

