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
    
    void OnTrigger(){
        if(Input(Grip) && pickup == false){
            self.SetParent(controller);
            self.position = vec3(0.0f, -0.18f, 0.013f);
            pickup = true;
        }
        else if(Input(Grip) == false && pickup == true) {
            self.SetParent(originalParent);
            self.position = vec3(-0.105f, 0.137f, -0.547f);
            pickup = false;
        }
    }
}