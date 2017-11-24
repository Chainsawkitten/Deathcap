class StressScript {
    Hub @hub;
    Entity @self;
	Entity @lantern;
	Entity @audioSource;
	Entity @alien;
	
	float voulumeAmplification;
	float lightAmplification;
	
	float minDistance;
	float maxDistance;
	
	private float startVolume;
	private float startIntensity;
	
    StressScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
		
        // Remove this if updates are not desired.
        RegisterUpdate();
    }

	bool first = true;
    // Called by the engine for each frame.
    void Update(float deltaTime) {
		if(first){
		
			startVolume = audioSource.GetSoundSource().volume;
			startIntensity = lantern.GetPointLight().intensity;
			first = false;
		
		}
		float distanceToAlien = 0;
		if(distanceToAlien < maxDistance){
		
			audioSource.GetSoundSource().volume = startVolume + (startVolume * voulumeAmplification * (1 - distanceToAlien / maxDistance));
			lantern.GetPointLight().intensity = startIntensity + (startIntensity * lightAmplification * (1 - distanceToAlien / maxDistance));
		
		}
		
    }
}
