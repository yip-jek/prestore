############################################################################
BIN_DIR = ../bin

APP_INCL = -I../include

############################################################################
ifeq ($(shell uname -m), i686) # 32 bit OS
OS_BITS = 32
else # 64 bit OS
OS_BITS = 64
endif

ifeq ($(shell uname -s), AIX) # OS-AIX
CPP = xlC
CPP_FLAGS = -g -q$(OS_BITS) -brtl -O2 -DAIX

MQ_DIR  = /usr/mqm
MQ_INCL = -I$(MQ_DIR)/inc
MQ_LIB  = -L$(MQ_DIR)/lib64 -limqb23ia -limqs23ia -lmqm

SHARE_DIR  = /usr/local/aibase1
SHARE_INCL = -I$(SHARE_DIR)/include/nbase
SHARE_LIB  = -L$(SHARE_DIR)/lib64/ora_static -lnbase

INCLS = $(APP_INCL) $(MQ_INCL) $(SHARE_INCL)
LIBS  = $(MQ_LIB) $(SHARE_LIB)
else	# OS-Linux
CPP = g++
CPP_FLAGS = -g -m$(OS_BITS) -Wall -O2 -DLINUX

INCLS = $(APP_INCL)
LIBS  = 
endif

############################################################################
CP = cp
MV = mv
RM = rm -f

