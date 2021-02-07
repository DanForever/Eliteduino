from pyhid import pyhid
from enum import IntEnum, auto

class UnsupportedDataType(Exception):
    pass

HID_BUFFER_SIZE = 64

REPORT_ID_BYTE_POSITION = 1
STAT_TYPE_BYTE_POSITION = 2
STAT_DATA_POSITION = 3

class ReportType(IntEnum):
    DATA = 1
    CONTROLS_REBIND = 2

class StatType(IntEnum):
    CMDR_NAME = 1
    SYSTEM = auto()
    STATION = auto()
    STATION_TYPE = auto()

class StationType(IntEnum):
    UNKNOWN_STATION_TYPE = -1
    CORIOLIS = 1
    OCELLUS = auto()
    ORBIS = auto()
    OUTPOST = auto()
    MEGA_SHIP = auto()
    ASTEROID = auto()
    SURFACE_PORT = auto()
    COUNT = auto()

StationTypeMap = \
{
    "Coriolis" : StationType.CORIOLIS,
    "Ocellus" : StationType.OCELLUS,
    "Orbis" : StationType.ORBIS,
    "Outpost" : StationType.OUTPOST,
    "FleetCarrier" : StationType.MEGA_SHIP,
    "AsteroidStation" : StationType.ASTEROID,
    "CraterOutpost" : StationType.SURFACE_PORT
}

class StatBuffer(pyhid.Buffer):
    def __init__(self):
        super(StatBuffer, self).__init__(HID_BUFFER_SIZE)
        self.data_length = 0
    
    def _set_report_id(self, value : ReportType):
        self[REPORT_ID_BYTE_POSITION] = int(value)
    
    def _set_stat_type(self, value : StatType):
        self[STAT_TYPE_BYTE_POSITION] = int(value)
        
    def _set_string_data(self, value):
        byte_data = bytes(value, "utf-8")
        i = STAT_DATA_POSITION
        for byte in byte_data:
            self[i] = byte
            i = i + 1
            
            if(i == self.size):
                break
                
        # Make the size equal to the length of the data, clamped to the size of the buffer
        
        sizeOfBytesWithNullTerminator = len(byte_data) + 1
        sizeOfBufferMinusHeader = self.size - STAT_DATA_POSITION
        
        self.data_length = min(sizeOfBytesWithNullTerminator, sizeOfBufferMinusHeader)
        self[self.data_length + STAT_DATA_POSITION - 1] = 0
        
        #print("Buffer size: " + str(self.size))
        #print("Data length:" + str(self.data_length))
        #print("Byte array length: " + str(sizeOfBytesWithNullTerminator))
        
    def _set_int_data(self, value):
        # For now we assume that the value fits into a single byte
        self[STAT_DATA_POSITION] = value
        
    @property
    def data_start(self):
        return STAT_DATA_POSITION
        
    @property
    def data_end(self):
        return self.data_start + self.data_length
        
    # Override len() so that we return the amount of the buffer used, rather than the full buffer each time
    def __len__(self):
        return self.data_end
    
    def _set_data(self, value):
        if(isinstance(value, str)):
            self._set_string_data(value)
        elif(isinstance(value, int)):
            self._set_int_data(value)
        else:
            raise UnsupportedDataType("Unsupported data type: {type(value)} ({value})")
            
    report_id = property(fset=_set_report_id)
    stat_type = property(fset=_set_stat_type)
    data = property(fset=_set_data)

class Eliteduino():
    def __init__(self):
        self.hid = pyhid.Hid()
        self.hid.init()
        
        self.device_path = None
        self.device = None
        
        self.data = {}
        self.buffer = StatBuffer()
        
        self.build_vicinity_map()
        
    def shutdown(self):
        self.hid.shutdown()
    
    def find_device_path(self):
        with self.hid.enumerate() as enumeration:
            for deviceInfo in enumeration:
                if(deviceInfo.vendor_id == 0x16c0 and deviceInfo.usage_page==0xffab):
                    self.device_path = deviceInfo.path
                    break
    
    @property
    def connected(self):
        return self.device is not None and self.device.connected
        
    def connect(self):
        if(self.connected):
            return True
        
        # First we need to check to see if we are connected to the eliteduino
        if(self.device_path is None):
            self.find_device_path()
            if(self.device_path is None):
                return False
                
        if(self.device is None):
            self.device = self.hid.device(self.device_path)
        
        self.device.try_connect()
        
        return self.device.connected
        
    def update_stat(self, stat_type, value, force = False):
        print(">>> Update Stat <<<")
        if(value is None and not force):
            print(f"Not sending {stat_type.name} as value is None")
            return
            
        if(self.data.get(stat_type) is value and not force):
            print(f"Not sending {stat_type.name} as value is the same as was sent previously")
            return
        
        [print(hex(b), end=" ") for b in self.buffer._instance]
        print("")
        self.data[stat_type] = value
        
        print(f"Sending report type: {ReportType.DATA.value} / Stat type: {stat_type.name} / {value}")
        
        self.buffer.report_id = ReportType.DATA
        self.buffer.stat_type = stat_type
        self.buffer.data = value
        
        if(self.connect()):
            self.device.write(self.buffer)
        
        [print(hex(b), end=" ") for b in self.buffer._instance]
        print("")
        print("<<< Update Stat >>>")
        
    def update_vicinity_internal(self, description, stationType = StationType.UNKNOWN_STATION_TYPE):
            self.update_stat(StatType.STATION, description)
            self.update_stat(StatType.STATION_TYPE, stationType)
        
    def build_vicinity_map(self):
        
        self.vicinity_description_map = \
        {
            "Supercruise" : "Supercruise",
            "Hyperspace" : "Hyperspace",
            "SupercruiseEntry" : "Supercruise",
        }
        
        # Some aliases to try and make these following lines a bit more readable
        updater = self.update_vicinity_internal
        updater_map = self.vicinity_description_map
        
        self.vicinity_map = \
        {
            "StartJump" : lambda entry: updater(updater_map[entry["JumpType"]]),
            "FSDJump" : lambda entry: updater(updater_map["SupercruiseEntry"]),
            "SupercruiseEntry" : lambda entry: updater(updater_map["SupercruiseEntry"]),
            "SupercruiseExit" : lambda entry: updater(f"Near {entry['Body']}"),
            "Undocked" : lambda entry: updater(f"Near {entry['StationName']}"),
            "Docked" : lambda entry: updater(entry['StationName'], StationTypeMap.get(entry["StationType"], StationType.UNKNOWN_STATION_TYPE)),
            "Startup" : lambda entry: updater(entry.get('StationName', ''), StationTypeMap.get(entry.get("StationType"), StationType.UNKNOWN_STATION_TYPE)),
        }
        
    def update_vicinity(self, data):
        
        entry = data["entry"]
        
        updater = self.vicinity_map.get(entry["event"])
        
        if(updater is not None):
            updater(entry)
        
    def update(self, data):
        
        # some quick and dirty logging to collect a whole bunch of updates
        import os
        desktop = os.path.join(os.path.join(os.environ['USERPROFILE']), 'Desktop')
        dumpfile_path = os.path.join(desktop,"edmc-journal_entry.txt")
        try:
            with open(dumpfile_path, "a+") as dumpfile:
                dumpfile.write(str(data))
                dumpfile.write("\n")
        except EnvironmentError as e:
            print(f"there was an error opening '{dumpfile_path}': {e}")
        
        self.update_stat(StatType.CMDR_NAME, data.get("cmdr"))
        self.update_stat(StatType.SYSTEM, data.get("system"))
        self.update_vicinity(data)
        
    def dummy_values(self):
        if not hasattr(self, 'dummyStationType'):
            self.dummyStationType = 0
        self.dummyStationType = self.dummyStationType + 1
        if(self.dummyStationType >= StationType.COUNT):
            self.dummyStationType = 1
        
        print("Sending dummy values!")
        self.update_stat(StatType.CMDR_NAME, "Dummy commander", True)
        self.update_stat(StatType.SYSTEM, "Dummy system", True)
        self.update_stat(StatType.STATION, "Dummy station", True)
        self.update_stat(StatType.STATION_TYPE, self.dummyStationType, True)