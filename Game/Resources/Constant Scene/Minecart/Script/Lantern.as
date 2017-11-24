class Lantern {
    DebugDrawingManager @debugDrawingManager;
    Entity @self;
    Entity @controller;
    Entity @originalParent;
    bool pickup;
    
    Lantern(Entity @entity){
        @debugDrawingManager = Managers().debugDrawingManager;
        @self = entity;
        @controller = GetEntityByGUID(1508919751);
        @originalParent = self.GetParent();
        
        pickup = false;
    }
    
    void ReceiveMessage(Entity @sender, int i) {
        if (i == 1 || !IsVRActive) {
            self.SetParent(sender);
            self.position = vec3(0.0f, -0.18f, 0.013f);
        }
        else if (i == 2) {
            self.SetParent(originalParent);
            self.position = vec3(-0.105f, 0.137f, -0.547f);
        }
}