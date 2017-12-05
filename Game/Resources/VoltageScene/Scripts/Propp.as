class Propp {
    Hub @hub;
    Entity @self;
    Entity @rightCtrl;
    Entity @originalParent;
    //Entity @S1T1;
    Entity @mastermind;
    bool isPressed;
    //vec3 worldPos;
    int hoverSlot = -1;
    int slot = -1;
    vec3 startPosition;

    Propp(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @rightCtrl = GetEntityByGUID(1508919758);
        @originalParent = self.GetParent();
        //@S1T1 = GetEntityByGUID(1905121);
        @mastermind = GetEntityByGUID(1511791235);
        
        //worldPos = S1T1.GetWorldPosition();
        startPosition = self.position;
        isPressed = false;

        RegisterUpdate();
    }

    void Update(float deltaTime) {
        if (!Input(Trigger, rightCtrl) && isPressed) {
            isPressed = false;
            if (hoverSlot > 0) {
                vec3 tempPos = self.GetWorldPosition();
                self.SetParent(originalParent);
                self.SetWorldPosition(tempPos);
                SendMessage(mastermind, hoverSlot - 1);
                slot = hoverSlot - 1;
            } else {
                ReturnToStartPosition();
            }
        }

        hoverSlot = -1;
    }

    void ReceiveMessage(Entity @sender, int signal) {
        if (signal == 0) {
            ReturnToStartPosition();
        }
    }

    void ReturnToStartPosition() {
        self.SetParent(originalParent);
        self.position = startPosition;
    }

    void PickupTrigger() {
        if (Input(Trigger, rightCtrl) && rightCtrl.GetChildFromIndex(1) == null && !isPressed) {
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
