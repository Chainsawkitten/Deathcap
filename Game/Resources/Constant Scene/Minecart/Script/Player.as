class Player{
    Hub @hub;
    Entity @self;
    vec2 Cursor;
    float deathTimer;
    
    Player(Entity @entity){
        @hub = Managers();
        @self = @entity;
        RegisterUpdate();
        Cursor = vec2(0,0);
    }
    
    // Update the camera's rotation based on mouse movement.
    void MouseUpdate(){
        if(Cursor.x == 0 && Cursor.y == 0)
            Cursor = GetCursorXY();
        self.rotation.x -= 0.3f * (GetCursorXY().x - Cursor.x);
        self.rotation.y -= 0.3f * (GetCursorXY().y - Cursor.y);
        
        Cursor = GetCursorXY();
    }
    
    void Update(float deltaTime){
        // Only control camera with mouse if we're not running in VR.
        if (!IsVRActive())
            MouseUpdate();
            
		deathTimer += i;
        if(deathTimer > 10)
        	RestartScene();
    }
}