class LightsOutScript {
    Entity @board;
    Entity @rightController;
    Entity @elevatorCartScript;
    Entity @puzzleBoard;
    Component::SoundSource @buttonSound;
    dictionary buttons;
    dictionary buttonStates;
    int numPressedButtons = 0;
    bool gameInactive = true;
    bool isPressed = false;

    LightsOutScript(Entity @entity){
        @board = GetEntityByGUID(1511530025);
        @rightController = GetEntityByGUID(1508919758);
        @puzzleBoard = GetEntityByGUID(1512029307);
        @buttonSound = puzzleBoard.GetSoundSource();
        
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                string key = row * 4 + column;
                Entity @btn = board.GetChild("btn-" + row + "-" + column);
                @buttons[key] = btn.GetChild("btn-" + row + "-" + column).GetChild("button");
                buttonStates[key] = false;
            }
        }

        RegisterUpdate();
    }

    void ReceiveMessage(Entity @sender, int message) {
        if (message == 0) {
            gameInactive = false;
            @elevatorCartScript = @sender;
        }
        if (message == 1) {
        	gameInactive = true;
        }
    }

    void Update(float deltaTime) {
        if (isPressed && !Input(Trigger, rightController)) {
            isPressed = false;
        }
    }

    void Toggle(int index) {
        buttonSound.Play();
        bool pressed = !bool(buttonStates["" + index]);
        buttonStates["" + index] = pressed;

        Entity@ btn;
        if (pressed) {
            buttons.get("" + index, @btn);
            btn.position.x = -0.06f;
            numPressedButtons += 1;
        } else {
            buttons.get("" + index, @btn);
            btn.position.x = 0.0f;
            numPressedButtons -= 1;
        }
    }

    bool IsValid(int index) {
        return index >= 0 && index <= 15;
    }

    void ResetBoard() {
        Entity @btn;
        if (Input(Trigger, rightController)) {
            for (int x = 0; x < 16; ++x) {
                buttonStates["" + x] = false;
                buttons.get("" + x, @btn);
                btn.position.x = 0.0f;
                numPressedButtons = 0;
            }
        }
    }

    // Index goes first row 0 -> 3, second row 4 -> 7 etc.
    void ButtonPress(int index) {
        if (gameInactive) {
            return;
        }

        if (!isPressed && Input(Trigger, rightController)) {
            isPressed = true;

            int left = index - 1;
            if (index % 4 != 0 && IsValid(left)) {
                Toggle(left);
            }

            int right = index + 1;
            if (index % 4 != 3 && IsValid(right)) {
                Toggle(right);
            }

            int up = index - 4;
            if (index > 3 && IsValid(up)) {
                Toggle(up);
            }

            int down = index + 4;
            if (index < 12 && IsValid(down)) {
                Toggle(down);
            }

            Toggle(index);

            if (numPressedButtons == 16) {
                gameInactive = true;
                SendMessage(elevatorCartScript, 0);
            }
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

    void b_1_0() {
        ButtonPress(4);
    }

    void b_1_1() {
        ButtonPress(5);
    }

    void b_1_2() {
        ButtonPress(6);
    }

    void b_1_3() {
        ButtonPress(7);
    }

    void b_2_0() {
        ButtonPress(8);
    }

    void b_2_1() {
        ButtonPress(9);
    }

    void b_2_2() {
        ButtonPress(10);
    }

    void b_2_3() {
        ButtonPress(11);
    }

    void b_3_0() {
        ButtonPress(12);
    }

    void b_3_1() {
        ButtonPress(13);
    }

    void b_3_2() {
        ButtonPress(14);
    }

    void b_3_3() {
        ButtonPress(15);
    }
}
