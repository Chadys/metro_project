# coding: utf-8
import os, sys, re

# script qui permet de fusionner les stations qui sont les mêmes mais présentes sur plusieurs lignes du métro

def fusion(stations):
	test, result = '',''
	for station in stations:
		test+=station+'|'
		result+=station+'/'
	test=test[:-1]
	result=result[:-1]

	with open("../tokyo.metro", 'r+') as source:
	    data=source.read()
	    data=re.sub(r''+test, result, data)
	    data=re.sub(result+r'( = [A-Za-z\-]+ .*?)\n'+result+r' = [A-Za-z\-]* (.*?\n)', result+r'\1'+r' \2', data)
	    source.seek(0, 0)
		source.truncate()
	    source.write(data)

# Ne fait la fusion avec des argument utilisateur que si le fichier a été appelé directement,
# et pas s'il est importé par un autre fichier python
if __name__ == "__main__":
	if len(sys.argv) < 2:
		print "Error in file use, missing stations' number"
		quit()

	fusion(sys.argv[1:])

# exemple : python fusion.py F05 Y05
# transforme :
# F05 = Hikawadai F04 F06
# Y05 = Hikawadai Y04 Y06
# en :
# F05/Y05 = Hikawadai F04 F06 Y04 Y06