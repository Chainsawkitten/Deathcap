class Controller{
    Hub @hub;
    Entity @self;
    Entity @testEntity;
    bool isPressed;
    
    Controller(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @testEntity = GetEntity(1509964911);
        RegisterUpdate();
        isPressed = false;
    }
    
    void Update(float deltaTime){
        if (Input(Grip) && !isPressed) {
            print("Grip pressed" + self.GetUniqueIdentifier() + "\n");
            SendMessage(testEntity, 2);
            isPressed = true;
        } else if (!Input(Grip) && isPressed) {
            print("Grip unpressed\n");
            SendMessage(testEntity, 1);
            isPressed = false;
        }
    }
}