class MonsterScript {
    Hub @hub;
    Entity @self;

    MonsterScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }

	void BecomeExposed() {
		print("I am becoming exposed now!\n");
	}
}
