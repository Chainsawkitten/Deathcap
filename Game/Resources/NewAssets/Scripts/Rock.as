class Rock {
    Hub @hub;
    Entity @self;
    Entity @originalParent;
    Entity @bridgeParent;

    Rock(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @originalParent = self.GetParent();
        @bridgeParent = GetEntityByGUID(1510240835);

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (self.GetWorldPosition().x <= 0.0f){
            SendMessage(bridgeParent, 1);
            hub.physicsManager.MakeKinematic(self.GetRigidBody());
            self.SetWorldPosition(vec3(4.524f, -0.994f, -1.070f));
        }
    }
    
    void ReceiveMessage(Entity @sender, int i)
    {
        if (i == 1) {
                hub.physicsManager.MakeDynamic(self.GetRigidBody());
                hub.physicsManager.ForceTransformSync(self.GetRigidBody());
                vec3 tempWorldPos = self.GetWorldPosition();
                quat tempWorldRot = self.GetWorldOrientation();
                self.SetParent(originalParent);
                self.SetWorldPosition(tempWorldPos);
                self.SetWorldOrientation(tempWorldRot);
        } else if (i == 2) {
                hub.physicsManager.MakeKinematic(self.GetRigidBody());
                self.position = vec3(0.0f,0.0f,0.0f);
                self.SetParent(sender);
        }
    }
}
