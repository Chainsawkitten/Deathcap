class elevator_moveCart {
    Hub @hub;
    Entity @self;
    Entity @elevator;
    Entity @board;
    Entity @puzzleBoard;
    Entity @realSelf;
    Entity @cage;
    float speed;
    vec3 tempPos;
    vec3 elevatorPos;
    bool moveForward;
    bool moveUpward;
    int phase;
    float uniformScale;

    elevator_moveCart(Entity @entity){
        @hub = Managers();
        @self = GetEntityByGUID(1508919163);
        @elevator = GetEntityByGUID(1511870044);
        @board = GetEntityByGUID(1511530025);
        @puzzleBoard = GetEntityByGUID(1512029307);
        @cage = GetEntityByGUID(1511870172);
        
        @realSelf = @entity;
        realSelf.SetEnabled(false, true);
        
        moveForward = true;
        moveUpward = false;
        speed = 2.0f;
        phase = 0;
        uniformScale = 1.0f;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    void ReceiveMessage(Entity @sender, int message) {
        if(message == 0) {
            phase = 1;
        }
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        switch(phase) {
            case 0:
                tempPos = self.GetWorldPosition();
                tempPos.x -= speed * deltaTime;
                self.SetWorldPosition(tempPos);
                break;

            case 1: 
                uniformScale -= (1.0f / 3.5f) * deltaTime;
                if (uniformScale < 0.0f) {
                    uniformScale = 0.0f;
                    phase = 2;
                    cage.GetSoundSource().Play();
                }

                puzzleBoard.scale = vec3(uniformScale, uniformScale, uniformScale);
                break;

            case 2:
                elevatorPos = elevator.GetWorldPosition();
                elevatorPos.y += speed * deltaTime;
                elevator.SetWorldPosition(elevatorPos);
                
                tempPos = self.GetWorldPosition();
                tempPos.y += speed * deltaTime;
                self.SetWorldPosition(tempPos);
                break;
        }
    }
    
    void MoveForward() {
        phase = 0;
    }
    
    void StopCart() {
        cage.GetSoundSource().Stop();
        phase = 3;
        SendMessage(board, 0);
    }
}