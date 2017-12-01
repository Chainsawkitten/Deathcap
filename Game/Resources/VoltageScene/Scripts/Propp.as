class Propp {
    Hub @hub;
    Entity @self;
    Entity @rightCtrl;
    Entity @originalParent;
    Entity @S1T1;
    bool isPressed;
    vec3 worldPos;

    Propp(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @rightCtrl = GetEntityByGUID(1508919758);
        @originalParent = self.GetParent();
        @S1T1 = GetEntityByGUID(1905121);
        
        worldPos = S1T1.GetWorldPosition();
        isPressed = false;
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (!Input(Trigger, rightCtrl) && isPressed) {
            isPressed = false;
            vec3 tempPos = self.GetWorldPosition();
            self.SetParent(originalParent);
            self.SetWorldPosition(tempPos);
        }
    }
    
    void PickupTrigger() {
        if (Input(Trigger, rightCtrl) && rightCtrl.GetChildFromIndex(1) == null &&!isPressed) {
            isPressed = true;
            self.position = vec3(0.0f, 0.0f, 0.0f);
            self.SetParent(rightCtrl);
        }
    }
}
