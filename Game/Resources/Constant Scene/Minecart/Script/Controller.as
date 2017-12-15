class Controller {
    Entity @self;
    Entity @rock;
    bool isPressed;
    Entity @lantern;
    Entity @childModel;
    bool pickUp;
    Component::AnimationController @animCtrl;

    Controller(Entity @entity){
        @self = @entity;
        @rock = GetEntityByGUID(1510240479);
        isPressed = false;
        @lantern = GetEntityByGUID(1509711303);
        @childModel = self.GetChildFromIndex(0);
        @animCtrl = childModel.GetAnimationController();
        pickUp = false;

        if(IsVRActive() && self.GetUniqueIdentifier() == 1508919751) {
            SendMessage(lantern, 1);
            animCtrl.SetBool("Closed", true);
            animCtrl.SetBool("Open", false);
        }

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if(!IsVRActive()){
            self.position.y = -3;
            self.position.x = -1;
        }

        if (!Input(Trigger, self) && self.GetUniqueIdentifier() != 1508919751) {
            animCtrl.SetBool("Open", true);
            animCtrl.SetBool("Closed", false);
            childModel.SetEnabled(true, false);

            if (isPressed) {
                isPressed = false;
                SendMessage(rock, 2);
            }
        }
        
        if (Input(Trigger, self) && self.GetUniqueIdentifier() != 1508919751) {
            animCtrl.SetBool("Open", false);
            animCtrl.SetBool("Closed", true);
        }
    }

    void OnLanternTrigger() {
        if(Input(Trigger, self) && pickUp == false){
            pickUp = true;
            SendMessage(lantern, 1);
        }
    }

    void ReceiveMessage(Entity @sender, int i) {
        if (i == 1) {
            childModel.SetEnabled(true, false);
        } else if (i == 2) {
            childModel.SetEnabled(false, false);
        }
    }
}
