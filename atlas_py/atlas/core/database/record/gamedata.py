

class gamedata(object):

    def get_version(name):
        return "1"
        #dummy

    def get_record_id(title, creator, engine):
        query = f'SELECT record_id FROM games WHERE title = {title} AND creator = {creator} AND engine = {engine}'
        return query
    def record_exist(title, creator, engine):
        return True
        #dummy