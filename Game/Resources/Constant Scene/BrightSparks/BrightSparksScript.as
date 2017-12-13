class BrightSparksScript {
    Hub @hub;
    Entity @self;
    float age = 0;
    
    BrightSparksScript(Entity @entity) {
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        age += deltaTime;
        if (age > 0.5f) {
            age = 0.0f;
            self.GetParent().SetEnabled(false, true);
        }
    }
}
