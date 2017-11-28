class Cart_debug_script{
    Hub @hub;
    Entity @self;
    
    Cart_debug_script(Entity @entity) {
        @hub = Managers();
        @self = GetEntityByGUID(1508919384);
        
        RegisterUpdate();
		
		if (@self == null)
            print("NULLLLL!\n");
        print("ASDf\n");
    }
    
    //Update carts movements and send it's position to Player Script.
    void Update(float deltaTime) {
        self.SetWorldPosition(vec3(5.0f, -0.9f,0.02949f)); //first puzzel
    }
}