#!/usr/bin/env python

#============================================================================
#
# @file spy_buildex.py
#
# REQUIREMENTS
#   python 2.7.3+ (same as buildex.py)
#
# GENERAL DESCRIPTION
#   This script wraps buildex.py for the purpose of overriding function
#   "find_tool_dirt". This allows this script to spy on the toolchain
#   path used, and inject a different toochain path if specified by a
#   command line argument. This file is invoked exactly as you would
#   invoke buildex.py except you can optionally provide the
#   "BUILDSPY,/fake/path/" argument.
#
# NOTES
#   - should work on all platforms (tested on Windows and Linux)
#
# MAINTAINENCE
#   - update the variable CLANG_TOOL_NAME
#
# Copyright (c) 2018, Qualcomm Technologies, Inc. All rights reserved.
#
#----------------------------------------------------------------------------
#
#                          EDIT HISTORY FOR MODULE
#
# This section contains comments describing changes made to the module.
# Notice that changes are listed in reverse chronological order.
#
# when       who     what, where, why
# --------   ---     -----------------------------------------------------------
# 09/07/18   msantos written for boot.xf.3.0/3.2

import buildex
import os
import sys
import platform

# TARGET-SPECIFIC HARDCODED VARIABLES
#------------------------------------------------------------------------------
CLANG_TOOL_NAME = 'CLANG40'
#------------------------------------------------------------------------------

FAKE_TOOLCHAIN_PATH = None
REAL_TOOLCHAIN_PATH = ''
BUILDEX_find_tool_dirt = None

#####################################################################
# override for 'find_tool_dirt' that gets the real toolchain path
# and symlinks everything from the path, then overwrites the real path
# with the symlink path before invoking the real 'find_tool_dirt'
#####################################################################
def CALLFLOWGRAPH_find_tool_dirt(tools_dirt,tool_name):
  global REAL_TOOLCHAIN_PATH

  if tool_name == CLANG_TOOL_NAME:
    if platform.system() == 'Windows':
      REAL_TOOLCHAIN_PATH = tools_dirt['ToolChain'][tool_name]['Windows']['Path']
    else:
      REAL_TOOLCHAIN_PATH = tools_dirt['ToolChain'][tool_name]['Linux']['Path']

    if FAKE_TOOLCHAIN_PATH:
      tools_dirt['ToolChain'][tool_name]['Windows']['Path'] = FAKE_TOOLCHAIN_PATH
      tools_dirt['ToolChain'][tool_name]['Path'] = FAKE_TOOLCHAIN_PATH
      tools_dirt['ToolChain'][tool_name]['Linux']['Path'] = FAKE_TOOLCHAIN_PATH

  return BUILDEX_find_tool_dirt(tools_dirt,tool_name)

#####################################################################
# override the function 'find_tool_dirt' then call buildex
#####################################################################
def main():
  global FAKE_TOOLCHAIN_PATH
  global BUILDEX_find_tool_dirt

  buildspy_args = sys.argv[-1].split(',') # if this should run buildex with the override

  if buildspy_args[0] == 'BUILDSPY':
    FAKE_TOOLCHAIN_PATH = buildspy_args[1]
    # ensure path is in correct format for buildex
    if platform.system() == 'Windows':
      if FAKE_TOOLCHAIN_PATH[-1] != '\\':
        FAKE_TOOLCHAIN_PATH += '\\'
    else:
      if FAKE_TOOLCHAIN_PATH[-1] != '/':
        FAKE_TOOLCHAIN_PATH += '/'
    if not os.path.isdir(FAKE_TOOLCHAIN_PATH):
      raise Exception('got a path that does not exist: "'+FAKE_TOOLCHAIN_PATH+'"')
    # remove the buildspy args for buildex
    sys.argv = sys.argv[:-1]

  # override buildex function
  BUILDEX_find_tool_dirt = buildex.find_tool_dirt
  buildex.find_tool_dirt = CALLFLOWGRAPH_find_tool_dirt

  buildex.main() # run buildex
  print
  print
  print
  print REAL_TOOLCHAIN_PATH # to pass this info to the command line for use by callflowgraph.py

if __name__ == "__main__":
  main()
