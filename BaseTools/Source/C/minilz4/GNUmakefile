## @file
# GNU/Linux makefile for 'minilz4' module build.
#
# Copyright (c) 2018, Qualcomm Technologies Inc. All rights reserved.<BR>
#
# Copyright (c) 2009 - 2012, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

ARCH ?= IA32
MAKEROOT ?= ..

APPNAME = minilz4

SDK_C = Sdk/C

OBJECTS = \
  minilz4.o \
  $(SDK_C)/lz4.o \
  $(SDK_C)/lz4hc.o

include $(MAKEROOT)/Makefiles/app.makefile

