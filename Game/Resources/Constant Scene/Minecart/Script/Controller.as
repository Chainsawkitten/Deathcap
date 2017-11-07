class Controller{
    Hub @hub;
    Entity @self;
    Entity @testEntity;
    
    Controller(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @testEntity = GetEntity(1509964911);
        RegisterUpdate();
    }
    
    void Update(float deltaTime){
        if(Input(Grip)){
        print("True");
            SendMessage(testEntity, 2);
        }
        else{
            SendMessage(testEntity, 1);
        }
    }
}