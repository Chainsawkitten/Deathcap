class MMTransistor {
    Entity @mastermind;
    
    MMTransistor(Entity @entity){

    }
    
    void PutInSlot1(){
        SendMessage(mastermind, 0);
    }
    void PutInSlot2(){
        SendMessage(mastermind, 1);
    }
    void PutInSlot3(){
        SendMessage(mastermind, 2);
    }
    void PutInSlot4(){
        SendMessage(mastermind, 3);
    }
    
    void RemoveFromSlot1(){
        SendMessage(mastermind, 4);
    }
    void RemoveFromSlot2(){
        SendMessage(mastermind, 5);
    }
    void RemoveFromSlot3(){
        SendMessage(mastermind, 6);
    }
    void RemoveFromSlot4(){
        SendMessage(mastermind, 7);
    }
    
}
