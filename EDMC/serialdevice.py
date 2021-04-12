import serial

potential_device_vids = [ 0x1b4f ]

def list_all_ports():
    import serial.tools.list_ports
    ports = serial.tools.list_ports.comports()
    
    print("Available serial ports:")
    for port in ports:
        print(f"{port} - {port.vid}")

def find(excluded_devices = []):
    import serial.tools.list_ports
    ports = serial.tools.list_ports.comports()
    
    for port in ports:
        #if port.vid in potential_device_vids and port.device not in excluded_devices:
        if port.device not in excluded_devices:
            return port.device
            
    return None


class Device():
    def __init__(self):
        self.connection = None
        self._id = None
        
    def shutdown(self):
        pass
        
    @property
    def id(self):
        return self._id
        
    @property
    def connected(self):
        return self.connection is not None
        
    def connect(self, excluded_devices = []):
        if(self.connected):
            return True
        
        while True:
            potential_device = find(excluded_devices)
            
            if potential_device is None:
                break
                
            try:
                self.connection = serial.Serial(potential_device)
                self._id = potential_device
                break
            except serial.serialutil.SerialException as e:
                excluded_devices.append(potential_device)
                self.connection = None
        
        return self.connected
            
    def disconnect(self):
        if self.connection is not None:
            self.connection.close()
            self.connection = None
            self._id = None
            
    def write(self, buffer):
        if(self.connected):
            try:
                data = buffer.bytes(with_reportid = False)
                written = self.connection.write(data)
                return written
                
            except serial.serialutil.SerialException:
                # For the sake of our sanity, just assume that we have been disconnected
                print(f"Lost connection to {self._id}")
                self.disconnect()
                return 0