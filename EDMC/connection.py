
class Connection():
    """
    A single connection to a single device
    """
    def __init__(self):
        self.device = None
        
    def shutdown(self):
        if self.device is not None:
            self.device.shutdown()
    
    @property
    def id(self):
        return self.device.id

    @property
    def connected(self):
        return self.device is not None and self.device.connected
    
    def connect(self, device_module_name, excluded_devices = []):
        if self.connected:
            return True
        
        module = __import__(device_module_name)
        
        device = module.Device()
        if(device.connect(excluded_devices)):
            self.device = device
            return True
            
        device.shutdown()
        
        return False
        
    def disconnect(self):
        if self.connected:
            self.device.disconnect()
        
    def write(self, buffer):
        self.device.write(buffer)

class ConnectionCollation():
    """
    A series of connections to devices using the same protocol
    """
    def __init__(self, device_module_name):
        self._connections = []
        self._excluded_devices = []
        self._device_module_name = device_module_name
        
    @property
    def connected_devices(self):
        return [connection.id for connection in self._connections]
    
    @property
    def excluded_devices(self):
        return [*self._excluded_devices, *self.connected_devices]
    
    def validate_connections(self):
        self._connections = [connection for connection in self._connections if connection.connected]
    
    @property
    def connected(self):
        return any(connection.connected for connection in self._connections)
    
    def connect(self):
        excluded = self.excluded_devices
        
        while True:
            connection = Connection()
            if connection.connect(self._device_module_name, excluded):
                self._connections.append(connection)
                excluded.append(connection.id)
            else:
                break
        
        self._excluded_devices = [device for device in excluded if device not in self.connected_devices]
        
        return self.connected
    
    def disconnect(self):
        for connection in self._connections:
            connection.disconnect()
        self._connections = []
        
    def write(self, buffer):
        for connection in self._connections:
            connection.write(buffer)
            
    def print_connections(self):
        print(f"Connections with {self._device_module_name}")
        for connection in self._connections:
            print(f"  - {connection.id}")
        
class Connector():
    """
    Presents an interface that allows for multiple connections at once,
    which allows for a user to have multiple devices connected at the same time
    """
    def __init__(self):
        self.connections = [ ConnectionCollation("serialdevice"), ConnectionCollation("hiddevice") ]
    
    def validate_connections(self):
        for collation in self.connections:
            collation.validate_connections()
    
    @property
    def connected(self):
        has_connection = any(collation.connected for collation in self.connections)
        print(f"Connector has connection: {has_connection}")
        return has_connection
    
    def connect(self):
        self.validate_connections()
        
        for collation in self.connections:
            collation.connect()
            
        return self.connected
            
    def disconnect(self):
        for collation in self.connections:
            collation.disconnect()
            
    def write(self, buffer):
        for collation in self.connections:
            collation.write(buffer)
            
    def print_connections(self):
        for collation in self.connections:
            collation.print_connections()