class SwitcherScript_Final {
    Entity @self;
    Entity @elevatorPuzzle;
    Entity @puzzleFinalScript;

    SwitcherScript_Final(Entity @entity){
        @self = @entity;
        @elevatorPuzzle = GetEntityByGUID(1511874401);
        @puzzleFinalScript = GetEntityByGUID(1511260476);
    }

    void SwitchPuzzleFinal() {
        elevatorPuzzle.SetEnabled(false, true);
        puzzleFinalScript.SetEnabled(true, true);
    }
}
