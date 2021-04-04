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
        
    def shutdown(self):
        pass
        
    @property
    def connected(self):
        return self.connection is not None
        
    def connect(self):
        if(self.connected):
            return True
            
        list_all_ports()
            
        excluded_devices = []
        
        while True:
            potential_device = find(excluded_devices)
            
            if potential_device is None:
                break
                
            try:
                print(f"Attempting to connect to {potential_device}")
                self.connection = serial.Serial(potential_device)
                break
            except serial.serialutil.SerialException as e:
                print(f"Could not connect: {e}")
                excluded_devices.append(potential_device)
                self.connection = None
        
        return self.connected
            
    def disconnect(self):
        if self.connection is not None:
            self.connection.close()
            self.connection = None
            
    def write(self, buffer):
        if(self.connected):
            data = buffer.bytes(with_reportid = False)
            
            print("Sending data via serial:")
            [print(hex(b), end=" ") for b in data]
            written = self.connection.write(data)
            
            return written