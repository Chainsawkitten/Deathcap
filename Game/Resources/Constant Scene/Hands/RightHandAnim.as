class RightHandAnim {
    Hub @hub;
    Entity @self;

    RightHandAnim(Entity @entity){
        @hub = Managers();
        @self = @entity;
		animationControlller = self.GetAnimationController();

        RegisterUpdate();
    }

    void Update(float deltaTime) {

    }
}
