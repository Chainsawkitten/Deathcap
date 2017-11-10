class Minecart_Puzzle_1{
    Hub @hub;
    Entity @self;
    float speed;
    bool trigger;
    float stopTime;
    float endTime;
    bool hasHitPlane;
    
    Minecart_Puzzle_1(Entity @entity){
        @hub = Managers();
        @self = @entity;
        speed = 2.0f;
        stopTime = 0.0f;
        endTime = 7.5f;
        hasHitPlane = false;
        
        trigger = false;
        RegisterUpdate();
    }
    
    //Update carts movements and send it's position to Player Script.
    void Update(float deltaTime){
        self.position.z -= speed*deltaTime;
        
        // Braking phase
        if (hasHitPlane && stopTime < endTime){
            stopTime += deltaTime;
            speed -= 0.0056f;
        }
        // Stopping phase
        else if (stopTime >= endTime && !trigger) {
            self.position.z = 4;
            speed = 0.0f;
        }
        // Start again after puzzle has been solved
        if (trigger){
            if (speed < 2.0f)
                speed += 0.00664f;
            if (speed > 2.0f){
                speed = 2.0f;
                trigger = false;
            }
        }
    }
    
    void ReceiveMessage(int signal){
        if (signal == 1)
            trigger = true;
    }
}