class MainScript {
    Hub @hub;
    Entity @self;
	Entity @minecart;
	int phase;
	float speed;
	Entity @knife;
	Entity @monster;
	float waitForMonsterTimer = 0.0f;

    MainScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

		@minecart = GetEntityByGUID(1511257883);
		@knife = GetEntityByGUID(1511264657);
		@monster = GetEntityByGUID(1511261389);
		phase = 0;
		speed = 4.0f;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
		switch (phase) {
			case 0: { // Entering final scene
				vec3 pos = minecart.GetWorldPosition();
				pos.x += speed * deltaTime;
				minecart.SetWorldPosition(pos);
				break;
			}
			case 2: { // Wait for monster to collapse
				waitForMonsterTimer += deltaTime;
				if (waitForMonsterTimer >= 3.0f) {
					phase = 3;
					print("Player: Honk honk, motherfucker! I'm free!\n");
				}
				break;
			}
			case 3: { // Continue after monster has been killed
				vec3 pos = minecart.GetWorldPosition();
				pos.x += speed * deltaTime;
				minecart.SetWorldPosition(pos);
				break;
			}
		}
	}

	void ReceiveMessage(Entity @sender, int signal) {
		switch (signal) {
			case 0: { // When monster has successfully eaten the player
				phase = 4; // Lost phase
				print("Monster: I'm losing.\n");
				break;
			}
		}
	}

	void StopBeforeMonster() {
		phase = 1;
		SendMessage(monster, 0); // Player was stopped
	}

	void HoveringKnife() {
		// Check input, pick up knife if clicked.
	}

	void KnifeHitMonster() {
		if (phase != 4) {
			SendMessage(monster, 1); // Die
			phase = 2; // Wait for collapse
			print("Player: I'm going to wait for the monster to collapse now.\n");
		}
	}
}
