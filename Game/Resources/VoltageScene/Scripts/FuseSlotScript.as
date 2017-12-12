class FuseSlotScript {
    Hub @hub;
    Entity @self;
    Entity @sparkEmitterEntity = null;

    FuseSlotScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Disable partilces.
        sparkEmitterEntity = self.GetChildFromIndex(0);
        sparkEmitterEntity.SetEnabled(false, true);
    }
}
