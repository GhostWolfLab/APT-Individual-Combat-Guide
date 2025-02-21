#!/usr/bin/env python
import os
import sys
try:
	os.system('rm -rf /tmp/log/access.log')
except:
	sys.exit()
