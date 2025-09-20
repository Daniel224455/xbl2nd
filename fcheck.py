#!/usr/bin/env python

#============================================================================
#
# @file fcheck.py
#
# GENERAL DESCRIPTION
#   Check for files, folder getting added and fail build
#
# Copyright 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.
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
# 01/18/19   vk      Initial revision

from optparse import OptionParser
import os.path 

def main():
    file_path = []
    path=[]
    cwd = os.getcwd()
    print (cwd) 

    log_file="fcheck.log"

    usage = "usage: %prog [options] arg"
    parser = OptionParser(usage)
    parser.add_option("-n", "--name", dest="filename",
                      help="Check if filename exists")

    (options, args) = parser.parse_args()

    if len (args) != 1:
      parser.error("Missing arguments")

    if os.path.exists(log_file):
      print ("Remove existing log file ...")  
      try:
          os.remove(log_file)
      except OSError as e:
          print "Error deleting %s" % log_file
          exit(1)        
          print "!!! Exit !!!"

    for filen in options.filename.split(","):
      #print(filen)
      path=[]
      path = cwd
      path = os.path.join(path, filen)
      result  = os.path.exists(path)
      if result is None:
        print "%s error checking" % path
      elif result:  
        print "Found    :%s" % path
        print "!!! Exit !!!"
        exit(2)
      else: 
        print "Not Found:%s" % path
    
    file=open(log_file, "w")
    file.write("fcheck.py complete, success.")
    file.close
    exit(0)
    
if __name__ == "__main__":
    main()
