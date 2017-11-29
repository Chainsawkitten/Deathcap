class elevator_moveCart {
    Hub @hub;
    Entity @self;
	Entity @elevator;
    float speed;
    vec3 tempPos;
	vec3 elevatorPos;
	bool moveForward;
	bool moveUpward;

    elevator_moveCart(Entity @entity){
        @hub = Managers();
        @self = GetEntityByGUID(1508919163);
		@elevator = GetEntityByGUID(1511870044);
        
		moveForward = true;
		moveUpward = false;
        speed = 4.0f;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
	
		if(moveForward == true) {
			tempPos = self.GetWorldPosition();
			tempPos.x += speed * deltaTime;
			self.SetWorldPosition(tempPos);
		}
		
		if(moveUpward == true) {
			elevatorPos = elevator.GetWorldPosition();
			elevatorPos.y += speed * deltaTime;
			elevator.SetWorldPosition(elevatorPos);
			
			tempPos = self.GetWorldPosition();
			tempPos.y += speed * deltaTime;
			self.SetWorldPosition(tempPos);
		}
    }
	
	void MoveUpward() {
		moveForward = false;
		moveUpward = true;
	}
	
	void MoveForward() {
		moveForward = true;
		moveUpward = false;
	}
	
	void StopCart() {
		moveForward = false;
		moveUpward = false;
	}
}