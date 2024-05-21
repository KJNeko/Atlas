
import os
 
lines = []
def list_files_recursive(path):
    for entry in os.listdir(path):
        if entry != "__pycache__" and entry != ".qtcreator" and entry != "assets":
            full_path = os.path.join(path, entry)
            if os.path.isdir(full_path):
                list_files_recursive(full_path)
            else:
                print(full_path)
                lines.append("\"" + full_path.replace('\\', '/') + ',\"\n')



 
# Specify the directory path you want to start from
f = open("atlas.pyproject", "w")
f.write("{\n\"files\": [\n")
directory_path = 'atlas'
list_files_recursive(directory_path)
f.writelines(lines)
f.write("]\n}")
f.close