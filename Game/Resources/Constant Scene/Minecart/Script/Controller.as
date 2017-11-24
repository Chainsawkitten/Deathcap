class Controller {
    Hub @hub;
    Entity @self;
    Entity @lantern;
    bool pickUp;

    Controller(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @lantern = GetEntityByGUID(1509711303);
        pickUp = false;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (!Input(Trigger, self) && pickUp) {
            pickUp = false;
            SendMessage(lantern, 2);
        }
    }
    
    void OnLanternTrigger() {
        if(Input(Trigger, self) && pickUp == false){
            pickUp = true;
            SendMessage(lantern, 1);
        }
    }
}
