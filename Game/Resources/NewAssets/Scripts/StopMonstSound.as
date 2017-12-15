class StopMonstSound {
    Hub @hub;
    Entity @self;
    Entity @monsterSound;
    
    StopMonstSound(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @monsterSound = GetEntityByGUID(1512561652);

    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }
    
    void StopMonsterSound(){
        monsterSound.GetSoundSource().Stop();
    }
}
