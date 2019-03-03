import re

pattern1 = r"\bdo\B"
pattern2 = 'bird'
string = 'dddog runs d dof cat'

m = re.search(pattern1, string)
if m is not None:
    print m.group(), 'index=',m.start()
else:
    print None

# print re.search(pattern2, string)

print(re.split(r"[,;\.]", "a;b,c.d;e"))

print(re.search(r"\bruns\b", "dog runs to cat")).group()    # <_sre.SRE_Match object; span=(4, 8), match='runs'>
# \B : empty string (but not at the start or end of a word)
print(re.search(r"\B runs \B", "dog   runs  to cat")).group()  # <_sre.SRE_Match object; span=(8, 14), match=' runs '>
# \\ : match \