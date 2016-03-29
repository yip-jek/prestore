############################################################################
BIN = ../bin


############################################################################
ifeq ($(shell uname -m), i686) # 32 bit OS
OS_BITS = 32
else # 64 bit OS
OS_BITS = 64
endif

ifeq ($(shell uname -s), AIX) # OS-AIX
CPP = xlC
CPP_FLAGS = -g -q$(OS_BITS) -brtl -O2 -DAIX
else	# OS-Linux
CPP = g++
CPP_FLAGS = -g -m$(OS_BITS) -Wall -O2 -DLINUX
endif

############################################################################
CP = cp
MV = mv
RM = rm -f

