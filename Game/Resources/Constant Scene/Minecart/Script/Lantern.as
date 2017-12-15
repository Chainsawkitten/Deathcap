class Lantern {
    Entity @self;
    Entity @originalParent;
    bool pickup;
    
    Lantern(Entity @entity){
        @self = entity;
        @originalParent = self.GetParent();
        
        pickup = false;
        
        if(!IsVRActive()){
            self.SetParent(originalParent);
            self.position = vec3(-0.105f, 0.937f, -0.547f);
        }
    }
    
    void ReceiveMessage(Entity @sender, int i) {
        if (i == 1) {
            self.SetParent(sender);
            self.position = vec3(0.0f, -0.18f, 0.013f);
        }
    }
}