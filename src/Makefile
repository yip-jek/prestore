include ../rules.mk

srcs = $(wildcard *.cpp)
objs = $(srcs:.cpp=.o)
bin  = $(BIN_DIR)/prestore

.PHONY: all
all: $(bin)
	@echo "all made!"

$(bin): $(objs)
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(LIBS)

.cpp.o:
	$(CPP) $(CPP_FLAGS) $(INCLS) -c $<

clean:
	@echo "deleting $(bin)"
	@$(RM) $(bin)
	@echo "deleting $(objs)"
	@$(RM) $(objs)

