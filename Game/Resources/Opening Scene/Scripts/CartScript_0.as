class CartScript_0 {
    Hub @hub;
    Entity @self;
    float speed;
    vec3 tempPos;

    CartScript_0(Entity @entity){
        @hub = Managers();
        @self = GetEntityByGUID(1508919384);
        
        speed = 2.0f;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        tempPos = self.GetWorldPosition();
        tempPos.x -= speed * deltaTime;
        self.SetWorldPosition(tempPos);
    }
}
