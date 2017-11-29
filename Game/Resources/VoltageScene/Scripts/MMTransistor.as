class MMTransistor {
    Hub @hub;
    Entity @self;
    Entity @mastermind;
    
    string transistorColor;
    int colorCount = 8;
    
    MMTransistor(Entity @entity){
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
        SendMessage(mastermind, base + GetColorID());
    
    }
    void PutInSlot2(){
    
        int base = colorCount;
        SendMessage(mastermind, base + GetColorID());
    
    }
    void PutInSlot3(){
    
        int base = colorCount * 2;
        SendMessage(mastermind, base + GetColorID());
    
    }
    void PutInSlot4(){
    
        int base = colorCount * 3;
        SendMessage(mastermind, base + GetColorID());
    
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
            
        if(transistorColor == "Red"){
            return 0;
        }
        else if(transistorColor == "Green"){
            return 1;
        }
        else if(transistorColor == "Blue"){
            return 2;
        }
        else if(transistorColor == "Black"){
            return 3;
        }
        else if(transistorColor == "White"){
            return 4;
        }
        else if(transistorColor == "Yellow"){
            return 5;
        }
        else if(transistorColor == "Orange"){
            return 6;
        }
        else if(transistorColor == "Brown"){
            return 7;
        }
         
        return -1;
         
    }
    
}
