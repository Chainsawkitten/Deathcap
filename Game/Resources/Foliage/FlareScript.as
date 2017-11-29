class FlareScript {
    Hub @hub;
    Entity @self;
    float time;
    float intensity;
    
    FlareScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
        
        intensity = self.GetPointLight().intensity;
        time = 0;
        
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        auto light = self.GetPointLight();
        time += deltaTime*4;
        light.intensity = intensity + ((sin(2*time/3.5) + sin((3.5*time)/3) + sin(2*time/5))/3)*50;
    }
}
