class ElevatorSoundScript {
    Hub @hub;
    Entity @self;

    ElevatorSoundScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }
    
    void ReceiveMessage(Entity @sender, int signal) {
    
        self.GetSoundSource().Play();
    
    }

}
