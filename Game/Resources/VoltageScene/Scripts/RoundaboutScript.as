class RoundaboutScript {
    Hub @hub;
    Entity @self;
    float correctAngle;
    float rotateSpeed;
    
    RoundaboutScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    bool rotate = false;
    float rotateAngle;
    float amountRotated;
    float direction;
    // Called by the engine for each frame.
    void Update(float deltaTime) {

        if(rotate){
        
            self.rotation.y += direction * rotateSpeed * deltaTime;            
            if(amountRotated + rotateSpeed * deltaTime > rotateAngle){
            
                self.rotation += direction * (rotateAngle - amountRotated);
                rotate = false;
                
            } else            
                amountRotated += rotateSpeed * deltaTime;
            
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
                case 2:{ //Rotate to correct angle
                    
                    rotateAngle = correctAngle - self.rotation.y;
                    direction = 1;
                    rotate = true;
                    
                    if(rotateAngle < 0){
                        rotateAngle *= -1;
                        direction = -1;
                    }
                    
                    break;
                }
            
            }
        
    }
    
}
