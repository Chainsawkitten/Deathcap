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

	
	
    }
		
	bool puzzleSolved;
	int correctCount;
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
