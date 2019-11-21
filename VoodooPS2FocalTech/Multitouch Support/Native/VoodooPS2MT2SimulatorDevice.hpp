//
//  VoodooI2CMT2SimulatorDevice.hpp
//  VoodooI2C
//
//  Created by Alexandre on 10/02/2018.
//  Copyright © 2018 Alexandre Daoud and Kishor Prins. All rights reserved.
//

#ifndef VoodooI2CMT2SimulatorDevice_hpp
#define VoodooI2CMT2SimulatorDevice_hpp

#include <IOKit/IOLib.h>
#include <IOKit/IOKitKeys.h>
#include "LegacyIOService.h"

#include "../../Library/LegacyIOHIDDevice.h"

#include <kern/clock.h>

#include "../Dependencies/helpers.hpp"
#include "../MultitouchHelpers.hpp"
#include "../VoodooPS2DigitiserTransducer.hpp"
#include "../VoodooPS2MultitouchInterface.hpp"

#define MT2_MAX_X 7612
#define MT2_MAX_Y 5065

struct __attribute__((__packed__)) MAGIC_TRACKPAD_INPUT_REPORT_FINGER {
    UInt8 AbsX;
    UInt8 AbsXY;
    UInt8 AbsY[2];
    UInt8 Touch_Major;
    UInt8 Touch_Minor;
    UInt8 Size;
    UInt8 Pressure;
    UInt8 Orientation_Origin;
};

#define MAX_FINGER_COUNT 4

struct __attribute__((__packed__)) MAGIC_TRACKPAD_INPUT_REPORT {
    UInt8 ReportID;
    UInt8 Button;
    UInt8 Unused[5];
    
    UInt8 TouchActive;
    
    UInt8 multitouch_report_id;
    UInt8 timestamp_buffer[3];
    
    MAGIC_TRACKPAD_INPUT_REPORT_FINGER FINGERS[MAX_FINGER_COUNT]; //May support more fingers
};

class VoodooPS2NativeEngine;

#define EXPORT __attribute__((visibility("default")))

class EXPORT VoodooPS2MT2SimulatorDevice : public IOHIDDevice {
    OSDeclareDefaultStructors(VoodooPS2MT2SimulatorDevice);
    
public:
    void constructReport(VoodooI2CMultitouchEvent multitouch_event, AbsoluteTime timestamp);
    IOReturn setReport(IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options) override;
    
    static bool getMultitouchPreferences(void* target, void* ref_con, IOService* multitouch_device, IONotifier* notifier);
    
    IOReturn getReport(IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options) override;
    virtual IOReturn newReportDescriptor(IOMemoryDescriptor** descriptor) const override;
    virtual OSNumber* newVendorIDNumber() const override;
    
    
    virtual OSNumber* newProductIDNumber() const override;
    
    
    virtual OSNumber* newVersionNumber() const override;
    
    
    virtual OSString* newTransportString() const override;
    
    
    virtual OSString* newManufacturerString() const override;
    
    virtual OSNumber* newPrimaryUsageNumber() const override;
    
    virtual OSNumber* newPrimaryUsagePageNumber() const override;
    
    virtual OSString* newProductString() const override;
    
    virtual OSString* newSerialNumberString() const override;
    
    virtual OSNumber* newLocationIDNumber() const override;
    
    IOReturn setPowerState(unsigned long whichState, IOService* whatDevice) override;
    
    bool start(IOService* provider) override;
    
    void stop(IOService* provider) override;
    
    void releaseResources();
protected:
private:
    bool ready_for_reports = false;
    VoodooPS2NativeEngine* engine;
    AbsoluteTime start_timestamp;
    OSData* new_get_report_buffer = nullptr;
    UInt16 stashed_unknown[15];
    UInt8 touch_state[15];
    UInt8 new_touch_state[15];
    IOWorkLoop* work_loop;
    IOCommandGate* command_gate;
    MAGIC_TRACKPAD_INPUT_REPORT input_report;

    void constructReportGated(VoodooI2CMultitouchEvent& multitouch_event, AbsoluteTime& timestamp);
};


#endif /* VoodooI2CMT2SimulatorDevice_hpp */
