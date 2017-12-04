class Propp {
    Hub @hub;
    Entity @self;
    Entity @rightCtrl;
    Entity @originalParent;
    Entity @S1T1;
    Entity @mastermind;
    bool isPressed;
    vec3 worldPos;

    Propp(Entity @entity){
        @hub = Managers();
        @self = @entity;
        @rightCtrl = GetEntityByGUID(1508919758);
        @originalParent = self.GetParent();
        @S1T1 = GetEntityByGUID(1905121);
        @mastermind = GetEntityByGUID(1511791235);
        
        worldPos = S1T1.GetWorldPosition();
        isPressed = false;
    }

    void PickupTrigger() {
        if (Input(Trigger, rightCtrl) && rightCtrl.GetChildFromIndex(1) == null &&!isPressed) {
            isPressed = true;
            self.position = vec3(0.0f, 0.0f, 0.0f);
            self.SetParent(rightCtrl);
        }
    }

    void HoverImpl(int slot) {
	    print("Fuse `" + self.name + "` hovering slot " + slot + "\n");
        if (!Input(Trigger, rightCtrl) && isPressed) {
            isPressed = false;
            vec3 tempPos = self.GetWorldPosition();
            self.SetParent(originalParent);
            self.SetWorldPosition(tempPos);
            SendMessage(mastermind, slot - 1);
        }
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
