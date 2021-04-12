from pyhid import pyhid
from typing import NamedTuple

class DeviceId(NamedTuple):
    vendor_id : int
    usage_page : int
    path : str = None
    
    def __eq__(self,other):
        return self.vendor_id == other.vendor_id and self.usage_page == other.usage_page

_Teensy = 0x16c0
_TeensyUsagePage = 0xffab

_ProMico = 0x1b4f
_ProMicroUsagePage = 0xffc0

potential_devices = \
[
    DeviceId( _Teensy, _TeensyUsagePage ),
    DeviceId( _ProMico, _ProMicroUsagePage ),
]

class Device():
    def __init__(self):
        self._hid = pyhid.Hid()
        self._hid.init()
        
        self._id = None
        self._device = None
        
    def shutdown(self):
        self._hid.shutdown()
    
    def find(self, excluded_devices = []):
        with self._hid.enumerate() as enumeration:
            for deviceInfo in enumeration:
                device = DeviceId( deviceInfo.vendor_id, deviceInfo.usage_page )
                
                if device in potential_devices and not device in excluded_devices:
                    return device._replace( path = deviceInfo.path )
                    
        return None
    
    @property
    def id(self):
        return self._id
    
    @property
    def connected(self):
        return self._device is not None and self._device.connected
        
    def connect(self, excluded_devices = []):
        if self.connected:
            return True
        
        while True:
            potential_device = self.find(excluded_devices)
            
            if potential_device is None:
                return False
            
            device = self._hid.device(potential_device.path)
            
            print(f"Attempting to connect to device '{device}' over hid...")
            if device.try_connect():
                self._device = device
                self._id = potential_device
                return True
            else:
                print("Connection failed")
                excluded_devices.append(potential_device)
        
    def disconnect(self):
        if self.connected:
            self._device.disconnect()
        
    def write(self, buffer):
        self._device.write(buffer)