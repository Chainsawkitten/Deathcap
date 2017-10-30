class Player{
    Hub @hub;
    Entity @self;
    Entity @cart;
    vec2 Cursor;
    vec2 Rotation;
    Player(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @cart = self.GetParent();
        RegisterUpdate();
        Cursor = vec2(0,0);
        Rotation = vec2(0,0);
        
        RegisterUpdate();
    }
    
    /*void MouseUpdate(){
        //self.position = vec3(cart.position.x, cart.position.y+4, cart.position.z+1);
        if(Cursor.x == 0 && Cursor.y == 0)
            Cursor = GetCursorXY();
        Rotation.x += radians(0.3f * (GetCursorXY().x - Cursor.x));
        Rotation.y += radians(0.3f * (GetCursorXY().y - Cursor.y));
        
        SetLocalRotation(vec3(Rotation.x, Rotation.y, 0.0f));
        
        Cursor = GetCursorXY();
    }*/
    
    void Update(float i){
        //self.position = vec3(cart.position.x, cart.position.y+4, cart.position.z+1);
        if(Cursor.x == 0 && Cursor.y == 0)
            Cursor = GetCursorXY();
        Rotation.x = radians(0.3f * (Cursor.x - GetCursorXY().x));
        Rotation.y = radians(0.3f * (Cursor.y - GetCursorXY().y));

        self.RotateAroundWorldAxis(Rotation.x, vec3(0.0f, 1.0f, 0.0f));
        self.RotatePitch(Rotation.y);
        
        Cursor = GetCursorXY();
    }
}