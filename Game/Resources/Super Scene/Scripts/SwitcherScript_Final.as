class SwitcherScript_Final {
    Entity @self;
    Entity @puzzle1Script;
    Entity @puzzleFinalScript;

    SwitcherScript_Final(Entity @entity){
        @self = @entity;
        @puzzle1Script = GetEntityByGUID(1510920517);
        @puzzleFinalScript = GetEntityByGUID(1511260476);
    }

    void SwitchPuzzleFinal() {
        puzzle1Script.SetEnabled(false, true);
        puzzleFinalScript.SetEnabled(true, true);
    }
}
