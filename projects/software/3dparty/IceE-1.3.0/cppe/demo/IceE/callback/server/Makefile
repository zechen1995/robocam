# **********************************************************************
#
# Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
#
# This copy of Ice-E is licensed to you under the terms described in the
# ICEE_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	= ../../../..

SERVER		= server

TARGETS		= $(SERVER)

OBJS		= Callback.o \
		  CallbackSenderI.o \
		  Server.o

SRCS		= $(OBJS:.o=.cpp)

SLICE_SRCS	= Callback.ice

include $(top_srcdir)/config/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

$(SERVER): $(OBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

include .depend
