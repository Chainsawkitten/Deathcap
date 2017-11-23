class Controller {
    Hub @hub;
    Entity @self;
    Entity @rock;
    Entity @cart;
    bool isPressed;
    bool pickUp;

    Controller(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @rock = GetEntityByGUID(1510240479);
        @cart = GetEntityByGUID(1508919384);
        isPressed = false;
        pickUp = false;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        //if (self.name == "Right Controller") {
        //    print("Controller position: (" + self.GetWorldPosition().x + ", " + self.GetWorldPosition().y + ", " + self.GetWorldPosition().z + ")\n");
        //}
        if(!IsVRActive()){
            self.position.y = -3;
            self.position.x = -1;
        }
        if (!Input(Trigger, self) && isPressed) {
            isPressed = false;
            SendMessage(rock, 2);
        }
        
        if (!Input(Trigger, self) && pickUp) {
            pickUp = false;
            SendMessage(lantern, 2);
        }
    }
    
    void OnRockTrigger() {
        //print("Rock trigger\n");
        if (Input(Trigger, self) && !isPressed) {
            isPressed = true;
            SendMessage(rock, 1);
        } 
    }
    
    void OnLanternTrigger() {
        print("Lantern Trigger\n");
        if(Input(Trigger, self) && pickUp == false){
            pickUp = true;
            SendMessage(lantern, 1);
        }
    }
}
