class Bridge {
    Entity @self;
    Entity @cart;
    bool trigger;
    bool playingAudio;

    Bridge(Entity @entity){
        @self = @entity;
        @cart = GetEntityByGUID(1510920517);
        trigger = false;
        playingAudio = false;
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (trigger == true && roll(self.rotation) < radians(0.0f)){
            if(playingAudio == false){
                playingAudio = true;
                self.GetSoundSource().Play();
            }
            self.RotateRoll(radians(0.12f));
        }
        else if (roll(self.rotation) > 0.0f) {
            trigger = false;
            SendMessage(cart, 3);
        }
    }
    
    void ReceiveMessage(Entity @sender, int i){
        if (i == 1 && trigger == false)
            trigger = true;
    }
}
