//
//  VoodooI2CNativeEngine.cpp
//  VoodooI2C
//
//  Created by Alexandre on 10/02/2018.
//  Copyright © 2018 Alexandre Daoud and Kishor Prins. All rights reserved.
//

#include "VoodooPS2NativeEngine.hpp"

#define super VoodooPS2MultitouchEngine
OSDefineMetaClassAndStructors(VoodooPS2NativeEngine, VoodooPS2MultitouchEngine);

MultitouchReturn VoodooPS2NativeEngine::handleInterruptReport(VoodooI2CMultitouchEvent event, AbsoluteTime timestamp) {
    if (!voodooInputInstance) {
        return MultitouchReturnContinue;
    }

    message.timestamp = timestamp;
    message.contact_count = event.contact_count;
    memset(message.transducers, 0, VOODOO_INPUT_MAX_TRANSDUCERS * sizeof(VoodooInputTransducer));
    
    VoodooPS2DigitiserTransducer* transducer = (VoodooPS2DigitiserTransducer*) event.transducers->getObject(0);

    if (!transducer)
        return MultitouchReturnBreak;
    
    if (transducer->type == kDigitiserTransducerStylus)
        stylus_check = 1;

    for (int i = 0; i < event.contact_count; i++) {
        VoodooPS2DigitiserTransducer* transducer = (VoodooPS2DigitiserTransducer*) event.transducers->getObject(i+stylus_check);
        VoodooInputTransducer* inputTransducer = &message.transducers[i];
        
        if (!transducer) {
            continue;
        }
        
        inputTransducer->fingerType = (MT2FingerType) (kMT2FingerTypeIndexFinger + (i % 4));
        inputTransducer->secondaryId = transducer->secondary_id;
        
        inputTransducer->type = (transducer->type == DigitiserTransducerType::kDigitiserTransducerFinger) ? VoodooInputTransducerType::FINGER : VoodooInputTransducerType::STYLUS;
        
        inputTransducer->isValid = transducer->is_valid;
        inputTransducer->isTransducerActive = transducer->tip_switch.value();
        inputTransducer->isPhysicalButtonDown = transducer->physical_button.value();
        
        inputTransducer->currentCoordinates.x = transducer->coordinates.x.value();
        inputTransducer->previousCoordinates.x = transducer->coordinates.x.last.value;
        
        inputTransducer->currentCoordinates.y = transducer->coordinates.y.value();
        inputTransducer->previousCoordinates.y = transducer->coordinates.y.last.value;
        inputTransducer->supportsPressure = false;
        inputTransducer->timestamp = timestamp;

        // TODO: does VoodooI2C know width(s)? how does it measure pressure?
        inputTransducer->currentCoordinates.width = transducer->tip_pressure.value() / 2;
        inputTransducer->previousCoordinates.width = transducer->tip_pressure.last.value / 2;

        inputTransducer->currentCoordinates.pressure = transducer->tip_pressure.value();
        inputTransducer->previousCoordinates.pressure = transducer->tip_pressure.last.value;

        // Force Touch emulation
        // The button state is saved in the first transducer
        if (((VoodooPS2DigitiserTransducer*) event.transducers->getObject(0))->physical_button.value() && isForceClickEnabled()) {
            inputTransducer->supportsPressure = true;
            inputTransducer->isPhysicalButtonDown = 0x0;
            inputTransducer->currentCoordinates.pressure = 0xff;
            inputTransducer->currentCoordinates.width = 10;
        }
    }
    
    // set the thumb to improve 4F pinch and spread gesture and cross-screen dragging
    if (event.contact_count >= 4 || transducer->physical_button.value()) {
        // simple thumb detection: to find the lowest finger touch in the vertical direction.
        UInt32 y_max = 0;
        int thumb_index = 0;
        for (int i = 0; i < event.contact_count; i++) {
            VoodooInputTransducer* inputTransducer = &message.transducers[i];
            if (inputTransducer->isValid && inputTransducer->currentCoordinates.y >= y_max) {
                y_max = inputTransducer->currentCoordinates.y;
                thumb_index = i;
            }
        }
        message.transducers[thumb_index].fingerType = kMT2FingerTypeThumb;
    }
        
    super::messageClient(kIOMessageVoodooInputMessage, voodooInputInstance, &message, sizeof(VoodooInputEvent));
    
    return MultitouchReturnBreak;
}

bool VoodooPS2NativeEngine::start(IOService* provider) {
    if (!super::start(provider))
        return false;

    parentProvider = OSDynamicCast(VoodooPS2MultitouchInterface, provider);
    
    if (!parentProvider) {
        return false;
    }

    voodooInputInstance = NULL;
    
    setProperty(VOODOO_INPUT_LOGICAL_MAX_X_KEY, parentProvider->logical_max_x, 32);
    setProperty(VOODOO_INPUT_LOGICAL_MAX_Y_KEY, parentProvider->logical_max_y, 32);
    
    setProperty(VOODOO_INPUT_PHYSICAL_MAX_X_KEY, parentProvider->physical_max_x * 10, 32);
    setProperty(VOODOO_INPUT_PHYSICAL_MAX_Y_KEY, parentProvider->physical_max_y * 10, 32);
    
    setProperty(kIOFBTransformKey, 0ull, 32);
    setProperty("VoodooInputSupported", kOSBooleanTrue);
    
    stylus_check = 0;

    return true;
}

bool VoodooPS2NativeEngine::isForceClickEnabled() {
    AbsoluteTime now_abs;
    uint64_t diff_ns;
    clock_get_uptime(&now_abs);
    absolutetime_to_nanoseconds(now_abs - lastForceClickPropertyUpdateTime, &diff_ns);

    if (diff_ns < 1e9) {
        lastForceClickPropertyUpdateTime = now_abs;
        return lastIsForceClickEnabled;
    }

    // Blocking reading here takes about 10 microseconds
    OSDictionary* dict = OSDynamicCast(OSDictionary, this->getProperty("MultitouchPreferences", gIOServicePlane, kIORegistryIterateRecursively));
    if (!dict) {
        return lastIsForceClickEnabled;
    }
    if (OSCollectionIterator* i = OSCollectionIterator::withCollection(dict)) {
        while (OSSymbol* key = OSDynamicCast(OSSymbol, i->getNextObject())) {
            // System -> Preferences -> Trackpad -> Force Click and haptic feedback
            // ForceSuppressed
            if (key->isEqualTo("ForceSuppressed")) {
                OSBoolean* value = OSDynamicCast(OSBoolean, dict->getObject(key));

                if (value != NULL) {
                    lastIsForceClickEnabled = !value->getValue();
                }
            }
        }

        i->release();
    }

    lastForceClickPropertyUpdateTime = now_abs;
    return lastIsForceClickEnabled;
}

void VoodooPS2NativeEngine::stop(IOService* provider) {
    super::stop(provider);
}

bool VoodooPS2NativeEngine::handleOpen(IOService *forClient, IOOptionBits options, void *arg) {
    if (forClient && forClient->getProperty(VOODOO_INPUT_IDENTIFIER)) {
        voodooInputInstance = forClient;
        voodooInputInstance->retain();
        
        return true;
    }
    return false;
}

bool VoodooPS2NativeEngine::handleIsOpen(const IOService *forClient) const {
    return voodooInputInstance != NULL && forClient == voodooInputInstance;
}

void VoodooPS2NativeEngine::handleClose(IOService *forClient, IOOptionBits options) {
    if (voodooInputInstance && forClient == voodooInputInstance) {
        OSSafeReleaseNULL(voodooInputInstance);
    }
}
