## @file
# GNU/Linux makefile for 'GenSec' module build.
#
# Copyright (c) 2011,2013,2018 Qualcomm Technologies Inc. All rights reserved.
# Copyright (c) 2007 - 2018, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
MAKEROOT ?= ..

APPNAME = GenSec

OBJECTS = GenSec.o

include $(MAKEROOT)/Makefiles/app.makefile

LIBS = -lCommon
#
#QCOM_EDK2_PATCH Start
#
#ifeq ($(CYGWIN), CYGWIN)
#  LIBS += -L/lib/e2fsprogs -luuid
#endif
#
#ifeq ($(LINUX), Linux)
#  LIBS += -luuid
#endif
#
#QCOM_EDK2_PATCH End
#
