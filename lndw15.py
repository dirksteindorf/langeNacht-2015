#!/usr/bin/python2.7

import subprocess
import re
import time
import os

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
  windows = getWindows(name)
  i = 0
  while(True):
    os.system(cmd+" -i -R "+windows[i])
    print "current window: "+windows[i]
    i+=1
    i%=len(windows)
    time.sleep(10)
    print "time's up"
