class MinecartLightScript {
    Hub @hub;
    Entity @self;

    MinecartLightScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        if(!IsVRActive())
            self.GetPointLight().intensity = 100.f;
    }
}
