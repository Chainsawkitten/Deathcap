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
            if(self.rotation.y <= -45.0f){
                SendMessage(sibling, 1);
            }
            else
                self.rotation.y -= 0.3;
            
        }
    }
}