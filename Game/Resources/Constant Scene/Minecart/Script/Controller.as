class Controller {
    Entity @self;
    Entity @rock;
    bool isPressed;
    Entity @lantern;
    bool pickUp;
    Component::AnimationController @animCtrl;

    Controller(Entity @entity){
        @self = @entity;
        @rock = GetEntityByGUID(1510240479);
        isPressed = false;
        @lantern = GetEntityByGUID(1509711303);
        @animCtrl = self.GetChildFromIndex(0).GetAnimationController();
        pickUp = false;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if(!IsVRActive()){
            self.position.y = -3;
            self.position.x = -1;
        }
        
        if (!Input(Trigger, self)) {
            animCtrl.SetBool("Open", true);
            animCtrl.SetBool("Closed", false);
                        print("Closed");
        
            if (isPressed) {
                isPressed = false;
                SendMessage(rock, 2);
            }
            
            if (pickUp) {
                pickUp = false;
                SendMessage(lantern, 2);
            } 
        }
        
        if (Input(Trigger, self)) {
            animCtrl.SetBool("Open", false);
            animCtrl.SetBool("Closed", true);
            print("Open");
        }
    }
    
    void OnLanternTrigger() {
        if(Input(Trigger, self) && pickUp == false){
            pickUp = true;
            SendMessage(lantern, 1);
        }
    }
}
