class MonsterScript {
    Hub @hub;
    Entity @self;
    Entity @cart;
    float fallspeed;
    float waitTimer;
    float eatingTimer;
    int phase;
    
	Component::SoundSource @snd_shriek;
    Component::SoundSource @snd_death;
    Component::SoundSource @snd_landing;
    Component::SoundSource @snd_falling;

    MonsterScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @cart = GetEntityByGUID(1511260476);
        
        @snd_shriek = GetEntityByGUID(1512046440).GetSoundSource();
        @snd_landing = GetEntityByGUID(1512045002).GetSoundSource();
        @snd_death = GetEntityByGUID(1512044219).GetSoundSource();
        @snd_falling = GetEntityByGUID(1512044955).GetSoundSource();
        
        phase = 0;
        fallspeed = 0.0f;
        waitTimer = 0.0f;
        eatingTimer = 0.0f;

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
                    snd_landing.Play();
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
            case 6: { // Dying
                self.RotateYaw(3.141592f * deltaTime);
                break;
            }
        }
    }

    void ReceiveMessage(Entity @sender, int signal) {
        switch (signal) {
            case 0: { // Player was stopped by monster
                phase = 5; // Start eating
                print("Monster: I'm going to eat you now.\n");
                break;
            }
            case 1: { // Die
                phase = 6; // Collapse
                snd_death.Play();
                print("Monster: I'm dying now.\n");
                break;
            }
        }
    }

    void BecomeExposed() {
        print("I am becoming exposed now!\n");
        phase = 1;
        snd_falling.Play();
    }

    void StopCharging() {
        print("Stopping my charge\n");
        phase = 4;
        snd_shriek.Play();
    }
}
