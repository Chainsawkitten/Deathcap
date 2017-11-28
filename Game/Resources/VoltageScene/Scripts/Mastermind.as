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

		if(correct > 0)
			light1.GetSpotLight().intensity = 100;
		else
			light1.GetSpotLight().intensity = 0;
			
		if(correct > 1)
			light2.GetSpotLight().intensity = 100;
		else
			light2.GetSpotLight().intensity = 0;
			
		if(correct > 2)
			light3.GetSpotLight().intensity = 100;
		else
			light3.GetSpotLight().intensity = 0;
			
		if(correct > 3)
			light4.GetSpotLight().intensity = 100;
		else
			light4.GetSpotLight().intensity = 0;
		
		for(int i = 0; i < rightColors; i++){
		
			if(light1.GetSpotLight().intensity == 0)
				light1.GetSpotLight().intensity = 50;
			else if(light2.GetSpotLight().intensity == 0)
				light2.GetSpotLight().intensity = 50;
			else if(light3.GetSpotLight().intensity == 0)
				light3.GetSpotLight().intensity = 50;
			else if(light4.GetSpotLight().intensity == 0)
				light4.GetSpotLight().intensity = 50;
				
		}
		
		if(puzzleSolved){
		
			light1.GetSpotLight().intensity = 200;
			light2.GetSpotLight().intensity = 200;
			light3.GetSpotLight().intensity = 200;
			light4.GetSpotLight().intensity = 200;
		
		}
			
    }
		
	bool puzzleSolved = false;
	int correct = 0;
	int rightColors = 0;
	void CheckSolution(){
	
		correct = 0;
		rightColors = 0;
		puzzleSolved = false;
		
		if(slot1.GetChildFromIndex(0) != null && slot2.GetChildFromIndex(0) != null && slot3.GetChildFromIndex(0) != null && slot4.GetChildFromIndex(0) != null){
		
			string trans1 = slot1.GetChildFromIndex(0).name;
			string trans2 = slot2.GetChildFromIndex(0).name;
			string trans3 = slot3.GetChildFromIndex(0).name;
			string trans4 = slot4.GetChildFromIndex(0).name;
			
			if(trans1 == "Green")
				correct = correct + 1;
			else if(trans1 == "Red" || trans1 == "Yellow" || trans1 == "Purple")
				rightColors = rightColors + 1;
				
			if(trans2 == "Red")
				correct = correct + 1;
			else if(trans2 == "Green" || trans2 == "Yellow" || trans2 == "Purple")
				rightColors = rightColors + 1;
				
			if(trans1 == "Yellow")
				correct = correct + 1;
			else if(trans1 == "Red" || trans1 == "Green" || trans1 == "Purple")
				rightColors = rightColors + 1;
				
			if(trans1 == "Purple")
				correct = correct + 1;
			else if(trans1 == "Red" || trans1 == "Yellow" || trans1 == "Green")
				rightColors = rightColors + 1;
				
			if(correct == 4)
				puzzleSolved = true;
		
		}
		


	}
	
}
