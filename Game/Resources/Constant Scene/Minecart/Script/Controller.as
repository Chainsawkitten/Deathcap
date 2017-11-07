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
            @testEntity = IsIntersect(@self);
            if (testEntity != null) 
                SendMessage(testEntity, 2); //Break
        }
    }
}