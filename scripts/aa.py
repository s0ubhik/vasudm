import re

def parse_function_defs(filename):
  with open(filename, 'r') as f:
    contents = f.read()

  function_defs = re.findall(r'(?<=^)(?P<return_type>\w+)\s+(?P<name>\w+)\s*\((?P<params>.+?)\)', contents, flags=re.M)

  parsed_defs = []
  for def_heading in function_defs:
    return_type, name, params_str = def_heading

    params = params_str.split(',')

    cleaned_params = [param.strip().split(' ')[-1] for param in params]

    parsed_defs.append({
      'return_type': return_type,
      'name': name,
      'params': cleaned_params
    })

  return parsed_defs

if __name__ == '__main__':
  filename = 'include/main.h'  # Replace with the actual file path
  parsed_defs = parse_function_defs(filename)

  # Access parsed information using dictionary keys
  for def_info in parsed_defs:
    print(f"{def_info['return_type']} {def_info['name']}({', '.join(def_info['params'])})")
