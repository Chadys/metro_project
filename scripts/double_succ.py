# coding: utf-8
import re

# script utile pour éliminer les doubles parmis les successeurs des stations
# apparus lors de la fusion.
# La fonction éliminer les doublons en créant un set des successeurs

with open("../tokyo.metro", 'r+') as source:
	data=source.readlines()
	new=[]
	for line in data:
		match=re.search(r'([\w/]{2,} = [A-Za-z\-]+) (.*)', line)
		if match is not None:
			line = match.group(1)
			succ = match.group(2).split()
			succ_set = set(succ)
			while(succ_set):
				line+=' '+succ_set.pop()
			line+='\n'
		new.append(line)
	source.seek(0, 0)
	source.truncate()
	source.writelines(new)

# Exemple :
# Avant fusion:
# F02/Y02 = Chikatetsu-narimasu F01 F03 Y01 Y03
# Après fusion :
# F02/Y02 = Chikatetsu-narimasu F01/Y01 F03/Y03 F01/Y01 F03/Y03
# Après nettoyage des doublons :
# F02/Y02 = Chikatetsu-narimasu F01/Y01 F03/Y03
