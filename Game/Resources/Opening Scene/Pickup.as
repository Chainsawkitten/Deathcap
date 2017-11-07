class Pickup {
    Hub @hub;
    Entity @self;
    Entity @lastParent;
    Entity @originalParent;

    Pickup(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @originalParent = self.GetParent();

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
    }
    
    void ReceiveMessage(Entity @sender, int i)
    {
        if(i == 1 && @originalParent != self.GetParent()){
                @lastParent = self.SetParent(@originalParent);
        }
        else if(i == 2 && @originalParent == self.GetParent()){
                self.position = vec3(0.0f,0.0f,0.0f);
                @originalParent = self.SetParent(@sender);
        }
    }
}
