class elevator_frontDoorCon {
    Hub @hub;
    Entity @doorWithX;
    Entity @doorWithService;
    Entity @puzzleBoard;
    bool openDoor;
    bool doorOpen;
    vec3 tempPosWithX;
    vec3 tempPosWithService;
    float boardPitch;
    float speed;
    float uniformScale;
    Entity @partical_1;
    Entity @partical_2;
    Entity @partical_3;
    
    float doorTime;
    float timer;
    
    elevator_frontDoorCon(Entity @entity){
        @hub = Managers();
        @doorWithX = GetEntityByGUID(1511870049);
        @doorWithService = GetEntityByGUID(1511870278);
        @puzzleBoard = GetEntityByGUID(1512029307);
        openDoor = false;
        doorOpen = false;
        speed = 0.5f;
        uniformScale = 0;
        timer = 0.0f;
        doorTime = 2.5f;
        @partical_1 = GetEntityByGUID(1512564077);
        @partical_2 = GetEntityByGUID(1512565858);
        @partical_3 = GetEntityByGUID(1512566495);

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if(openDoor == true) {
            if(!doorOpen && timer < doorTime) {
                tempPosWithX = doorWithX.GetWorldPosition();
                tempPosWithService = doorWithService.GetWorldPosition();

                tempPosWithX.z -= speed * deltaTime;
                tempPosWithService.z += speed * deltaTime;

                doorWithX.SetWorldPosition(tempPosWithX);
                doorWithService.SetWorldPosition(tempPosWithService);
                timer += deltaTime;
                doorOpen = timer >= doorTime;
            }

        }

        if(openDoor == false) {
            if(doorOpen && timer < doorTime) {
                tempPosWithX = doorWithX.GetWorldPosition();
                tempPosWithService = doorWithService.GetWorldPosition();

                tempPosWithX.z += speed * deltaTime;
                tempPosWithService.z -= speed * deltaTime;

                doorWithX.SetWorldPosition(tempPosWithX);
                doorWithService.SetWorldPosition(tempPosWithService);
                timer += deltaTime;
                doorOpen = !(timer >= doorTime);
            }
        }
    }
    
    void ReceiveMessage(Entity @sender, int signal){
        if (signal == 0)
            CloseDoor(); 
        else
            OpenDoor();
    }
    
    void OpenDoor() {
        openDoor = true;
        timer = 0.0f;
        doorWithX.GetSoundSource().Play();
        doorWithService.GetSoundSource().Play();
    }

    void CloseDoor() {
        timer = 0.0f;
        openDoor = false;
    }
}
