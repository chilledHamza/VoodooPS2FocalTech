//
//  VoodooI2CMultitouchEngine.hpp
//  VoodooI2C
//
//  Created by Alexandre on 22/09/2017.
//  Copyright © 2017 Alexandre Daoud. All rights reserved.
//

#ifndef VoodooPS2MultitouchEngine_hpp
#define VoodooPS2MultitouchEngine_hpp

#include <IOKit/IOLib.h>
#include <IOKit/IOKitKeys.h>
#include <IOKit/IOService.h>

#include "MultitouchHelpers.hpp"

class VoodooPS2MultitouchInterface;

/* Base class that all mutltitouch engines should inherit from */

class EXPORT VoodooPS2MultitouchEngine : public IOService {
  OSDeclareDefaultStructors(VoodooPS2MultitouchEngine);

 public:
    VoodooPS2MultitouchInterface* interface;

    /* Intended to be overwritten by an inherited class to set the engine's priority
     *
     * @return The engine's score
     */

    virtual UInt8 getScore();

    /* Intended to be overwritten by an inherited class to handle a multitouch event
     * @event The event to be handled
     * @timestamp The event's timestamp
     *
     * @return *MultitouchContinue* if the next engine in line should also be allowed to process the event, *MultitouchBreak* if this is the last engine that should be allowed to process the event
     */

    virtual MultitouchReturn handleInterruptReport(VoodooI2CMultitouchEvent event, AbsoluteTime timestamp);

    bool willTerminate(IOService* provider, IOOptionBits options) override;

    /* Sets up the multitouch engine
     * @provider The <VoodooI2CMultitouchInterface> that we have matched against
     *
     * This function is intended to be overwritten by an inherited class but should still be called at the beginning of the overwritten
     * function.
     * @return *true* upon successful start, *false* otherwise
     */

    virtual bool start(IOService* provider);
};


#endif /* VoodooPS2MultitouchEngine_hpp */
