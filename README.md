
## VoodooPS2FocalTech

VoodooPS2FocalTech is kernel extension for FocalTech Touchpad found in Haier Y11C Notebook (ACPI device name FTE0001). VoodooPS2FocalTech support up to 4 fingers and Multi-Finger  gestures. 

* Note: pressing Fn + F7  disable the Touchpad device, but most of the times when device is enabled it is out of sync and doesn't work anymore (reboot required). To resync press F7 key (after you enabling touchpad with Fn + F7)

## Installation
* Download [VoodooPS2Controller](https://github.com/acidanthera/VoodooPS2) (version 2.0.4 or higher required)
* Remove VoodooPS2Mouse and VoodooPS2Trackpad kernel extensions from `VoodooPS2Controller.kext/Contents/PlugIns/`
* Copy VoodooPS2FocalTech to `VoodooPS2Controller.kext/Contents/PlugIns/`
* Finally Install modified VoodooPS2Controller to `/Library/Extensions/` and Reboot
  
## Supported Gestures

VoodooPS2FocalTech use VoodooI2C's Native Gesture Engine, that implement Magic Trackpad 2 simulation. All Gesture listed in System Preferences Trackpad Pane are supported (except Force Touch)

For details check [VoodooI2C Documentation](https://voodooi2c.github.io/#Supported%20Gestures/Supported%20Gestures)

## Credits

* Seth Forshee – [Touchpad Protocol Reverse Engineering](http://www.forshee.me/2011/11/18/touchpad-protocol-reverse-engineering.html)
* Alexandred, ben9923 & others – [VoodooI2C](https://github.com/alexandred/VoodooI2C)
* RehabMan, turbo, mackerintel, nhand42, phb, Chunnan, jape, bumby, Dense, acidanthera, kprinssu & others – [VoodooPS2Contoller](https://github.com/acidanthera/VoodooPS2)
