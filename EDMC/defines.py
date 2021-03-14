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
    FSD_CHARGING = auto()
    FSD_COOLDOWN = auto()
    MASS_LOCKED = auto()
    OVERHEATING = auto()
    BEING_INTERDICTED = auto()
    HARDPOINTS_DEPLOYED = auto()

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
