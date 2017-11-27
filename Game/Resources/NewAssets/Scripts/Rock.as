class Rock {
    Hub @hub;
    Entity @self;
    Entity @originalParent;
    Entity @bigBalkPivot;
    Entity @bucketParent;
    Entity @rightCtrl;
    bool isPickedUp;

    Rock(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @originalParent = self.GetParent();
        @bigBalkPivot = GetEntityByGUID(1510912841);
        @bucketParent = GetEntityByGUID(1510911323);
        @rightCtrl = GetEntityByGUID(1508919758);
        isPickedUp = false;
        
        RegisterUpdate();
    }
    
    void Update(float deltaTime) {
        if (!Input(Trigger, rightCtrl) && isPickedUp == true) {
            hub.physicsManager.MakeDynamic(self.GetRigidBody());
            hub.physicsManager.ForceTransformSync(self.GetRigidBody());
            vec3 tempWorldPos = self.GetWorldPosition();
            quat tempWorldRot = self.GetWorldOrientation();
            self.SetParent(originalParent);
            self.SetWorldPosition(tempWorldPos);
            self.SetWorldOrientation(tempWorldRot);
            isPickedUp = false;
        }
    }
    
    void HitBucket() {
        SendMessage(bigBalkPivot, 1);
        hub.physicsManager.MakeKinematic(self.GetRigidBody());
        self.SetWorldPosition(bucketParent.GetWorldPosition());
        self.SetParent(bucketParent);
        self.GetSoundSource().Play();
    }
    
    void PickedUp() {
        if (Input(Trigger, rightCtrl) && isPickedUp == false) {
            SendMessage(bigBalkPivot, 2);
            hub.physicsManager.MakeKinematic(self.GetRigidBody());
            self.position = vec3(0.0f,0.0f,0.0f);
            self.SetParent(rightCtrl);
            isPickedUp = true;
        }
    }
}
