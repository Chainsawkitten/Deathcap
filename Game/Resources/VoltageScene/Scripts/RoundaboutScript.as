class RoundaboutScript {
    Hub @hub;
    Entity @self;
    float correctAngle = 54.9f;
    float rotateSpeed;
    
    RoundaboutScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    float currentAngle = 0;
    
    bool rotate = false;
    float rotateAngle;
    float amountRotated;
    float direction;
    // Called by the engine for each frame.
    void Update(float deltaTime) {

        if(rotate){
        
            float rotation = direction * 0.1f * 3.141592f * deltaTime;
            self.RotateAroundWorldAxis(rotation, vec3(0, 1, 0));
            float worldYaw = abs(yaw(self.GetWorldOrientation()));
            if (worldYaw >= radians(correctAngle)) {
                self.SetWorldOrientation(quat(sin(radians(-correctAngle / 2)), 0, 1.0f * cos(radians(-correctAngle / 2)), 0));
                rotate = false;
            }
        }
    
    }
    
    void ReceiveMessage(Entity @sender, int signal) {
        
        if(!rotate)
            switch(signal){
            
                case 0:{ //Rotate right
                    direction = 1;
                    rotateAngle = 50;
                    rotate = true;
                    break;
                }
                case 1:{ //Rotate left
                    direction = -1;
                    rotateAngle = 75;
                    rotate = true;
                    break;
                }
                case 2: { //Rotate to correct angle
                    direction = 1;
                    rotate = true;
                    break;
                }
            
            }
        
    }
    
}
