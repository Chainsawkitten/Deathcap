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
        
        deathTimer = 0.0f;
    }
    
    // Update the camera's rotation based on mouse movement.
    void MouseUpdate(){
        if(Cursor.x == 0 && Cursor.y == 0)
            Cursor = GetCursorXY();

        float horizontal = radians(0.3f * (Cursor.x - GetCursorXY().x));
        float vertical = radians(0.3f * (Cursor.y - GetCursorXY().y));
        self.RotateAroundWorldAxis(horizontal, vec3(0.0f, 1.0f, 0.0f));
        self.RotatePitch(vertical);
        
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