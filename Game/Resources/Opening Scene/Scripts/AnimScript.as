class AnimScript {
    Hub @hub;
    Entity @self;
    AnimScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
		self.GetAnimationController().SetBool("Lol", true);
    }
	
	void TriggerAnim(){
		print(self.name + "\n");
	}
}
