class Mastermind {

    Hub @hub;
    Entity @self;

	string slot1;
	string slot2;
	string slot3;
	string slot4;
	
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

		if(slot1 != "" && slot2 != "" && slot3 != "" && slot4 != ""){
			
			if(slot1 == "Green")
				correct = correct + 1;
			else if(slot1 == "Red" || slot1 == "Yellow" || slot1 == "Purple")
				rightColors = rightColors + 1;
				
			if(slot2 == "Red")
				correct = correct + 1;
			else if(slot2 == "Green" || slot2 == "Yellow" || slot2 == "Purple")
				rightColors = rightColors + 1;
				
			if(slot3 == "Yellow")
				correct = correct + 1;
			else if(slot3 == "Red" || slot3 == "Green" || slot3 == "Purple")
				rightColors = rightColors + 1;
				
			if(slot4 == "Purple")
				correct = correct + 1;
			else if(slot4 == "Red" || slot4 == "Yellow" || slot4 == "Green")
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
            AddToSlot(signal, sender.name);
        else
            RemoveFromSlot(signal - 4);
            
    }

    void AddToSlot(int slot, string transistorColor){
    
        switch(slot){
        
            case 0: {
                slot1 = transistorColor;
                break;
            }
            case 1: {
                slot2 = transistorColor;
                break;
            }
            case 2: {
                slot3 = transistorColor;
                break;
            }
            case 3: {
                slot4 = transistorColor;
                break;
            }
        
        }
    
    }
    void RemoveFromSlot(int slot){
    
        switch(slot){
        
            case 0: {
                slot1 = "";
                break;
            }
            case 1: {
                slot2 = "";
                break;
            }
            case 2: {
                slot3 = "";
                break;
            }
            case 3: {
                slot4 = "";
                break;
            }
        
        }
    
    }
    
}
