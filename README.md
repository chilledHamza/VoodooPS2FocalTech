
## VoodooPS2FocalTech

VoodooPS2FocalTech is kernel extension for FocalTech Touchpad found in Haier Y11C Notebook (ACPI device name FTE0001). VoodooPS2FocalTech support up to 4 fingers with Multi-touch gestures. 

* Note: pressing Fn + F7  disable/enable Touchpad device, but mostly when device is re-enabled it is "out of sync". To resync press F7 key (device must be enabled for resync to work)

## Installation
* Download [VoodooPS2Controller](https://github.com/acidanthera/VoodooPS2/releases) (v2.2.5 or above)
* Disable VoodooPS2Mouse, VoodooPS2Trackpad and VoodooInput PlugIns in `/EFI/OC/config.plist`
* Copy VoodooPS2FocalTech to /`EFI/OC/Kexts/`
* Add VoodooPS2FocalTech in `config.plist under Kernel -> Add` after VoodooPS2Controller entry
* Save config.plist and Reboot
  
## Supported Gestures

VoodooPS2FocalTech use VoodooI2C's Native Gesture Engine, that implement Magic Trackpad 2 simulation. All Gesture listed in System Preferences Trackpad Pane are supported (except Force Touch)

For details check [VoodooI2C Documentation](https://voodooi2c.github.io/#Supported%20Gestures/Supported%20Gestures)

## Credits

* Seth Forshee – [Touchpad Protocol Reverse Engineering](http://www.forshee.me/2011/11/18/touchpad-protocol-reverse-engineering.html)
* Alexandred, ben9923 & others – [VoodooI2C](https://github.com/alexandred/VoodooI2C)
* RehabMan, turbo, mackerintel, nhand42, phb, Chunnan, jape, bumby, Dense, acidanthera, kprinssu & others – [VoodooPS2Contoller](https://github.com/acidanthera/VoodooPS2)
