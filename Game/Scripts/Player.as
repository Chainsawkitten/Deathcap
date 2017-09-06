class Player{
	Hub @hub;
	Entity @self;
	Entity @cart;
	vec2 Cursor;
	Player(Entity @entity){
		@hub = Managers();
		@self = @entity;
		@cart = @self.GetParent();
		RegisterUpdate();
		Cursor = vec2(0,0);
	}
	
	//Update carts movements and send it's position to Player Script.
	void Update(float i){
		self.position = vec3(cart.position.x, cart.position.y+4, cart.position.z+1);
		if(Cursor.x == 0 && Cursor.y == 0)
			Cursor = GetCursorXY();
		self.rotation.x += 0.3f * (GetCursorXY().x - Cursor.x);
		self.rotation.y += 0.3f * (GetCursorXY().y - Cursor.y);
		
		Cursor = GetCursorXY();
	}
}