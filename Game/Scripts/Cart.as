class Cart{
	Hub @hub;
	Entity @self;
	Cart(Entity @entity){
		@hub = Managers();
		@self = @entity;
		RegisterUpdate();
	}
	
	//Update carts movements and send it's position to Player Script.
	void Update(float i){
		self.position.z -= 0.01f;
	}
}