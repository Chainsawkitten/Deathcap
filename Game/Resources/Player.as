class Player{
    Hub @hub;
    Entity @self;
    vec2 Cursor;
    Player(Entity @entity){
        @hub = Managers();
        @self = @entity;
        RegisterUpdate();
        Cursor = vec2(0,0);
    }
    
    void Update(float i){
    
    }
}