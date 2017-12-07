class MonsterRun {
    Hub @hub;
    Entity @self;
    vec3 monsterPos;
    float speed;
    bool run;
    Component::AnimationController @animCtrl;
    
    MonsterRun(Entity @entity){
        @hub = Managers();
        @self = @entity;
        speed = 2.1f;
        run = false;
        @animCtrl = self.GetAnimationController();
        animCtrl.SetBool("E", false);
        // Remove this if updates are not desired.
        RegisterUpdate();
        self.SetEnabled(false, true);
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        
        if(run){
        
            if(monsterPos.x > -145.7f){
                monsterPos = self.GetWorldPosition();
            
                monsterPos.x -= speed * deltaTime;
        
                self.SetWorldPosition(monsterPos); 
            }
            if(monsterPos.x < -142.7f){
                animCtrl.SetBool("E", true);
            }

        }

    }
    
    void MonsterRunning(){
        run = true;
    }
    
    void ReceiveMessage(Entity @sender, int signal){
        if(signal == 0){
            MonsterRunning();
        }
    }
    
}
