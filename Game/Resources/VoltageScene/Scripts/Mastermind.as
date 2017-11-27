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

		if(correctCount > 0)
			light1.GetPointLight().intensity = 100;
		else
			light1.GetPointLight().intensity = 0;
			
		if(correctCount > 1)
			light2.GetPointLight().intensity = 100;
		else
			light2.GetPointLight().intensity = 0;
			
		if(correctCount > 2)
			light3.GetPointLight().intensity = 100;
		else
			light3.GetPointLight().intensity = 0;
			
		if(correctCount > 3)
			light4.GetPointLight().intensity = 100;
		else
			light4.GetPointLight().intensity = 0;
			
		if(puzzleSolved){
		
			light1.GetPointLight().intensity = 200;
			light2.GetPointLight().intensity = 200;
			light3.GetPointLight().intensity = 200;
			light4.GetPointLight().intensity = 200;
		
		}
			
    }
		
	bool puzzleSolved = false;
	int correctCount = 0;
	void CheckSolution(){
	
		correctCount = 0;
		puzzleSolved = false;
	
		if(slot1.GetChild("Green") != null)
			correctCount = correctCount + 1;
		if(slot2.GetChild("Red") != null)
			correctCount = correctCount + 1;
		if(slot3.GetChild("Yellow") != null)
			correctCount = correctCount + 1;
		if(slot4.GetChild("Purple") != null)
			correctCount = correctCount + 1;
			
		if(correctCount == 4)
			puzzleSolved = true;

	}
	
}
