class Lever {
    Hub @hub;
    Entity @self;
    Entity @sibling;
    
    Lever(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @sibling = self.GetParent().GetChild("Gates");
        
        RegisterUpdate();
    }
    
    void Update(float deltaTime){
        if (Input(OpenGate)){
            if(self.rotation.x <= -45.0f){
                SendMessage(sibling, 1);
            }
            else
                self.rotation.x -= 0.3;
            
        }
    }
}