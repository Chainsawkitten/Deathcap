class MainScript {
    Hub @hub;
    Entity @self;
    Entity @minecart;
    int phase;
    float speed;
    Entity @knife;
    Entity @monster;
    Entity @rightHand;
    Entity @camera;
    Entity @particles;
    float waitForMonsterTimer;
    bool knifePickedUp;
    float MonsterHealth = 60.0f;
    float fogTimer = 3.0f;
    // Time for fog to reach 1.0 density. This essentially gives an inverse
    // measure of how fast the darkness approaches the player.
    float fogApexDuration = 10.0f;
    // Time after fog start that fade will begin. Things close to us will be
    // visible through fog even in high density, so we use a color filter to
    // fade the entire screen to black.
    float fadeStartTime = 1.0f;
    float fadeTimer = 0.0f;

    float fadeApexDuration = 3.0f; // Time for fade to reach zero
    float particleTimer = 0.0f;
    bool particleActive = false;
    
    Entity @resultScene;
  
    MainScript(Entity @entity) {
        @hub = Managers();
        @self = @entity;
        @camera = GetEntityByGUID(1508919707);
        @minecart = GetEntityByGUID(1508919163);
        @knife = GetEntityByGUID(1511264657);
        @monster = GetEntityByGUID(1511261389);
        @rightHand = GetEntityByGUID(1508919758);
        Component::Lens @lens;
        @particles = GetEntityByGUID(1512553749);
        particles.SetEnabled(false, false);
        phase = 0;
        speed = 4.0f;
        waitForMonsterTimer = 0.0f;
        knifePickedUp = false;
        particleActive = false;

        self.SetEnabled(false, true);

        // Remove this if updates are not desired.
        RegisterUpdate();
        
        @resultScene = GetEntityByGUID(1513352309);
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (particleActive)
            particleTimer += deltaTime;

        if (particleTimer > 1.0f) {
            particles.SetEnabled(false, false);
            particleTimer = 0.0f;
        }

        switch (phase) {
            case 0: { // Entering final scene.
                vec3 pos = minecart.GetWorldPosition();
                pos.x -= speed * deltaTime;
                minecart.SetWorldPosition(pos);
                break;
            }
            case 1: { // Stopping smoothly
                vec3 pos = minecart.GetWorldPosition();
                if (pos.x > 0.0f) {
                    pos.x += 0.05 * deltaTime;
                    minecart.SetWorldPosition(pos);
                }

                // Puzzle Skip.
                if (!IsVRActive() && Input(PuzzleSkip, @self)) {
                    phase = 2;
                    SendMessage(monster, 1); // Monster died.
                }
                break;
            }
            case 2: { // Wait for monster to collapse.
                waitForMonsterTimer += deltaTime;
                if (waitForMonsterTimer >= 3.0f) {
                    phase = 3;
                    PrepareForFade();
                }
                break;
            }
            case 3: { // Continue after monster has been killed.
                vec3 pos = minecart.GetWorldPosition();
                pos.x -= speed * deltaTime;
                minecart.SetWorldPosition(pos);
                if (DoFade(deltaTime))
                    GameOver(true);
                break;
            }
            case 4: { // Lost phase
                if (DoFade(deltaTime))
                    GameOver(false);
                break;
            }
        }
    }

    void ReceiveMessage(Entity @sender, int signal) {
        switch (signal) {
            case 0: { // When monster has successfully eaten the player.
                PrepareForFade();
                phase = 4; // Lost phase.
                DropKnife();
                break;
            }
        }
    }

    void PrepareForFade() {
        hub.renderManager.SetFogApply(true);
        hub.renderManager.SetColorFilterApply(true);
        hub.renderManager.SetFogColor(vec3(0, 0, 0));
    }

    bool DoFade(float deltaTime) {
        // The idea of fading is that we start with fog to begin fading
        // with distance. However, with fog we can generally see things
        // close to us. To finish of the fade, a color filter approaching
        // zero is used when the fog has darkened most of the view. The
        // color filter applies to the entire screen uniformly which is
        // why it's not used exclusively, but works nicely when fog has
        // consumed most of the scene.

        // Cube curve to get a density function that grows slowly at
        // first and faster later to darken things close to us as well.
        fogTimer += deltaTime;
        float fogRatio = fogTimer / fogApexDuration;
        fogRatio = fogRatio * fogRatio * fogRatio;

        bool completed = false;
        if (fogTimer >= fadeStartTime) {
            fadeTimer += deltaTime;
            float fadeRatio = fadeTimer / fadeApexDuration;
            if (fadeRatio >= 1.0f) {
                fadeRatio = 1.0f;
                completed = true;
            }
            hub.renderManager.SetColorFilterColor(vec3(1.0f - fadeRatio, 1.0f - fadeRatio, 1.0f - fadeRatio));
        }

        hub.renderManager.SetFogDensity(fogRatio);

        return completed;
    }

    void GameOver(bool won) {
        phase = 1337;
        if (won)
            print("Player won the game.\n");
        else
            print("Player was eaten alive.\n");
        
        camera.GetParent().SetWorldPosition(resultScene.GetWorldPosition());
        hub.renderManager.SetColorFilterColor(vec3(1.0f, 1.0f, 1.0f));
        hub.renderManager.SetFogDensity(0.0f);
        
        if (!won)
            GetEntityByGUID(1513352348).RotateYaw(3.141592f);
      
        //if(!IsVRActive())
        //    exit(0);
        knife.position = vec3(30, 30, 30);

    }

    void StopBeforeMonster() {
        phase = 1;
        SendMessage(monster, 0); // Player was stopped.
    }

    void HoveringKnife() {
        if (Input(Trigger, rightHand) && !knifePickedUp) {
            knife.SetParent(rightHand);
            knife.position = vec3(0, 0, 0);
            knife.SetLocalOrientation(quat(1.0f, 0.0f, 0.0f, 0.0f));
            knife.RotateYaw(radians(90.0f));
            knifePickedUp = true;
            SendMessage(rightHand, 2);
        }
    }

    void KnifeHitMonster() {
        particles.SetEnabled(true, false);
        particleActive = true;
        MonsterHealth -= 20.0f;

        // Only allow killing the monster in the early phases.
        if ((phase == 0 || phase == 1) && knifePickedUp && MonsterHealth <= 0.0f) {
            SendMessage(monster, 1); // Die.
            phase = 2; // Wait for collapse.
            DropKnife();
        }

        //Particle Effect
    }

    void DropKnife() {
        if (knifePickedUp)
            knife.position = vec3(30, 30, 30);
    }
}
