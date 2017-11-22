class MainScript {
    Hub @hub;
    Entity @self;
	Entity @minecart;
	int phase;
	float speed;
	Entity @knife;

    MainScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

		@minecart = GetEntityByGUID(1511257883);
		@knife = GetEntityByGUID(1511264657);
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
		}
    }

	void StopBeforeMonster() {
		phase = 1;
	}

	void HoveringKnife() {
		// Check input, pick up knife if clicked.
	}
}
