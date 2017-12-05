class Mastermind {

    Hub @hub;
    Entity @self;

	Entity@ slot1 = null;
	Entity@ slot2 = null;
	Entity@ slot3 = null;
	Entity@ slot4 = null;
	
	Entity @light1;
	Entity @light2;
	Entity @light3;
	Entity @light4;
    
    Entity @flickeringLights;
	Entity @roundabout;
    
    Mastermind(Entity @entity){
        @hub = Managers();
        @self = @entity;
		
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        
		for(int i = 0; i < correct; i++){
		
            TurnOnLight(100, vec3(0,255,0));
				
		}
		for(int i = 0; i < rightColors; i++){
		
            TurnOnLight(100, vec3(255,0,0));
				
		}
		
		if(puzzleSolved){
		
		
		}
        else{
        
            
        
        }
			
    }
		
	bool puzzleSolved = false;
	int correct = 0;
	int rightColors = 0;
    
	void CheckSolution(){
		correct = 0;
		rightColors = 0;
		puzzleSolved = false;
        TurnOffLights();

		if (slot1 != null && slot2 != null && slot3 != null && slot4 != null) {
			
			if (slot1.name == "Green")
				correct = correct + 1;
			else if(slot1.name == "Red" || slot1.name == "Yellow" || slot1.name == "Purple")
				rightColors = rightColors + 1;
				
			if (slot2.name == "Red")
				correct = correct + 1;
			else if(slot2.name == "Green" || slot2.name == "Yellow" || slot2.name == "Purple")
				rightColors = rightColors + 1;
				
			if (slot3.name == "Yellow")
				correct = correct + 1;
			else if(slot3.name == "Red" || slot3.name == "Green" || slot3.name == "Purple")
				rightColors = rightColors + 1;
				
			if (slot4.name == "Purple")
				correct = correct + 1;
			else if(slot4.name == "Red" || slot4.name == "Yellow" || slot4.name == "Green")
				rightColors = rightColors + 1;
				
			if(correct == 4)
				puzzleSolved = true;
		
		}

	}
	
    void TurnOnLight(int intensity, vec3 lightColor){
    
        if(light1.GetSpotLight().intensity == 0){
            light1.GetSpotLight().intensity = intensity;
            light1.GetSpotLight().color = lightColor;    
        }
        else if(light2.GetSpotLight().intensity == 0){
            light2.GetSpotLight().intensity = intensity;
            light2.GetSpotLight().color = lightColor;    
        }
        else if(light3.GetSpotLight().intensity == 0){
            light3.GetSpotLight().intensity = intensity;
            light3.GetSpotLight().color = lightColor;    
        }
        else if(light4.GetSpotLight().intensity == 0){
            light4.GetSpotLight().intensity = intensity;
            light4.GetSpotLight().color = lightColor;    
        }

    }
    void TurnOffLights(){
    
        light1.GetSpotLight().intensity = 0;
        light2.GetSpotLight().intensity = 0;
        light3.GetSpotLight().intensity = 0;
        light4.GetSpotLight().intensity = 0;

    }

    void ReceiveMessage(Entity @sender, int signal) {
        
        if(signal < 4)
            AddToSlot(signal, sender);
        else
            RemoveFromSlot(signal - 4);
            
    }

    void AddToSlot(int slot, Entity@ fuse) {
    
        switch(slot){
        
            case 0: {
                @slot1 = @fuse;
                break;
            }
            case 1: {
                @slot2 = @fuse;
                break;
            }
            case 2: {
                @slot3 = @fuse;
                break;
            }
            case 3: {
                @slot4 = @fuse;
                break;
            }
        
        }
    
    }
    void RemoveFromSlot(int slot){
    
        switch(slot){
        
            case 0: {
                @slot1 = null;
                break;
            }
            case 1: {
                @slot2 = null;
                break;
            }
            case 2: {
                @slot3 = null;
                break;
            }
            case 3: {
                @slot4 = null;
                break;
            }
        
        }
    
    }
    
}
