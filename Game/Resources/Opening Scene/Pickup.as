class Pickup {
    Hub @hub;
    Entity @self;
    Entity @parent;

    Pickup(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }
    
    void ReceiveMessage(Entity @sender, int i)
    {
        if(i == 2)
            @parent = self.SetParent(@sender);
    }
}
