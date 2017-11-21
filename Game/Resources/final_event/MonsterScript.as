class MonsterScript {
    Hub @hub;
    Entity @self;
	int phase = 0;

    MonsterScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
		switch (phase) {
			case 0: // Waiting
				break;
			case 1: { // Jump out
				self.position.z += 6.0f * deltaTime;
				if (self.position.z >= 0.0f) {
					self.position.z = 0.0f;
					phase = 2;
				}
				break;
			}
		}
    }

	void BecomeExposed() {
		print("I am becoming exposed now!\n");
		phase = 1;
	}
}
