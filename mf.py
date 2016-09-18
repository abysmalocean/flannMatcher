# Dongwei Wang
# python file process
# this program get detailed memory fetch information and calculate reuse distance for each memory fetch
# reuse distance of a memory access is the number of accesses to unique addresses made since the last reference to the requested data

#mr: memory fetch result( hit(0) miss(2) )
#mt: memory fetch type( read(0) or write(4) )
#ms: memory fetch size
#mc: memory fetch cycle
#ad: memory fetch address (long long)

import string
import os
import time
import sys
import urllib
import urllib2
import requests

# get current directory
cur_dir = os.getcwd()
print(cur_dir + '\n')
source = str(sys.argv[1])
print(source + '\n')
strprefix = str(sys.argv[2])
print(strprefix + '\n')
trace = open(source,'r')

dest_tar = str(cur_dir + '/targets/tar_list')
dest_src = str(cur_dir + '/source/src_list')
ot_tar = open(dest_tar,'w')
ot_src = open(dest_src,'w')

strcont = '<Contents>'
strkey = '<Key>'
strkey_slash = '</Key>'
strsrc = 'source'
strtar = 'targets'

strsrc_arr = []
strtar_arr = []

# loop to test each line in file

for line in trace:
    # find the line of statistics
    if line.find(strcont) != -1:
        strvar = line.split(strcont)
        for i in range(0, len(strvar)):
            strvar2 = strvar[i].split(strkey)
            for j in range(0, len(strvar2)):
                strvar3 = strvar2[j].split(strkey_slash)
                for k in range(0,len(strvar3)):
                    if strvar3[k].find(strsrc) != -1:
                        strvar4 = strvar3[k].split('/')
                        if strvar4[1] != '':
                            strsrc_arr.append(strvar3[k])
                    if strvar3[k].find(strtar) != -1:
                        strvar5 = strvar3[k].split('/')
                        if strvar5[1] != '':
                             strtar_arr.append(strvar3[k])

for i in range(0, len(strsrc_arr)):
    strsrc_arr[i] = strprefix + '/'+strsrc_arr[i]
    print(strsrc_arr[i])
    ot_src.write(strsrc_arr[i]+'\n')

for i in range(0, len(strtar_arr)):
    strtar_arr[i] = strprefix + '/'+strtar_arr[i]
    print(strtar_arr[i])
    ot_tar.write(strtar_arr[i]+'\n')

trace.close()
ot_src.close()
ot_tar.close()
