class SwitcherScript_Final {
    Hub @hub;
    Entity @self;
    Entity @puzzle1Script; //This will need to be updated for every new puzzle that we put in.
    Entity @finalScript;

    SwitcherScript_Final(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @puzzle1Script = GetEntityByGUID(1510920517);
        @finalScript = GetEntityByGUID(1511260476);
    }

    void SwitchToFinalScene() {
        puzzle1Script.SetEnabled(false, true);
        finalScript.SetEnabled(true, true);
    }
}
