from PIL import Image 
import os 

path = os.getcwd()

for file in os.listdir(path): 
    if file.endswith(".jpg") or file.endswith(".png"): 
        img = Image.open(file)
        file_name, file_ext = os.path.splitext(file)
        img.save('{}.webp'.format(file_name))
