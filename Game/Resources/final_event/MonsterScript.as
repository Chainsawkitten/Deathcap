class MonsterScript {
    Hub @hub;
    Entity @self;
	int phase = 0;
	float fallspeed = 0.0f;
	float waitTimer = 0.0f;
	float eatingTimer = 0.0f;
	Entity @cart;

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
			case 2: { // Landing
				waitTimer += deltaTime;
				if (waitTimer >= 1.0f) {
					phase = 3;
				}
				break;
			}
			case 3: { // Approach player
				self.position.x -= 6.0f * deltaTime;
				break;
			}
			case 5: { // Eating
				eatingTimer += deltaTime;
				if (eatingTimer >= 4.0f) {
					SendMessage(cart, 0); // Player was eaten
					phase = 7;
				}
				break;
			}
		}
	}

	void ReceiveMessage(Entity @sender, int signal) {
		switch (signal) {
			case 0: { // Player was stopped by monster
				@cart = @sender;
				phase = 5; // Start eating
				print("Monster: I'm going to eat you now.\n");
				break;
			}
			case 1: { // Die
				phase = 6; // Collapse
				print("Monster: I'm dying now.\n");
				break;
			}
		}
	}

	void BecomeExposed() {
		print("I am becoming exposed now!\n");
		phase = 1;
	}

	void StopCharging() {
		print("Stopping my charge\n");
		phase = 4;
	}
}
