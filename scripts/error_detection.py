# coding: utf-8
import re, ipdb

"""script qui permet de vérifier que toutes les correspondance de chaque stations existe bien
et a bien une corespondance dans le sens inverse"""

error = 0
with open("../tokyo.metro", 'r+') as source:
	stations = [line.split() for line in source.readlines()[10:]]
	for station in stations:
		for correspondance in station[3:]:
			for s in stations:
				if correspondance == s[0]:
					if station[0] not in s:
						error+=1
						print "La station "+station[2]+" ("+station[0]+") n'a pas un lien bilatéral avec "+s[2]+" ("+s[0]+")"
					break
			else:
				error+=1
				print "La station "+correspondance+" liée a "+station[2]+" ("+station[0]+") n'existe pas"

if error: print str(error)+" erreur detectées"
else: print "Pas d'erreur détectée"

# Exemple d'utilisation sur "tokyo_metro (second version)":

# La station m02 liée a Honancho (m03) n'existe pas
# La station Meiji-jingumae (C03/F15) n'a pas un lien bilatéral avec Shibuya (F16/Z01)
# La station Nakano-shimbashi (m05) n'a pas un lien bilatéral avec Nakano-sakaue (M06)
# La station G01 liée a Omote-sando (C04/G02/Z02) n'existe pas
# La station Shibuya liée a Shibuya (F16/Z01) n'existe pas
# La station C03/F15G01 liée a Shibuya (F16/Z01) n'existe pas
# La station = liée a Shibuya (F16/Z01) n'existe pas
# La station N19 liée a Shimo (N18) n'existe pas
# 8 erreur detectées
