class Player{
    Hub @hub;
    Entity @self;
    vec2 Cursor;
    vec2 Rotation;
    float deathTimer;
    
    Player(Entity @entity){
        @hub = Managers();
        @self = @entity;
        RegisterUpdate();
        Cursor = vec2(0,0);
        Rotation = vec2(0,0);
        
        deathTimer = 0.0f;
    }
    
    // Update the camera's rotation based on mouse movement.
    void MouseUpdate(){
        if(Cursor.x == 0 && Cursor.y == 0)
            Cursor = GetCursorXY();
        Rotation.x = radians(0.3f * (Cursor.x - GetCursorXY().x));
        Rotation.y = radians(0.3f * (Cursor.y - GetCursorXY().y));
        
        self.RotateAroundWorldAxis(Rotation.x, vec3(0.0f, 1.0f, 0.0f));
        self.RotatePitch(Rotation.y);
        
        Cursor = GetCursorXY();
    }
    
    void Update(float deltaTime){
        // Only control camera with mouse if we're not running in VR.
        if (!IsVRActive())
            MouseUpdate();
            
        deathTimer += 0.001;
        if(deathTimer > 10)
            RestartScene();
    }
}