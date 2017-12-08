class Particals {
    Hub @hub;
    Entity @self;

    Particals(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        self.RotatePitch(5);
    }
}
