class VoltageRandomEvents {
    Hub @hub;
    Entity @self;
    Entity @jensLampa;

    VoltageRandomEvents(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @jensLampa = GetEntityByGUID(1511528947);
    }

    void ReceiveMessage(Entity @sender, int signal) {
        if (signal == 0) {
            SendMessage(jensLampa, 0);
        }
    }
}
