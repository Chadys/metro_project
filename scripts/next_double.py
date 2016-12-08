# coding: utf-8
import re, fusion

"""script utile pour appeler la fonction de fusion sur chaque station de métro présent en double"""

with open("../tokyo.metro", 'r') as source:
	data=source.readlines()
	station=()
	for line in data:
		match=re.search(r'([\w/]{2,}) = ([A-Za-z\-]+)', line)
		if match is not None:
			stationbis = match.group(1,2)
			if station and station[1] == stationbis[1]:
				print station[1]
				print station[0]+' '+stationbis[0]
				fusion.fusion([station[0],stationbis[0]])
				station = (station[0]+'/'+stationbis[0], station[1])
				continue
			station=stationbis
	print "No more double station"

# Exemple : python next_double.py 
# Otemachi
# C11 M18
# Otemachi
# C11/M18 T09
# Otemachi
# C11/M18/T09 Z08
# Senkawa
# F07 Y07
# Shibuya
# F16 Z01
# Shikatetsu-akatsuka
# F03 Y03
# Shinjuku
# F13 M08
# Takebashi
# T08 Z05
# Tameike-sanno
# G06 N06
# Ueno
# G16 H17
# Wakoshi
# F01 Y01
# Yotsuya
# M12 N08
# No more double station