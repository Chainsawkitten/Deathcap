class SwitcherScript_1 {
    Hub @hub;
    Entity @self;
    Entity @puzzle0Cart;
    Entity @puzzle0Script;
    Entity @puzzle1Cart;
    Entity @puzzle1Script;

    SwitcherScript_1(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @puzzle0Cart = GetEntityByGUID(1508919163);
        @puzzle0Script = GetEntityByGUID(1511352103);
        @puzzle1Cart = GetEntityByGUID(1508919163);
        @puzzle1Script = GetEntityByGUID(1510920517);

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    void SwitchPuzzle0() {
        if (puzzle0Cart.GetParent().GetUniqueIdentifier() == 1508399850) {
            puzzle0Cart.SetEnabled(false, true);
            puzzle0Script.SetEnabled(false, true);
        }
        if (puzzle1Cart.GetParent().GetUniqueIdentifier() == 1510049192) {
            puzzle1Cart.SetEnabled(true, true);
            puzzle1Script.SetEnabled(true, true);
        }
            
    }
}
