class Controller {
    Hub @hub;
    Entity @self;
    Entity @rock;
    bool isPressed;

    Controller(Entity @entity){
        @hub = Managers();
        @self = @entity;
        isPressed = false;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (Input(Trigger) && !isPressed) {
          
        } else if (!Input(Trigger) && isPressed) {
            
        }
    }
}
