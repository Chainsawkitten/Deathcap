class MonsterScript {
    Hub @hub;
    Entity @self;
	int phase = 0;
	float fallspeed = 0.0f;

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
				fallspeed += 9.82f * deltaTime;
				self.position.y -= fallspeed * deltaTime;
				if (self.position.y <= 0.0f) {
					self.position.y = 0.0f;
					phase = 2;
				}
				break;
			}
			case 2: { // Approach player
				self.position.x -= 6.0f * deltaTime;
				break;
			}
		}
    }

	void BecomeExposed() {
		print("I am becoming exposed now!\n");
		phase = 1;
	}
}
