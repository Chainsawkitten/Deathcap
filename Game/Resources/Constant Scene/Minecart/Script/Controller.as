class Controller {
    Hub @hub;
    Entity @self;
    Entity @rock;
    bool isPressed;

    Controller(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @rock = GetEntityByGUID(1510240479);
        isPressed = false;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (Input(Trigger) && !isPressed) {
            print("Grip pressed" + self.GetUniqueIdentifier() + "\n");
            SendMessage(rock, 2);
            isPressed = true;
        } else if (!Input(Trigger) && isPressed) {
            print("Grip unpressed\n");
            SendMessage(rock, 1);
            isPressed = false;
        }
    }
}
