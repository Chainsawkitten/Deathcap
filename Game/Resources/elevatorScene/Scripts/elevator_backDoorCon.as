class elevator_backDoorCon {
    Hub @hub;
    Entity @doorWithX;
    Entity @doorWithService;
    bool openDoor;
    bool openingDoor;
    vec3 tempPosWithX;
    vec3 tempPosWithService;
    float speed;

    elevator_backDoorCon(Entity @entity) {
        @hub = Managers();
        @doorWithX = GetEntityByGUID(1511870598);
        @doorWithService = GetEntityByGUID(1511870668);
        openDoor = false;
        openingDoor = false;
        speed = 0.5f;
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (openDoor == true) {
            if (tempPosWithX.z > -1.5f) {
                tempPosWithX = doorWithX.GetWorldPosition();
                tempPosWithService = doorWithService.GetWorldPosition();

                tempPosWithX.z += speed * deltaTime;
                tempPosWithService.z -= speed * deltaTime;

                doorWithX.SetWorldPosition(tempPosWithX);
                doorWithService.SetWorldPosition(tempPosWithService);
            }
        }

        if (openDoor == false && openingDoor == true) {
            tempPosWithX = doorWithX.GetWorldPosition();
            tempPosWithService = doorWithService.GetWorldPosition();

            tempPosWithX.z -= speed * deltaTime;
            tempPosWithService.z += speed * deltaTime;

            doorWithX.SetWorldPosition(tempPosWithX);
            doorWithService.SetWorldPosition(tempPosWithService);	
        }
    }
    
    void ReceiveMessage(Entity @sender, int signal) {
        if (signal == 0)
            CloseDoor();
        else
            OpenDoor();
    }

    void OpenDoor() {
        doorWithX.GetSoundSource().Play();
        doorWithService.GetSoundSource().Play();
        openDoor = true;
    }

    void CloseDoor() {
        openingDoor = true;
        openDoor = false;
    }
}
