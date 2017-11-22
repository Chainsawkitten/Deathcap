class MainScript {
    Hub @hub;
    Entity @self;
	Entity @minecart;
	int phase;
	float speed;
	Entity @knife;
	Entity @monster;
    Entity @rightHand;
	float waitForMonsterTimer = 0.0f;
    bool knifePickedUp = false;

    MainScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

		@minecart = GetEntityByGUID(1508919163);
		@knife = GetEntityByGUID(1511264657);
		@monster = GetEntityByGUID(1511261389);
        @rightHand = GetEntityByGUID(1508919758);
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
                // Fade here
				break;
			}
		}
	}

	void ReceiveMessage(Entity @sender, int signal) {
		switch (signal) {
			case 0: { // When monster has successfully eaten the player
				phase = 4; // Lost phase
				print("Player: I'm losing.\n");
				break;
			}
		}
	}

	void StopBeforeMonster() {
		phase = 1;
		SendMessage(monster, 0); // Player was stopped
	}

	void HoveringKnife() {
        if (Input(Trigger, rightHand) && !knifePickedUp) {
            knife.SetParent(rightHand);
            knife.position = vec3(0, 0, 0);
            knife.SetLocalOrientation(quat(1.0f, 0.0f, 0.0f, 0.0f));
            knife.RotateYaw(radians(90.0f));
            knifePickedUp = true;
        }
	}

	void KnifeHitMonster() {
		if (phase != 4 && knifePickedUp) {
			SendMessage(monster, 1); // Die
			phase = 2; // Wait for collapse
			print("Player: I'm going to wait for the monster to collapse now.\n");
		}
	}
}
