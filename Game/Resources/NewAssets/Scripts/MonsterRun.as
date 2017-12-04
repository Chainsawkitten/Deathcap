class MonsterRun {
    Hub @hub;
    Entity @self;
	vec3 monsterPos;
	float speed;
	bool run;
	Component::AnimationController @animCtrl;
	
    MonsterRun(Entity @entity){
        @hub = Managers();
        @self = @entity;
		speed = 5.0f;
		run = false;
		@animCtrl = self.GetAnimationController();
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
		
		if(run){
			monsterPos = self.GetWorldPosition();
			speed += deltaTime * 1.1f;
		
			animCtrl.SetFloat("Speed", speed / 10.0f);
		
			monsterPos.x -= speed * deltaTime;
	
			self.SetWorldPosition(monsterPos); 
		}

    }
	
	void MonsterRunning(){
		run = true;
	}
	
	void ReceiveMessage(Entity @sender, int signal){
		if(signal == 0){
			MonsterRunning();
		}
	}
	
}
