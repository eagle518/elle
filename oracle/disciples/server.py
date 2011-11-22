#! /usr/bin/python2

#
# ---------- imports ----------------------------------------------------------
#

import sys

import application

#
# ---------- functions --------------------------------------------------------
#

def Usage():
  print 'usage: $> ' + sys.argv[0]
  sys.exit(1)

#
# ---------- main -------------------------------------------------------------
#

if __name__ == '__main__':
  if len(sys.argv) != 1:
    Usage()

  application.Run(12345)
