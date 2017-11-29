class FlickeringLights {
    Hub @hub;
    Entity @self;
    Entity @light;
    
    float flickTime;
    
    FlickeringLights(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    bool flick = false;
    float timer = 0;
    
    bool first = true;
    float originalIntensity;
    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if(first){
        
            originalIntensity = light.GetPointLight().intensity;
        
        }
        if(flick){
        
            timer += deltaTime;
            if(timer > flickTime){
            
                light.GetPointLight().intensity = originalIntensity;
                timer = 0;
                flick = false;
            
            } else
                FlickLight(light, timer);
        
        }
    
    }
    
    void ReceiveMessage(Entity @sender, int signal) {
    
        flick = true;
    
    }
 
    void FlickLight(Entity @light, float t){
    
        if(sin(t) < 0)
            light.GetPointLight().intensity = 0;
        else
            light.GetPointLight().intensity = originalIntensity;
    
    }
 
}
