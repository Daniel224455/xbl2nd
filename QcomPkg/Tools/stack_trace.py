#============================================================================
#
#/** @file stack_trace.py
#
# GENERAL DESCRIPTION
#   Generate readable function name stack trace using logs from UART on crash
#
#  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
#  Qualcomm Technologies Proprietary and Confidential.
#
#**/
#
#----------------------------------------------------------------------------
#
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when       who       what, where, why
# --------   ---       ------------------------------------------------------
# 10/29/18    yg       Initial revision
#
#============================================================================
import os
import sys
import stat
import commands
import re
import string
import pdb
import re

from glob import glob

PROCESS_SKIP_TILL_UEFI_START = 1
PROCESS_DRIVER_LOADS  = 2
PROCESS_STACK_FRAME_LOGS  = 3

addr = 0

uefi_images = {} 

dist = 0
image_name = ""
image_addr = 0
matched_addr = 0
matched_name = ""

stack_frame = []
sf = []

filepath = ""

function_offset = 0
function_name = ""

# Matches the driver loading logs in UART log output
# TODO : Handle the bad logs that doesn't print a newline and the driver loading
#        log is mixed with the bad logs
drvr_load_log_re = re.compile(r'^-\s0x([0-9a-fA-F]+)\s\[\s*(\d+)\]\s(\S*)[.]efi$')

# Matches the stack frame address print output, doesn't really need to be re
stk_frm_re = re.compile(r'^([0-9a-fA-F]+)$')

# Matches the Address and symbol list in the *.map.txt file. This bring the dependency
# on the map file processing script to be in sync with the layout
map_file_symbol_re = re.compile(r'^[0-9]+\s+0x(\S+)\s+0x(\S+)\s+\S+\s+(\S+).*$')

if len(sys.argv) < 2:
   print "Usage: stack_trace.py <uart log file path>"
   print "       Log file should have UEFI Start, driver loading addresses and the"
   print "       stack frame printed\n"
   print "       Run the script from the build root folder in the matching build sources"
   print "       which has all build output objects and map files intact, or from a"
   print "       folder which has all matching *.map.txt files available under ./Build\n"
   exit ()

log_file = sys.argv[1]

fp = open(log_file)

process_state = PROCESS_SKIP_TILL_UEFI_START

while 1:
  line = fp.readline()
  if not line:
    break

  line = line.strip()

#  pdb.set_trace()

  if len(line) == 0:
    continue
  
#  Skip everything till we get to interesting point of logs
  if string.find(line.upper(), "UEFI Start".upper()) != -1:
    process_state = PROCESS_DRIVER_LOADS
    continue

#******************************************************************************
#
# Process the interesting part of the map
#
#******************************************************************************
  if process_state == PROCESS_DRIVER_LOADS:
    drvr_log_match = drvr_load_log_re.search(line)
    if drvr_log_match:
      #pdb.set_trace()

      #  Store address where its loaded
      addr = int(drvr_log_match.group(1), 16)
      #print addr

      #  Store the image/file name
      uefi_images[addr] = drvr_log_match.group(3)
      #print str(addr) + "  " + drvr_log_match.group(3)

    else:
      if string.find(line.upper(), "Stack frame:".upper()) != -1:
        process_state = PROCESS_STACK_FRAME_LOGS
        print ""
        continue


  if process_state == PROCESS_STACK_FRAME_LOGS:
    stk_frm_match = stk_frm_re.search(line)
    if stk_frm_match:
      addr = int(stk_frm_match.group(1), 16);
      dist = 0xffffffffffff   # max distance

      #pdb.set_trace()
      for img in uefi_images:
        image_addr = img
        image_name = uefi_images[img]
        if addr > image_addr:
          if (addr - image_addr) < dist:
            dist = (addr - image_addr)
            matched_addr = image_addr
            matched_name = image_name

      #print str(addr) + "  " + matched_name + "   " + str(matched_addr)

      temp_sf = []
      temp_sf.append(addr)
      temp_sf.append(matched_addr)
      temp_sf.append(matched_name)
      stack_frame.append(temp_sf)




map_files = [y for x in os.walk('./Build') for y in glob(os.path.join(x[0], '*.map.txt'))]

#print map_files

#print uefi_images

for x in stack_frame:
  #print x
  fname = x[2] + '.map.txt'
  #print fname

  for y in map_files:
    if y.find(fname) != -1:
      #print y
      filepath = y
      break

  function_offset = x[0] - x[1]
  function_offset = function_offset - 4096   # for image load offset compensation
  #print hex(function_offset)
  function_name = ""

#def get_symbol_name (offset, mapfile)
  
  map_file = open(filepath)

  while 1:
    line = map_file.readline()
    if not line:
      continue
    #print line
    line = line.strip()

    if len(line) == 0:
      continue

    symbol_match = map_file_symbol_re.search(line)
    if symbol_match:
      #print  symbol_match.group(1) + "   " + symbol_match.group(3)
      fn_addr = int(symbol_match.group(1), 16)

      if fn_addr <= function_offset:
        function_name = symbol_match.group(3)
      else:
        map_file.close()
        print ('%10x  %-32s    %-25s' %(function_offset, function_name, x[2]))
        break


print ""

