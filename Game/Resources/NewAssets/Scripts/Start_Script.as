class Start_Script {
    Hub @hub;
    Entity @self;
    Entity @cart;

    Start_Script(Entity @entity) {
        @hub = Managers();
        @self = @entity;
        @cart = GetEntityByGUID(1508919384);

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }
    
    void OnTrigger() {
        if (Input(Trigger))
            SendMessage(cart, 6);
    }
}