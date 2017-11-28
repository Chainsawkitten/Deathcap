class LightsOutScript {
    Hub @hub;
    Entity @self;
    Entity @board;
    array<Entity@> buttons(25);
    array<bool> buttonStates(25);
    int numPressedButtons = 0;
    bool gameWon = false;

    LightsOutScript(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @board = GetEntityByGUID(1511530025);
        
        for (int row = 0; row < 5; ++row) {
            for (int column = 0; column < 5; ++column) {
                Entity @btn = board.GetChild("btn-" + row + "-" + column);
                @buttons[row * 5 + column] = btn.GetChild("btn-" + row + "-" + column).GetChild("button");
                buttonStates[row * 5 + column] = false;
            }
        }

        Toggle(0);
    }

    void Toggle(int index) {
        bool pressed = !buttonStates[index];
        buttonStates[index] = pressed;

        if (pressed) {
            buttons[index].position.x = -0.06f;
            numPressedButtons += 1;
        } else {
            buttons[index].position.x = 0.0f;
            numPressedButtons -= 1;
        }
    }

    bool IsValid(int index) {
        return index >= 0 && index <= 24;
    }

    // Index goes first row 0 -> 4, second row 5 -> 9 etc.
    void ButtonPress(int index) {
        if (gameWon) {
            return;
        }

        print("Pressed button nr `" + index + "`\n");

        int left = index - 1;
        if (IsValid(left)) {
            Toggle(left);
        }

        int right = index + 1;
        if (IsValid(right)) {
            Toggle(right);
        }

        int up = index - 5;
        if (IsValid(up)) {
            Toggle(up);
        }

        int down = index + 5;
        if (IsValid(down)) {
            Toggle(down);
        }

        Toggle(index);

        if (numPressedButtons == 25) {
            gameWon = true;
            //SendMessage(somewhere);
        }
    }

    void b_0_0() {
        ButtonPress(0);
    }

    void b_0_1() {
        ButtonPress(1);
    }

    void b_0_2() {
        ButtonPress(2);
    }

    void b_0_3() {
        ButtonPress(3);
    }

    void b_0_4() {
        ButtonPress(4);
    }

    void b_1_0() {
        ButtonPress(5);
    }

    void b_1_1() {
        ButtonPress(6);
    }

    void b_1_2() {
        ButtonPress(7);
    }

    void b_1_3() {
        ButtonPress(8);
    }

    void b_1_4() {
        ButtonPress(9);
    }

    void b_2_0() {
        ButtonPress(10);
    }

    void b_2_1() {
        ButtonPress(11);
    }

    void b_2_2() {
        ButtonPress(12);
    }

    void b_2_3() {
        ButtonPress(13);
    }

    void b_2_4() {
        ButtonPress(14);
    }

    void b_3_0() {
        ButtonPress(15);
    }

    void b_3_1() {
        ButtonPress(16);
    }

    void b_3_2() {
        ButtonPress(17);
    }

    void b_3_3() {
        ButtonPress(18);
    }

    void b_3_4() {
        ButtonPress(19);
    }

    void b_4_0() {
        ButtonPress(20);
    }

    void b_4_1() {
        ButtonPress(21);
    }

    void b_4_2() {
        ButtonPress(22);
    }

    void b_4_3() {
        ButtonPress(23);
    }

    void b_4_4() {
        ButtonPress(24);
    }
}
