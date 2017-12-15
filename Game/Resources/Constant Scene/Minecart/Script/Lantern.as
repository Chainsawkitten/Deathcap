class Lantern {
    Entity @self;
    Entity @originalParent;
    Entity @camera;
    bool pickup;
    
    Lantern(Entity @entity){
        @self = entity;
        @originalParent = self.GetParent();
        @camera = GetEntityByGUID(1508919707);
        
        pickup = false;
        if(!IsVRActive()){
            self.SetParent(camera);
            self.position = vec3(-0.0f, 0.0f, -0.0f);
        }
    }
    
    
    void ReceiveMessage(Entity @sender, int i) {
        if (i == 1) {
            self.SetParent(sender);
            self.position = vec3(0.0f, -0.18f, 0.013f);
        }
    }
}