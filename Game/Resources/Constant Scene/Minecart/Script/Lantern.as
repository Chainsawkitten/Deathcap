class Lantern {
    DebugDrawingManager @debugDrawingManager;
    Entity @controller;
    Entity @self;
    
    Lantern(Entity @entity){
        @debugDrawingManager = Managers().debugDrawingManager;
        
        @self = entity;
        @controller = GetEntity(1508919751);
        RegisterUpdate();
    }
    
    void Update(float deltaTime){
        self.position.x = controller.position.x;
        self.position.y = controller.position.y;
        self.position.z = controller.position.z;
        self.rotation = controller.rotation;
        
    }
}