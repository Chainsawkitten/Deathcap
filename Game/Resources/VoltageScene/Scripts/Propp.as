class Propp {
    Hub @hub;
    Entity @self;
    Entity @rightCtrl;
    Entity @originalParent;
    Entity @mastermind;
    Entity @slot1Location = null;
    Entity @slot2Location = null;
    Entity @slot3Location = null;
    Entity @slot4Location = null;
    bool isPressed = false;
    int hoverSlot = -1;
    int slot = -1;
    vec3 startPosition;

    Propp(Entity @entity) {
        @hub = Managers();
        @self = @entity;
        @rightCtrl = GetEntityByGUID(1508919758);
        @originalParent = self.GetParent();
        @mastermind = GetEntityByGUID(1511791235);
        @slot1Location = GetEntityByGUID(1511860851);
        @slot2Location = GetEntityByGUID(1511860862);
        @slot3Location = GetEntityByGUID(1511860874);
        @slot4Location = GetEntityByGUID(1511860887);

        startPosition = self.position;

        RegisterUpdate();
    }

    void Update(float deltaTime) {
        if (!Input(Trigger, rightCtrl) && isPressed) {
            isPressed = false;
            if (hoverSlot > 0) {
                vec3 tempPos = self.GetWorldPosition();
                switch (hoverSlot) {
                    case 1: {
                        self.SetParent(slot1Location);
                        break;
                    }
                    case 2: {
                        self.SetParent(slot2Location);
                        break;
                    }
                    case 3: {
                        self.SetParent(slot3Location);
                        break;
                    }
                    case 4: {
                        self.SetParent(slot4Location);
                        break;
                    }
                }
                SendMessage(mastermind, hoverSlot - 1);
                slot = hoverSlot - 1;
            } else {
                ReturnToStartPosition();
            }
        }

        hoverSlot = -1;
    }

    void ReceiveMessage(Entity @sender, int signal) {
        if (signal == 0 && !isPressed) {
            ReturnToStartPosition();
        }
    }

    void ReturnToStartPosition() {
        self.SetParent(originalParent);
        self.position = startPosition;
    }

    void PickupTrigger() {
        if (Input(Trigger, rightCtrl) && rightCtrl.GetChildFromIndex(1) is null && !isPressed) {
            isPressed = true;
            self.position = vec3(0.0f, 0.0f, 0.0f);
            self.SetParent(rightCtrl);

            if (slot >= 0) {
                // Inform mastermind that we removed from a slot
                SendMessage(mastermind, slot + 4);
            }

            slot = -1;
        }
    }

    void HoverImpl(int slot) {
        hoverSlot = slot;
    }

    void HoverSlot1() {
        HoverImpl(1);
    }

    void HoverSlot2() {
        HoverImpl(2);
    }

    void HoverSlot3() {
        HoverImpl(3);
    }

    void HoverSlot4() {
        HoverImpl(4);
    }
}
