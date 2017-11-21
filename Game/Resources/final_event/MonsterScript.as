class MonsterScript {
    Hub @hub;
    Entity @self;
	int phase = 0;
	float jumpTranslation = 0.0f;
	vec3 originalPosition;

    MonsterScript(Entity @entity){
        @hub = Managers();
        @self = @entity;

		originalPosition = self.GetWorldPosition();

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
		switch (phase) {
			case 0: // Waiting
				break;
			case 1: { // Jump out
				jumpTranslation += 6.0f * deltaTime;
				float d = 7.0f;
				if (jumpTranslation >= d) {
					jumpTranslation = d;
					phase = 2;
				}

				// http://www.wolframalpha.com/input/?i=y+%3D+-(x-d)x*4*h%2F(d*d)+where+d+%3D+7,+h+%3D+2
				float height = 2.0f;
				float deltaAltitude = -(jumpTranslation - d) * jumpTranslation * 4 * height / (d * d);
				vec3 pos = originalPosition;
				pos.y += deltaAltitude;
				pos.z += jumpTranslation;
				self.SetWorldPosition(pos);

				break;
			}
		}
    }

	void BecomeExposed() {
		print("I am becoming exposed now!\n");
		phase = 1;
	}
}
