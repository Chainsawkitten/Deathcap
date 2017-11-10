class Minecart_Puzzle_1{
    Hub @hub;
    Entity @self;
    float speed;
    //bool trigger;
    bool startAgain;
    float stopTime;
    float endTime;
    
    Minecart_Puzzle_1(Entity @entity){
        @hub = Managers();
        @self = @entity;
        speed = 4.0f;
        stopTime = 0.0f;
        endTime = 7.5f;
        
        //trigger = false;
        startAgain = false;
        RegisterUpdate();
    }
    
    //Update carts movements and send it's position to Player Script.
    void Update(float deltaTime) {
        self.position.z -= speed*deltaTime;
        
        if (self.GetWorldPosition().x <= 5.4 && startAgain == false)
            speed = 0.0f;
        else if (startAgain && speed < 4.0f)
            speed += 0.00664f;
        else if (speed >= 4.0f) {
            speed == 4.0f;
        }
        
        /*// Braking phase
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
        }*/
    }
    
    void ReceiveMessage(Entity @sender, int signal){
        /*if (signal == 1 && trigger == false)
            trigger = true;*/
        if (signal == 3) {
            startAgain = true;
        }
    }
}