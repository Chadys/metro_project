# coding: utf-8
import re

# script permettant de changer la  structure du fichier du métro ("N01 = Meguro" devient "Meguro:N01")
# ainsi que de renommer les correspondances par leur index dans la liste des stations

with open("../tokyo.metro2", 'r+') as source:
    data=source.read()
    lines=[ligne.split()[0] for ligne in data.split('\n')[10:] if ligne]
    # Une autre façon d'écrire la ligne du dessus est la suivante, mais elle est moins concise
    # et plus lente. La complexité redevient égale en enlevant le filter, mais on risque alors 
    # d'avoir un string vide en dernier élément
    # lines=filter(lambda x: x, map(lambda x: x.split()[0] if x else '', data.split('\n')[10:]))
    data = re.sub(r'([\w/]{2,} )= ([A-Za-z\-]+) ((?:[\w/]{2,} )*)([\w/]{2,})',
    	lambda match: match.group(2)+':'+match.group(1)+match.group(3)+str(lines.index(match.group(4))),
    	data)
    while True:
    	data, nb_sub = re.subn(r'([A-Za-z\-]+:[\w/]{2,} )((?:[\w/]+ )*)([a-zA-Z][\w/]+)',
    	lambda match: match.group(1)+match.group(2)+str(lines.index(match.group(3))),
    	data)
    	if not nb_sub: break
    source.seek(0, 0)
    source.truncate()
    source.write(data)


# Exemple :
# Avant

# N19 = Akabane-Iwabuchi N18
# C06 = Akasaka C07/M14 C05
# G05/M13 = Akasaka-mitsuke G04/Z03 M12/N08 G06/N06 N07/Y16/Z04 C07/M14
# H15 = Akihabara H14 H16
# G04/Z03 = Aoyama-itchome C04/G02/Z02 N07/Y16/Z04 G05/M13 G03

# Après :

# Akabane-Iwabuchi:N19 105
# Akasaka:C06 51 86
# Akasaka-mitsuke:G05/M13 4 135 120 69 51
# Akihabara:H15 49 71
# Aoyama-itchome:G04/Z03 92 69 2 13