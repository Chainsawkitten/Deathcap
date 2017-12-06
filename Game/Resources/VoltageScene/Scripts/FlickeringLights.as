class FlickeringLights {
    Hub @hub;
    Entity @self;
    
    float flickTime = 1.0f;
    float flickSpeed = 10.0f;
    
    FlickeringLights(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    bool flick = false;
    float timer = 0.0f;
    
    bool first = true;
    float originalIntensity = 0.0f;
    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if(first){
        
            originalIntensity = self.GetPointLight().intensity;
            first = false;
            
        }
        if(flick){
        
            timer += deltaTime;
            if(timer > flickTime){
            
                self.GetPointLight().intensity = originalIntensity;
                timer = 0;
                flick = false;
            
            } else
                FlickLight(self, timer);
        
        }
    
    }
    
    void ReceiveMessage(Entity @sender, int signal) {
        
        flick = true;
    
    }
 
    void FlickLight(Entity @light, float t){
    
        if(sin(t * flickSpeed) < 0)
            light.GetPointLight().intensity = 0;
        else
            light.GetPointLight().intensity = originalIntensity;
    
    }
 
}
