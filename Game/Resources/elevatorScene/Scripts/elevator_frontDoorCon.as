class elevator_frontDoorCon {
    Hub @hub;
    Entity @doorWithX;
    Entity @doorWithService;
    Entity @puzzleBoard;
    bool openDoor;
    vec3 tempPosWithX;
    vec3 tempPosWithService;
    float boardPitch;
    float speed;
    float uniformScale;
    

    elevator_frontDoorCon(Entity @entity){
        @hub = Managers();
        @doorWithX = GetEntityByGUID(1511870049);
        @doorWithService = GetEntityByGUID(1511870278);
        @puzzleBoard = GetEntityByGUID(1512029307);
        openDoor = false;
        speed = 0.5f;
        uniformScale = 0;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if(openDoor == true) {
            if(tempPosWithX.z < 2.0f) {
                tempPosWithX = doorWithX.GetWorldPosition();
                tempPosWithService = doorWithService.GetWorldPosition();
            
                tempPosWithX.z -= speed * deltaTime;
                tempPosWithService.z += speed * deltaTime;
                
                doorWithX.SetWorldPosition(tempPosWithX);
                doorWithService.SetWorldPosition(tempPosWithService);
            }

            uniformScale += (1.0f / 3.5f) * deltaTime;
            if (uniformScale > 1.0f) {
                uniformScale = 1.0f;
            }

            puzzleBoard.scale = vec3(uniformScale, uniformScale, uniformScale);
            
        }
        
        if(openDoor == false) {
            if(tempPosWithX.z > 0.0f) {
                tempPosWithX = doorWithX.GetWorldPosition();
                tempPosWithService = doorWithService.GetWorldPosition();
            
                tempPosWithX.z += speed * deltaTime;
                tempPosWithService.z -= speed * deltaTime;
                
                doorWithX.SetWorldPosition(tempPosWithX);
                doorWithService.SetWorldPosition(tempPosWithService);
            }
        }
    }
	
	void OpenDoor() {
		openDoor = true;
        doorWithX.GetSoundSource().Play();
        doorWithService.GetSoundSource().Play();
	}
	
	void CloseDoor() {
		openDoor = false;
	}
}