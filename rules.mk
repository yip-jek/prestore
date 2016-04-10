############################################################################
BIN_DIR = ../bin

APP_INCL = -I../include


ifeq ($(shell uname -s), AIX) # OS-AIX
SHARE_DIR  = /usr/local/aibase1
else	# OS-Linux
SHARE_DIR  = ../aibase1
endif

SHARE_INCL = -I$(SHARE_DIR)/include
SHARE_LIB  = -L$(SHARE_DIR)/lib64/ora_static -laibase1

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

NBASE_INCL = -I$(SHARE_DIR)/include/nbase
NBASE_LIB  = -L$(SHARE_DIR)/lib64/ora_static -lnbase

INCLS = $(APP_INCL) $(MQ_INCL) $(SHARE_INCL) $(NBASE_INCL)
LIBS  = $(MQ_LIB) $(SHARE_LIB) $(NBASE_LIB)
else	# OS-Linux
CPP = g++
CPP_FLAGS = -g -m$(OS_BITS) -Wall -O2 -DLINUX

INCLS = $(APP_INCL) $(SHARE_INCL)
LIBS  = $(SHARE_LIB)
endif

############################################################################
CP = cp
MV = mv
RM = rm -f

