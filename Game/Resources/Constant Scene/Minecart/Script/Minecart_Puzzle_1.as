class Minecart_Puzzle_1{
    Hub @hub;
    Entity @self;
    float speed;
    bool hasHitPlane;
    float planePos;
    float stopTime;
    float endTime;
    float accel;
    bool puzzleSolved;
    
    Minecart_Puzzle_1(Entity @entity){
        @hub = Managers();
        @self = @entity;
        speed = 4.0f;
        stopTime = 0.0f;
        endTime = 5.0f;
        planePos = 0.0f;
        puzzleSolved = false;
        accel = 0.0f;
        //trigger = false;
        hasHitPlane = false;
        RegisterUpdate();
    }
    
    //Update carts movements and send it's position to Player Script.
    void Update(float deltaTime) {
        if (!hasHitPlane)
            self.position.z -= speed*deltaTime;
        // Braking phase
        else if (hasHitPlane && stopTime < endTime){
            stopTime += deltaTime;
            self.position.z = planePos + speed * stopTime + ((accel * stopTime * stopTime) / 2);
        }
        // Stopping phase
        else
            speed = 0.0f;
        // Start again after puzzle has been solved
        if (puzzleSolved){
            if (speed < 4.0f)
                speed += 0.664f * deltaTime;
            else if (speed > 4.0f){
                speed = 4.0f;
            }
            self.position.z -= speed * deltaTime;
        }
    }
    
    void OnTrigger(){
        hasHitPlane = true;
        planePos = self.position.z;
        accel = 2 * ((self.position.z - 5) - self.position.z - speed * endTime) / (endTime * endTime);
    }
    
    void ReceiveMessage(Entity @sender, int signal){
        if (signal == 3) {
            puzzleSolved = true;
        }
    }
}