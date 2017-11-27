class MonsterTestScript {
    Hub @hub;
    Entity @self;
	float speed;
	
	float direction;
	float maxTimer;
	float timer;
	
    MonsterTestScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

		direction = 1;
		
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

	bool first = true;
    // Called by the engine for each frame.
    void Update(float deltaTime) {

		if(first){
		
			timer = maxTimer;
			first = false;
		
		}
		
		self.position.x += deltaTime * direction * speed;
		timer -= deltaTime;
		float t = timer;
		if(timer < 0){
		
			direction = direction * -1;
			timer = maxTimer;
			
		}
	
    }
}
