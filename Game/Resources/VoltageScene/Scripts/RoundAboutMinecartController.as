class RoundAboutMinecartController {
    Hub @hub;
    Entity @self;
    Entity @minecart;
    Entity @brakeTarget;
    Entity @mastermind;
    int phase = 0;
    float speed = 2.0f;
    // v(t) = a*t*t + b*t + c
    // s(t) = V(t)
    // s(0) = 0, s(endTime) = breakDistance
    // v(0) = speed, v(endTime) = 0
    float a;
    float b;
    float c;
    float endTime = 5.0f;
    float stopTime = 0.0f;
    float brakeStartLocation = 0.0f;

    RoundAboutMinecartController(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @minecart = GetEntityByGUID(1508919384);
        @brakeTarget = GetEntityByGUID(1512484898);
        @mastermind = GetEntityByGUID(1511791235);

        // Remove this if updates are not desired.
        RegisterUpdate();
        
        self.SetEnabled(false, true);
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        switch (phase) {
            // Moving forward
            case 0: {
                vec3 position = minecart.GetWorldPosition();
                position.x -= speed * deltaTime;
                minecart.SetWorldPosition(position);
                break;
            }
            // Braking before the fuse box.
            case 1: {
                if (stopTime < endTime) {
                    stopTime += deltaTime;
                    float s = a * stopTime * stopTime * stopTime / 3 + b * stopTime * stopTime / 2 + c * stopTime;
                    vec3 pos = minecart.GetWorldPosition();
                    pos.x = brakeStartLocation - s;
                    minecart.SetWorldPosition(pos);
                } else {
                    phase = 2;
                }
                break;
            }
            // Waiting for permission to continue.
            case 2: {
                if (!IsVRActive() && Input(PuzzleSkip, @self))
                    SendMessage(mastermind, 100);
                break;
            }
        }
    }

    void ReceiveMessage(Entity @sender, int signal) {
        // Continue rolling when roundabout is in place.
        if (signal == 0) {
            phase = 0;
        }
    }

    void CommenceBrakingPhase() {
        phase = 1;
        brakeStartLocation = minecart.GetWorldPosition().x;
        float brakeDistance = abs(brakeTarget.GetWorldPosition().x - brakeStartLocation); // The distance during which we will brake.
        c = speed;
        b = (6 * brakeDistance - 4 * c * endTime) / (endTime * endTime);
        a = -(c + b * endTime) / (endTime * endTime);
    }
}
