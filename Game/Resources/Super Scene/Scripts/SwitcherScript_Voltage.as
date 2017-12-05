class SwitcherScript_Voltage {
    Entity @self;
    Entity @puzzle1Script;
    Entity @puzzleVoltageScript;

    SwitcherScript_Voltage(Entity @entity){
        @self = @entity;
        @puzzle1Script = GetEntityByGUID(1510920517);
        @puzzleVoltageScript = GetEntityByGUID(1511528317);
    }

    void SwitchPuzzleFinal() {
        puzzle1Script.SetEnabled(false, true);
        puzzleVoltageScript.SetEnabled(true, true);
    }
}
