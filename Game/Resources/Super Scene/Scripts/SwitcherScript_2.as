class SwitcherScript_2 {
    Entity @self;
    Entity @bridgePuzzle;
    Entity @elevatorPuzzle;

    SwitcherScript_2(Entity @entity){
        @self = @entity;
        @bridgePuzzle = GetEntityByGUID(1510920517);
        @elevatorPuzzle = GetEntityByGUID(1511874401);
    }

    void SwitchPuzzleFinal() {
        bridgePuzzle.SetEnabled(false, true);
        elevatorPuzzle.SetEnabled(true, true);
    }
}
