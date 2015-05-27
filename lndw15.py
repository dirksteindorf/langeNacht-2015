#!/usr/bin/python2.7

import subprocess
import re
import time
import os
import sys

cmd = "wmctrl"
name = "Default - rqt"

def getWindows(name):
  windows = []
  result = subprocess.check_output(cmd+" -l" , shell=True)
  lines = result.split("\n")
  for line in lines:
    match = re.search("(.*) 0 viz "+name, line)
    if(match):
      windows += [match.group(1)]
  return windows

if __name__ == "__main__":
  try:
    sleepTime = int(sys.argv[1])
  except:
    sleepTime = 300
  windows = getWindows(name)
  i = 0
  while(True):
    os.system(cmd+" -i -R "+windows[i])
    i+=1
    i%=len(windows)
    time.sleep(sleepTime)
