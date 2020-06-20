/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.2 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _APPLEPS2FOCALTECHTOUCHPAD_H
#define _APPLEPS2FOCALTECHTOUCHPAD_H

#include "VoodooPS2Controller/ApplePS2MouseDevice.h"
#include "Multitouch Support/VoodooPS2MultitouchInterface.hpp"
#include "LegacyIOHIPointing.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ApplePS2ALPSGlidePoint Class Declaration
//

#define FOCALTECH_MAX_FINGERS 4

#define kPacketLengthSmall  8
#define kPacketLengthLarge  16
#define kPacketLengthMax    16

#define kGetProductId       0xA7
#define kSetDeviceMode      0xEA
#define kDeviceModeAdvanced 0xED
#define kDeviceModeDefault  0xEE

#define LOGICAL_MAX_X       0x08E0
#define LOGICAL_MAX_Y       0x03E0
#define PHYSCICAL_MAX_X     0x0352
#define PHYSCICAL_MAX_Y     0x0173

struct focaltech_hw_state {
    int x;
    int y;
    bool valid;
};

typedef struct FTE_BYTES
{
    UInt8 byte0;
    UInt8 byte1;
    UInt8 byte2;
} FTE_BYTES_t;

class EXPORT ApplePS2FocalTechTouchPad : public IOHIPointing
{
    typedef IOHIPointing super;
    OSDeclareDefaultStructors(ApplePS2FocalTechTouchPad);
    
private:
    ApplePS2MouseDevice * _device;
    RingBuffer<UInt8,kPacketLengthMax*32> _ringBuffer;
    UInt32                _packetByteCount;
    uint64_t              keytime;
    uint64_t              maxaftertyping;
    int                   _fingerCount;
    bool                  _interruptHandlerInstalled;
    bool                  _powerControlHandlerInstalled;
    unsigned int          left  : 1;
    unsigned int          right : 1;
    FTE_BYTES_t           bytes;
    UInt8                 _isReadNext;
    UInt8                 _lastDeviceData[16];
    OSArray*              transducers;
    VoodooPS2MultitouchInterface* mt_interface;
    
    struct focaltech_hw_state fingerStates[FOCALTECH_MAX_FINGERS];
    
    bool publish_multitouch_interface();
    void unpublish_multitouch_interface();
    bool init_multitouch_interface();
    void sendTouchDataToMultiTouchInterface();
    
protected:
    virtual void   doHardwareReset();
    virtual void   switchProtocol();
    virtual void   getProductID(FTE_BYTES_t *bytes);
    virtual void   packetReady();
    virtual void   parsePacket(UInt8 *packet);
    virtual void   setTouchPadEnable( bool enable );
    virtual void   setDevicePowerState(UInt32 whatToDo);
    virtual PS2InterruptResult interruptOccurred(UInt8 data);
    
protected:
public:
    bool init( OSDictionary * properties ) override;
    ApplePS2FocalTechTouchPad * probe( IOService * provider, SInt32 *    score ) override;
    
    bool start( IOService * provider ) override;
    void stop( IOService * provider ) override;
    
    UInt32 deviceType() override;
    UInt32 interfaceID() override;
    
    virtual IOReturn message(UInt32 type, IOService* provider, void* argument) override;
};

#endif /* _APPLEPS2FOCALTECHTOUCHPAD_H */
