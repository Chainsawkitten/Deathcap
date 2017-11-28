class Mastermind {

    Hub @hub;
    Entity @self;

	Entity @slot1;
	Entity @slot2;
	Entity @slot3;
	Entity @slot4;
	
	Entity @light1;
	Entity @light2;
	Entity @light3;
	Entity @light4;
	
    Mastermind(Entity @entity){
        @hub = Managers();
        @self = @entity;
		
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

		CheckSolution();
        
		for(int i = 0; i < correct; i++){
		
            TurnOnLight(100, vec3(0,255,0));
				
		}
		for(int i = 0; i < rightColors; i++){
		
            TurnOnLight(100, vec3(255,0,0));
				
		}
		
		if(puzzleSolved){
		
		
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

		if(true){//slot1.GetChildFromIndex(0) != null && slot2.GetChildFromIndex(0) != null && slot3.GetChildFromIndex(0) != null && slot4.GetChildFromIndex(0) != null){
		
			string trans1 = "Green";//slot1.GetChildFromIndex(0).name;
			string trans2 = "Red";//slot2.GetChildFromIndex(0).name;
			string trans3 = "Yellow";//slot3.GetChildFromIndex(0).name;
			string trans4 = "Purple";//slot4.GetChildFromIndex(0).name;
			
			if(trans1 == "Green")
				correct = correct + 1;
			else if(trans1 == "Red" || trans1 == "Yellow" || trans1 == "Purple")
				rightColors = rightColors + 1;
				
			if(trans2 == "Red")
				correct = correct + 1;
			else if(trans2 == "Green" || trans2 == "Yellow" || trans2 == "Purple")
				rightColors = rightColors + 1;
				
			if(trans3 == "Yellow")
				correct = correct + 1;
			else if(trans3 == "Red" || trans3 == "Green" || trans3 == "Purple")
				rightColors = rightColors + 1;
				
			if(trans4 == "Purple")
				correct = correct + 1;
			else if(trans4 == "Red" || trans4 == "Yellow" || trans4 == "Green")
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
    
}
