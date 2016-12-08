# coding: utf-8
import re

"""script qui m'a servi à trier les stations par ordre alphabétique"""

def key(a):
    """opérateur de comparaison personnel, qui permet de trier les stations selon leur nom,
    sans compter leur code de ligne ('G06 = ', 'M15 = ', ...)"""
    
    regex=re.compile(r'[\w/]{2,} = ([A-Za-z\-]+)')
    match=re.search(regex,a)
    if match:
        return match.group(1)
    return ''

with open("../tokyo.metro", 'r+') as source:
    data=source.readlines()
    data.sort(key=key)
    source.seek(0, 0)
    source.truncate()
    source.writelines(data)