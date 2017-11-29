class elevator_frontDoor {
    Hub @hub;
    Entity @self;
	bool openDoor;
	float startPos;
	float endPos;
	vec3 tempPos;
	
	

    elevator_frontDoor(Entity @entity){
        @hub = Managers();
        @self = GetEntityByGUID(1511870049);
		openDoor = false;
		startPos = 0.023;
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {		
		tempPos = self.GetWorldPosition();
		if(openDoor == true) {
			if(tempPos.z > -0.85f) {
				tempPos.z += 0.5 * deltaTime;
				self.SetWorldPosition(tempPos);
				
			}
		}
    }
	
	void OpenDoor() {
		openDoor = true;
	}
	
	void CloseDoor() {
		openDoor = false;
	}
}
