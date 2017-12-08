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
    Entity @ParticleSystemLocation = null;
    
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
        if (self.name == "BlackPropp") {
            @ParticleSystemLocation = self.GetChild("BlackSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "BluePropp") {
            @ParticleSystemLocation = self.GetChild("BlueSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "BrownPropp") {
            @ParticleSystemLocation = self.GetChild("BrownSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "PinkPropp") {
            @ParticleSystemLocation = self.GetChild("PinkSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "RedPropp") {
            @ParticleSystemLocation = self.GetChild("RedSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "WhitePropp") {
            @ParticleSystemLocation = self.GetChild("WhiteSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "YellowPropp") {
            @ParticleSystemLocation = self.GetChild("YellowSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        } else if (self.name == "GreenPropp") {
            @ParticleSystemLocation = self.GetChild("GreenSparks");
            ParticleSystemLocation.SetEnabled(false, true);
        }
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
                        ParticleSystemLocation.SetEnabled(true, true);
                        self.SetParent(slot1Location);
                        break;
                    }
                    case 2: {
                        ParticleSystemLocation.SetEnabled(true, true);
                        self.SetParent(slot2Location);
                        break;
                    }
                    case 3: {
                        ParticleSystemLocation.SetEnabled(true, true);
                        self.SetParent(slot3Location);
                        break;
                    }
                    case 4: {
                        ParticleSystemLocation.SetEnabled(true, true);
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
