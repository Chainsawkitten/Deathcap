class Particals2 {
    Hub @hub;
    Entity @self;

    Particals2(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        self.RotatePitch(180);
    }
}
