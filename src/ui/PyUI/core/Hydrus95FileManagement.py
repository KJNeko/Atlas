import os

def CreateDirectory(path):
    if not os.path.exists(path):
        os.makedirs(path)
        print("The directory" + path + " has been created")

CreateDirectory("data")
CreateDirectory("data/images")
CreateDirectory("data/images/banners")
CreateDirectory("data/images/covers")
CreateDirectory("data/images/icons")
