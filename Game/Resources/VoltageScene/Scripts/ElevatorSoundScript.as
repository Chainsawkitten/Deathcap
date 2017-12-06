class ElevatorSoundScript {
    Hub @hub;
    Entity @self;

    ElevatorSoundScript(Entity @entity) {
        @hub = Managers();
        @self = @entity;
    }

    void ReceiveMessage(Entity @sender, int signal) {
        self.GetSoundSource().Play();
    }
}
