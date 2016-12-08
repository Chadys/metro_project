# coding: utf-8
import re

"""script permettant de changer la  structure du fichier du métro ("N01 = Meguro" devient "Meguro:N01")
ainsi que de renommer les correspondances par leur index dans la liste des stations"""

with open("../tokyo.metro", 'r+') as source:
    data=source.read()
    lines=[ligne.split()[0] for ligne in data.split('\n')[10:] if ligne]
    # Une autre façon d'écrire la ligne du dessus est la suivante, mais elle est moins concise
    # et plus lente. La complexité redevient égale en enlevant le filter, mais on risque alors 
    # d'avoir un string vide en dernier élément
    # lines=filter(lambda x: x, map(lambda x: x.split()[0] if x else '', data.split('\n')[10:]))
    data = re.sub(r'(\w{3} )= ([A-Za-z\-]+) ((?:\w{3} )*)(\w{3})',
    	lambda match: match.group(2)+':'+match.group(1)+match.group(3)+str(lines.index(match.group(4))),
    	data)
    while True:
    	data, nb_sub = re.subn(r'([A-Za-z\-]+:\w{3} )((?:\w+ )*)([a-zA-Z]\w+)',
    	lambda match: match.group(1)+match.group(2)+str(lines.index(match.group(3))),
    	data)
    	if not nb_sub: break
    source.seek(0, 0)
    source.truncate()
    source.write(data)


# Exemple :
# Avant

# N19 = Akabane-iwabuchi N18
# C06 = Akasaka C07/M14 C05
# G05 = Akasaka-mitsuke G04 G06 N07 Y16 Z04 M13
# M13 = Akasaka-mitsuke M12 N07 Y16 Z04 M14 G05
# H15 = Akihabara H14 H16
# G04 = Aoyama-itchome G02 G05 G03 Z03
# Z03 = Aoyama-itchome Z02 Z04 G04

# Après :

# Akabane-iwabuchi:N19 142
# Akasaka:C06 72/M14 C05
# Akasaka-mitsuke:G05 5 158 96 97 98 3
# Akasaka-mitsuke:M13 176 96 97 98 73 2
# Akihabara:H15 70 100
# Aoyama-itchome:G04 123 2 18 6
# Aoyama-itchome:Z03 124 98 5