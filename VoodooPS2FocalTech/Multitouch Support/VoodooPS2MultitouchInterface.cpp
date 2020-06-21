//
//  VoodooI2CMultitouchInterface.cpp
//  VoodooI2C
//
//  Created by Alexandre on 22/09/2017.
//  Copyright © 2017 Alexandre Daoud. All rights reserved.
//

#include "VoodooPS2MultitouchInterface.hpp"
#include "VoodooPS2MultitouchEngine.hpp"

#define super IOService
OSDefineMetaClassAndStructors(VoodooPS2MultitouchInterface, IOService);

void VoodooPS2MultitouchInterface::handleInterruptReport(VoodooI2CMultitouchEvent event, AbsoluteTime timestamp) {
    int i, count;
    VoodooPS2MultitouchEngine* engine;

    for (i = 0, count = engines->getCount(); i < count; i++) {
        engine = OSDynamicCast(VoodooPS2MultitouchEngine, engines->getObject(i));
        if (!engine)
            continue;

        if (engine->handleInterruptReport(event, timestamp) == MultitouchReturnBreak)
            break;
    }
}

bool VoodooPS2MultitouchInterface::handleOpen(IOService* forClient, IOOptionBits options, void* arg) {
    VoodooPS2MultitouchEngine* engine = OSDynamicCast(VoodooPS2MultitouchEngine, forClient);

    if (!engine)
        return false;

    engines->setObject(engine);

    return true;
}

void VoodooPS2MultitouchInterface::handleClose(IOService* forClient, IOOptionBits options) {
    VoodooPS2MultitouchEngine* engine = OSDynamicCast(VoodooPS2MultitouchEngine, forClient);

    if (engine)
        engines->removeObject(engine);
}

bool VoodooPS2MultitouchInterface::handleIsOpen(const IOService *forClient ) const {
    VoodooPS2MultitouchEngine* engine = OSDynamicCast(VoodooPS2MultitouchEngine, forClient);
    
    if (engine) {
        return engines->containsObject(engine);
    }
    
    return false;
}

SInt8 VoodooPS2MultitouchInterface::orderEngines(VoodooPS2MultitouchEngine* a, VoodooPS2MultitouchEngine* b) {
    if (a->getScore() > b->getScore())
        return 1;
    else if (a->getScore() < b->getScore())
        return -1;
    else
        return 0;
}

bool VoodooPS2MultitouchInterface::start(IOService* provider) {
    if (!super::start(provider))
        return false;

    engines = OSOrderedSet::withCapacity(1, (OSOrderedSet::OSOrderFunction)VoodooPS2MultitouchInterface::orderEngines);

    setProperty(kIOFBTransformKey, 0ull, 32);
    setProperty("VoodooI2CServices Supported", kOSBooleanTrue);

    return true;
}

void VoodooPS2MultitouchInterface::stop(IOService* provider) {
    OSSafeReleaseNULL(engines);

    super::stop(provider);
}
