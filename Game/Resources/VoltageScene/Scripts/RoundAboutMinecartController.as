class RoundAboutMinecartController {
    Hub @hub;
    Entity @self;
    Entity @minecart;
	int phase = 0;

    RoundAboutMinecartController(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @minecart = GetEntityByGUID(1508919384);

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
		switch (phase) {
			// Moving forward
			case 0: {
				vec3 position = minecart.GetWorldPosition();
				position.x -= 2.0f * deltaTime;
				minecart.SetWorldPosition(position);
				break;
			}
			// Braking before the fuse box
			case 1: {
				break;
			}
			// Waiting for permission to continue.
			case 2: {
				break;
			}
		}
    }
}
