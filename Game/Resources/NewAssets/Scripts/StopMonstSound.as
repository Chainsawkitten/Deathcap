class StopMonstSound {
    Hub @hub;
    Entity @self;
    Entity @monsterSound;
    
    StopMonstSound(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @monsterSound = GetEntityByGUID(1512561652);

    }
    
    void StopMonsterSound(){
        monsterSound.GetSoundSource().Stop();
    }
}
