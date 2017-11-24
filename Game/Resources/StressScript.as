class StressScript {
    Hub @hub;
    Entity @self;
	Entity @lantern;
	Entity @audioSource;
	Entity @alien;
	
	float voulumeAmplification;
	float lightAmplification;
	
	float maxDistance;
	
	private float startVolume;
	private float startIntensity;
	
    StressScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
		@lantern = GetEntityByGUID(1508402678);
		@audioSource = GetEntityByGUID(1509010103);
		@alien = GetEntityByGUID(1510736518);
		
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
		float distanceToAlien = sqrt(pow(self.position.x - alien.position.x, 2) + pow(self.position.y - alien.position.y, 2) + pow(self.position.z - alien.position.z, 2));
		float mDist = maxDistance;
		if(distanceToAlien < maxDistance){
		
			float newVolume = startVolume + (startVolume * voulumeAmplification * (1 - distanceToAlien / maxDistance));
			audioSource.GetSoundSource().volume = newVolume;
			
			float newIntensity = startIntensity + (startIntensity * lightAmplification * (1 - distanceToAlien / maxDistance));
			lantern.GetPointLight().intensity = newIntensity;
		
		} 
		
    }
}
