from atlas.core.database.record.gamedata import*
# This is not a static class. You have to create an instance
class Game():
    def __init__(self, game_id):
        self.game_id = game_id

    def import_record(title, creator, engine):
        id = recordID( title, creator, engine )
        if id != 0:
            #not implementd yet
            return Game(id)

        data = GameData(title, creator, engine)
        # Assuming m_game_id gets set to a valid ID before this point
        return Game(data.m_game_id)
    
    def record_exists( title, creator, engine):
	    return recordID( title, creator, engine )
    
