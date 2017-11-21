class Cart_Puzzle_1{
    Hub @hub;
    Entity @self;
    float speed;
    bool hasHitPlane;
    float planePos;
    float stopTime;
    float endTime;
    bool puzzleSolved;
    vec3 tempPos;
    // v(t) = a*t*t + b*t + c
    // s(t) = V(t)
    // s(0) = 0, s(endTime) = breakDistance
    // v(0) = speed, v(endTime) = 0
    float a;
    float b;
    float c;
    float brakeDistance;
    bool audioPlaying;
    bool startRolling;
    
    Cart_Puzzle_1(Entity @entity){
        @hub = Managers();
        @self = GetEntityByGUID(1508919384);
        speed = 2.0f;
        stopTime = 0.0f;
        endTime = 5.0f;
        planePos = 0.0f;
        puzzleSolved = false;
        //trigger = false;
        hasHitPlane = false;
        audioPlaying = false;
        startRolling = false;
        RegisterUpdate();
    }
    
    //Update carts movements and send it's position to Player Script.
    void Update(float deltaTime) {
        if (!hasHitPlane) {
            tempPos = self.GetWorldPosition();
            tempPos.x -= speed * deltaTime;
            self.SetWorldPosition(tempPos);
        }
        // Braking phase
        else if (hasHitPlane && stopTime < endTime){
            stopTime += deltaTime;
            tempPos = self.GetWorldPosition();
            float s = a * stopTime * stopTime * stopTime / 3 + b * stopTime * stopTime / 2 + c * stopTime;
            tempPos.x = planePos - s;
            self.SetWorldPosition(tempPos);
        }
        // Start again after puzzle has been solved
        else if (puzzleSolved){
            if (speed < 2.0f)
                speed += 0.5f * deltaTime;
            else if (speed > 2.0f){
                speed = 2.0f;
            }
            tempPos = self.GetWorldPosition();
            tempPos.x -= speed * deltaTime;
            self.SetWorldPosition(tempPos);
        }
        // Stopping phase
        else
            speed = 0.0f;
            
        // Play rail audio
        if(!audioPlaying && speed > 0.1f){
            self.GetSoundSource().Play();
            audioPlaying = true;
        }
        else if (speed < 0.1f){
            self.GetSoundSource().Stop();
            audioPlaying = false;
        }
        
        
    }
    
    void OnTrigger(){
        hasHitPlane = true;
        planePos = self.GetWorldPosition().x;
        brakeDistance = 10.0f; // The distance during which we will brake
        c = speed;
        b = (6 * brakeDistance - 4 * c * endTime) / (endTime * endTime);
        a = -(c + b * endTime) / (endTime * endTime);
    }
    
    void ReceiveMessage(Entity @sender, int signal){
        if (signal == 3) {
            puzzleSolved = true;
        }
    }
}