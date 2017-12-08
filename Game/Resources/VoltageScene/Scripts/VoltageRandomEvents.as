class VoltageRandomEvents {
    Hub @hub;
    Entity @self;

    VoltageRandomEvents(Entity @entity){
        @hub = Managers();
        @self = @entity;
    }

    void ReceiveMessage(Entity @sender, int signal) {
        if (signal == 0) {
            print("Invoke some random event here\n");
        }
    }
}
