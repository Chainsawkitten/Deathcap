class SwitcherScript_Final {
    Entity @self;
    Entity @puzzleVoltageScript;
    Entity @puzzleFinalScript;

    SwitcherScript_Final(Entity @entity){
        @self = @entity;
        @puzzleVoltageScript = GetEntityByGUID(1511528317);
        @puzzleFinalScript = GetEntityByGUID(1511260476);
    }

    void SwitchPuzzleFinal() {
        puzzleVoltageScript.SetEnabled(false, true);
        puzzleFinalScript.SetEnabled(true, true);
    }
}
