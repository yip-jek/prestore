############################################################################
BIN_DIR = ../bin

APP_INCL = -I../include

SHARE_DIR  = /usr/local/aibase1
LIBMM_DIR  = /usr/local/aibase2/libmm

SHARE_INCL = -I$(SHARE_DIR)/include
SHARE_LIB  = -L$(SHARE_DIR)/lib64/ora_static -laibase1

LIBMM_INCL = -I$(LIBMM_DIR)/include
LIBMM_LIB  = -L$(LIBMM_DIR)/lib64 -laibase2

ORA_LIB    = -L$(ORACLE_HOME)/lib -lclntsh

############################################################################
ifeq ($(shell uname -s), AIX) # OS-AIX
CPP = xlC
CPP_FLAGS = -g -q64 -brtl -O2 -DAIX

MQ_DIR  = /usr/mqm
MQ_INCL = -I$(MQ_DIR)/inc
MQ_LIB  = -L$(MQ_DIR)/lib64 -limqb23ia -limqs23ia -lmqm

NBASE_INCL = -I$(SHARE_DIR)/include/nbase
NBASE_LIB  = -L$(SHARE_DIR)/lib64/ora_static -lnbase

INCLS = $(APP_INCL) $(MQ_INCL) $(SHARE_INCL) $(NBASE_INCL) $(LIBMM_INCL)
LIBS  = $(MQ_LIB) $(SHARE_LIB) $(NBASE_LIB) $(LIBMM_LIB) $(ORA_LIB)
else	# OS-Linux
CPP = g++
CPP_FLAGS = -g -m64 -Wall -O2 -DLINUX

INCLS = $(APP_INCL) $(SHARE_INCL) $(LIBMM_INCL)
LIBS  = $(SHARE_LIB) $(LIBMM_LIB) $(ORA_LIB)
endif

############################################################################
CP = cp
MV = mv
RM = rm -f

