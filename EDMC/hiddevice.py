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
    
    def find_device_path(self):
        with self.hid.enumerate() as enumeration:
            for deviceInfo in enumeration:
                print("Found hid device vid: " + hex(deviceInfo.vendor_id) + ", usage: " + hex(deviceInfo.usage_page))
                if(deviceInfo.vendor_id == 0x16c0 and deviceInfo.usage_page==0xffab):
                    self.device_path = deviceInfo.path
                elif(deviceInfo.vendor_id == 0x1b4f and deviceInfo.usage_page==0xffc0):
                    self.device_path = deviceInfo.path
                elif(deviceInfo.vendor_id == 0x1b4f and deviceInfo.usage_page==0xffab):
                    self.device_path = deviceInfo.path
                    break
    
    @property
    def connected(self):
        return self._device is not None and self._device.connected
        
    def connect(self):
        if self.connected:
            return True
        
        excluded_devices = []
        
        while True:
            potential_device = self.find(excluded_devices)
            
            if potential_device is None:
                return False
            
            self._device = self._hid.device(potential_device.path)
            
            print(f"Attempting to connect to device '{self._device}' over hid...")
            if self._device.try_connect():
                return True
            else:
                print("Connection failed")
                excluded_devices.append(potential_device)
        
    def disconnect(self):
        if self.connected:
            self._device.disconnect()
        
    def write(self, buffer):
        self._device.write(buffer)