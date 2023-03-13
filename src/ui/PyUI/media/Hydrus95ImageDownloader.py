import requests
from io import BytesIO
from PIL import Image


def getF95BannerImage(id, url):
    img=Image.open(BytesIO(requests.get(url).content)).convert("RGB")
    img.save("data/images/banners/" +str(id)+".webp")

def getVNDBCoverImage(title):
    con = sl.connect(dbName)
    cursor = con.cursor()
    sql = """SELECT F95DB.id, Games.shortname, F95DB.bannerlink FROM F95DB INNER JOIN Games on Games.shortname = F95DB.id"""
    result = cursor.execute(sql)
