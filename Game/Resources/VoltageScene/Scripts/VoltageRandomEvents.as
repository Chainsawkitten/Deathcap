class VoltageRandomEvents {
    Hub @hub;
    Entity @self;

    VoltageRandomEvents(Entity @entity){
        @hub = Managers();
        @self = @entity;
    }

    void ReceiveMessage(Entity @sender, int signal) {
    }
}
