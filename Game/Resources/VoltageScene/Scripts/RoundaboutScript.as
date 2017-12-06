class RoundaboutScript {
    Hub @hub;
    Entity @self;
    Entity @minecartController;
    float correctAngle = 54.9f;
    float rotateSpeed;
    float currentAngle = 0;
    bool rotate = false;
    float rotateAngle;
    float amountRotated;
    float direction;

    RoundaboutScript(Entity @entity) {
        @hub = Managers();
        @self = @entity;
        @minecartController = GetEntityByGUID(1511528317);

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (rotate) {
            float rotation = direction * 0.1f * 3.141592f * deltaTime;
            self.RotateAroundWorldAxis(rotation, vec3(0, 1, 0));
            float worldYaw = abs(yaw(self.GetWorldOrientation()));
            if (worldYaw >= radians(correctAngle)) {
                self.SetWorldOrientation(quat(sin(radians(-correctAngle / 2)), 0, 1.0f * cos(radians(-correctAngle / 2)), 0));
                rotate = false;
                SendMessage(minecartController, 0);
            }
        }
    }

    void ReceiveMessage(Entity @sender, int signal) {
        if (!rotate) {
            if (signal == 2) { //Rotate to correct angle
                direction = 1;
                rotate = true;
            }
        }
    }
}
