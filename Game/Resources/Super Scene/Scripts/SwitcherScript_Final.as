class SwitcherScript_Final {
    Hub @hub;
    Entity @self;
    Entity @puzzle1Script;
    Entity @puzzleFinalScript;

    SwitcherScript_Final(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @puzzle1Script = GetEntityByGUID(1510920517);
        @puzzleFinalScript = GetEntityByGUID(1511788813);
    }

    void SwitchPuzzleFinal() {
        puzzle1Script.SetEnabled(false, true);
        puzzleFinalScript.SetEnabled(true, true);
    }
}
