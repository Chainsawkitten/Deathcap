class AnimScript {
    Hub @hub;
    Entity @self;
	Component::AnimationController @animCtrl;
	
    AnimScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
		@animCtrl = self.GetAnimationController(); 
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
	
    }
	
	void TriggerAnim(){
		print(self.name + "\n");
		animCtrl.SetBool("D", true);
	}
}
