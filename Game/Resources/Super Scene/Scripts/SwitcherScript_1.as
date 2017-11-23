class SwitcherScript_1 {
    Hub @hub;
    Entity @self;
    Entity @puzzle0Script;
    Entity @puzzle1Script;

    SwitcherScript_1(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @puzzle0Script = GetEntityByGUID(1511352103);
        @puzzle1Script = GetEntityByGUID(1510920517);
    }

    void SwitchPuzzle0() {
        puzzle0Script.SetEnabled(false, true);
        puzzle1Script.SetEnabled(true, true);
    }
}
