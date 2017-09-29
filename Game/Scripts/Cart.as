class Cart{
    Hub @hub;
    Entity @self;
	float speed = 0;
    Cart(Entity @entity){
        @hub = Managers();
        @self = @entity;
        RegisterUpdate();
    }
    
    //Update carts movements and send it's position to Player Script.
    void Update(float deltaTime){
        self.position.z -= speed*deltaTime;
    }
}