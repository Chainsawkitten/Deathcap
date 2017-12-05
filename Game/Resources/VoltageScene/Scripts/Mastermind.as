class Mastermind {
    Hub @hub;
    Entity @self;

    Entity@ slot1 = null;
    Entity@ slot2 = null;
    Entity@ slot3 = null;
    Entity@ slot4 = null;

    Entity@ slot1Key;
    Entity@ slot2Key;
    Entity@ slot3Key;
    Entity@ slot4Key;

    Entity @light1;
    Entity @light2;
    Entity @light3;
    Entity @light4;

    Entity @flickeringLights;
    Entity @roundabout;

    bool puzzleSolved = false;
    int correct = 0;
    int rightColors = 0;

    Mastermind(Entity @entity) {
        @hub = Managers();
        @self = @entity;
        @slot1Key = GetEntityByGUID(1511964701); // GreenPropp
        @slot2Key = GetEntityByGUID(1511964823); // RedPropp
        @slot3Key = GetEntityByGUID(1511964915); // YellowPropp
        @slot4Key = GetEntityByGUID(1511964754); // PinkPropp

        // Remove this if updates are not desired.
        RegisterUpdate();
    }

    // Called by the engine for each frame.
    void Update(float deltaTime) {
        if (puzzleSolved)
            return;

        CheckSolution();

        for (int i = 0; i < correct; i++) {
            TurnOnLight(100, vec3(0, 255, 0));
        }

        for (int i = 0; i < rightColors; i++) {
            TurnOnLight(100, vec3(255, 0, 0));
        }
    }

    void CheckSolution() {
        correct = 0;
        rightColors = 0;
        puzzleSolved = false;
        TurnOffLights();

        if (@slot1 != null && @slot2 != null && @slot3 != null && @slot4 != null) {
            if (slot1 is slot1Key)
                correct = correct + 1;
            else if (slot1 is slot2Key || slot1 is slot3Key || slot1 is slot4Key)
                rightColors = rightColors + 1;

            if (slot2 is slot2Key)
                correct = correct + 1;
            else if (slot2 is slot1Key || slot2 is slot3Key || slot2 is slot4Key)
                rightColors = rightColors + 1;

            if (slot3 is slot3Key)
                correct = correct + 1;
            else if (slot3 is slot1Key || slot3 is slot2Key || slot3 is slot4Key)
                rightColors = rightColors + 1;

            if (slot4 is slot4Key)
                correct = correct + 1;
            else if (slot4 is slot1Key || slot4 is slot2Key || slot4 is slot3Key)
                rightColors = rightColors + 1;

            if (correct == 4) {
                puzzleSolved = true;
                SendMessage(roundabout, 2);
            }
        }
    }

    void TurnOnLight(int intensity, vec3 lightColor) {
        if (light1.GetSpotLight().intensity == 0) {
            light1.GetSpotLight().intensity = intensity;
            light1.GetSpotLight().color = lightColor;
        }
        else if (light2.GetSpotLight().intensity == 0) {
            light2.GetSpotLight().intensity = intensity;
            light2.GetSpotLight().color = lightColor;
        }
        else if (light3.GetSpotLight().intensity == 0) {
            light3.GetSpotLight().intensity = intensity;
            light3.GetSpotLight().color = lightColor;
        }
        else if (light4.GetSpotLight().intensity == 0) {
            light4.GetSpotLight().intensity = intensity;
            light4.GetSpotLight().color = lightColor;
        }
    }

    void TurnOffLights() {
        light1.GetSpotLight().intensity = 0;
        light2.GetSpotLight().intensity = 0;
        light3.GetSpotLight().intensity = 0;
        light4.GetSpotLight().intensity = 0;
    }

    void ReceiveMessage(Entity @sender, int signal) {
        if (signal < 4)
            AddToSlot(signal, sender);
        else
            RemoveFromSlot(signal - 4);
    }

    void AddToSlot(int slot, Entity@ fuse) {
        // First remove any existing fuse in this slot.
        RemoveFromSlot(slot);

        switch(slot) {
            case 0: {
                @slot1 = @fuse;
                break;
            }
            case 1: {
                @slot2 = @fuse;
                break;
            }
            case 2: {
                @slot3 = @fuse;
                break;
            }
            case 3: {
                @slot4 = @fuse;
                break;
            }
        }
    }

    void RemoveFromSlot(int slot) {
        switch(slot) {
            case 0: {
                if (@slot1 != null) {
                    SendMessage(slot1, 0);
                }
                @slot1 = null;
                break;
            }
            case 1: {
                if (@slot2 != null) {
                    SendMessage(slot2, 0);
                }
                @slot2 = null;
                break;
            }
            case 2: {
                if (@slot3 != null) {
                    SendMessage(slot3, 0);
                }
                @slot3 = null;
                break;
            }
            case 3: {
                if (@slot4 != null) {
                    SendMessage(slot4, 0);
                }
                @slot4 = null;
                break;
            }
        }
    }
}
