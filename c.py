import re, sys

with open(sys.argv[1]) as fp:
  contents = fp.read()


function_defs = re.findall(r'(?<=^)(?P<return_type>\w+)\s+(?P<name>\w+)\s*\((?P<params>.*?)\)', contents, flags=re.M | re.DOTALL)

for match in function_defs:
    name = match[1]
    ret = match[0]
    params = re.sub(r'\s+', ' ', match[2])
    out = f"{ret} {name}({params});"
    print(out)
