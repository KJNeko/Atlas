
import os
 
lines = []
def list_files_recursive(path):

    for entry in os.listdir(path):
        if entry != "__pycache__" and entry != ".qtcreator" and entry != "assets" and entry != "tools" and entry != "data" and entry != "_internal":
            full_path = os.path.join(path, entry)
            if os.path.isdir(full_path):
                list_files_recursive(full_path)
            else:
                print(full_path)
                lines.append(full_path.replace('\\', '/'))
 
# Specify the directory path you want to start from
f = open("atlas.pyproject", "w")
f.write("{\n    \"files\": [\n")
directory_path = '.'
list_files_recursive(directory_path)
arr_legnth = len(lines)
for line in lines:
    if lines.index(line) < arr_legnth-1:
        f.write(f'        "{line}",\n')
    else:
        f.write(f'        "{line}"')
f.write("\n    ]\n}")
f.close