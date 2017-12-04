class PlayAudioOnStartup {
    Hub @hub;
    Entity @self;
    Component::SoundSource @sound;

    PlayAudioOnStartup(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @sound = self.GetSoundSource();
        sound.Play();
    }
}
