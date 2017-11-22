class CartScript_0 {
    Hub @hub;
    Entity @self;
    float speed;

    CartScript_0(Entity @entity){
        @hub = Managers();
        @self = GetEntityByGUID(1508919384);
        
        speed = 4.0f;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        self.position.z -= speed * deltaTime;
    }
}
