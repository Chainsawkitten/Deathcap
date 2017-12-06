class monsterSoundScript {
    Hub @hub;
    Entity @self;

    monsterSoundScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
        self.GetSoundSource().Play();
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }
}
