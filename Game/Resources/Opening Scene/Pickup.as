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
        if(i == 2){
                self.position = vec3(0.0f,0.0f,0.0f);
                @parent = self.SetParent(@sender);
            }
    }
}
