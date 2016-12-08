# coding: utf-8
import re

"""script permettant de séparer les stations qui sont sur plusieurs lignes
en une station pour chaque ligne tout en gardant les correspondances adaptées"""

def get_two(match):
    corr = match.group(3).split()
    string = ''
    stations = match.group(1).split('/')
    for station in stations:
        string+=station+match.group(2)
        for correspondances in match.group(3).split():
            correspondance = correspondances.split('/')
            for corr in correspondance:
                if corr[0] == station[0]:
                    string+=' '+corr
                elif not any(c[0] in [s[0] for s in stations] for c in correspondance):
                    string+=' '+corr
        for s in stations:
            if s != station:
                string +=' '+s
        string+='\n'
    return string
                    

with open("../tokyo.metro", 'r+') as source:
    data=source.read()
    data = re.sub(r'([\w/]{3,})( = [A-Za-z\-]+)((?: [\w/]{3,})*)\n',get_two,data)
    
    source.seek(0, 0)
    source.truncate()
    source.write(data)
    
# Exemple :
# Avant :
# N19 = Akabane-iwabuchi N18
# C06 = Akasaka C07/M14 C05
# G05/M13 = Akasaka-mitsuke G04/Z03 M12/N08 G06/N06 N07/Y16/Z04 C07/M14
# H15 = Akihabara H14 H16
# G04/Z03 = Aoyama-itchome C04/G02/Z02 N07/Y16/Z04 G05/M13 G03
    
# Après :

# N19 = Akabane-iwabuchi N18
# C06 = Akasaka C07/M14 C05
# G05 = Akasaka-mitsuke G04 G06 N07 Y16 Z04 M13
# M13 = Akasaka-mitsuke M12 N07 Y16 Z04 M14 G05
# H15 = Akihabara H14 H16
# G04 = Aoyama-itchome G02 G05 G03 Z03
# Z03 = Aoyama-itchome Z02 Z04 G04