class RailAudioPlayer {
    Hub @hub;
    Entity @self;
    vec3 tempPos;
    bool playingAudio;
    
    RailAudioPlayer(Entity @entity){
        @hub = Managers();
        @self = @entity;
        playingAudio = false;
        tempPos = self.GetWorldPosition();
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
    	vec3 curPos = self.GetWorldPosition();
    	
    	float v = distance(curPos, tempPos) / deltaTime;
    	
    	if (v > 0.5f) {
    		if(!playingAudio) {
    			print("Starting Audio\n");
    			playingAudio = true;
    			self.GetSoundSource().Play();
    		}
    	}
    	else {
    		if(playingAudio) {
    			print("Stopping Audio\n");
    			playingAudio = false;
    			self.GetSoundSource().Pause();	
    		}
    	}
    	
    	tempPos = curPos;
    }
}
