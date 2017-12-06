class SwitcherScript_Voltage {
    Entity @self;
    Entity @elevatorPuzzle;
    Entity @puzzleVoltageScript;

    SwitcherScript_Voltage(Entity @entity){
        @self = @entity;
        @elevatorPuzzle = GetEntityByGUID(1511874401);
        @puzzleVoltageScript = GetEntityByGUID(1511528317);
    }

    void SwitchPuzzleFinal() {
        elevatorPuzzle.SetEnabled(false, true);
        puzzleVoltageScript.SetEnabled(true, true);
    }
}
