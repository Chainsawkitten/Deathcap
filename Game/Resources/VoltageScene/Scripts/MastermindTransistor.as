class MastermindTransistor {
    Hub @hub;
    Entity @self;
    Entity @mastermind;
    
    string transistorColor;
    int colorCount = 8;
    
    MastermindTransistor(Entity @entity){
        @hub = Managers();
        @self = @entity;

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {

    }
    
    void PutInSlot1(){
    
        int base = 0;
        SendMessage(mastermind, base + GetColorID);
    
    }
    void PutInSlot2(){
    
        int base = colorCount;
        SendMessage(mastermind, base + GetColorID);
    
    }
    void PutInSlot3(){
    
        int base = colorCount * 2;
        SendMessage(mastermind, base + GetColorID);
    
    }
    void PutInSlot4(){
    
        int base = colorCount * 3;
        SendMessage(mastermind, base + GetColorID);
    
    }
    
    void RemoveFromSlot1(){
        SendMessage(mastermind, 32 + 0);
    }
    void RemoveFromSlot2(){
        SendMessage(mastermind, 32 + 1);
    }
    void RemoveFromSlot3(){
        SendMessage(mastermind, 32 + 2);
    }
    void RemoveFromSlot4(){
        SendMessage(mastermind, 32 + 3);
    }
    
    int GetColorID(){
    
        switch(colorID){
        
            case "red":{
                return 0;
            }
            case "green":{
                return 1;
            }
            case "blue":{
                return 2;
            }
            case "black":{
                return 3;
            }
            case "white":{
                return 4;
            }
            case "yellow":{
                return 5;
            }
            case "orange":{
                return 6;
            }
            case "brown":{
                return 7;
            }
        
        }
    
    }
    
}
