import eliteduino
import msvcrt
from defines import StatType

def plugin_start3(plugin_dir: str) -> str:
    """
    Load this plugin into EDMC
    """

    print("Eliteduino initializing")
    
    global eliteduino_instance
    eliteduino_instance = eliteduino.Eliteduino()

    return "Eliteduino"

def plugin_stop() -> None:
    """
    EDMC is closing
    """
    
    print("Eliteduino shutting down")
    
    global eliteduino_instance
    eliteduino_instance.shutdown()

def plugin_prefs(parent, cmdr, is_beta):
    """
    Return a TK Frame for adding to the EDMC settings dialog.
    """
    import myNotebook as nb
    frame = nb.Frame(parent)

    global eliteduino_instance
    nb.Button(frame, text="Send dummy data", command=eliteduino_instance.dummy_values).grid()
    nb.Button(frame, text="Set Mass locked", command=dummy_set_masslock).grid()
    nb.Button(frame, text="Clear Mass locked", command=dummy_clear_masslock).grid()

    return frame

def dummy_set_masslock():
    global eliteduino_instance
    eliteduino_instance.update_stat(StatType.MASS_LOCKED, True, True)
    
def dummy_clear_masslock():
    global eliteduino_instance
    eliteduino_instance.update_stat(StatType.MASS_LOCKED, False, True)
    
def debug_write_journal_entry_to_log(entry):
    # some quick and dirty logging to collect a whole bunch of updates
    import os
    desktop = os.path.join(os.path.join(os.environ['USERPROFILE']), 'Desktop')
    dumpfile_path = os.path.join(desktop,"edmc-journal_entry.txt")
    try:
        with open(dumpfile_path, "a+") as dumpfile:
            dumpfile.write(str(entry))
            dumpfile.write("\n")
    except EnvironmentError as e:
        print(f"there was an error opening '{dumpfile_path}': {e}")

def journal_entry(cmdr, is_beta, system, station, entry, state) -> None:
    global eliteduino_instance
    eliteduino_instance.update_stat(StatType.CMDR_NAME, cmdr)
    eliteduino_instance.update_stat(StatType.SYSTEM, system)
    eliteduino_instance.update_stat(StatType.STATION, station)
    eliteduino_instance.update_vicinity(entry)
    
    debug_write_journal_entry_to_log(entry)

def dashboard_entry(cmdr, is_beta, entry):
    fsd_charging       = bool(entry.get("Flags") & 0x00020000)
    fsd_cooldown       = bool(entry.get("Flags") & 0x00040000)
    masslocked         = bool(entry.get("Flags") & 0x00010000)
    overheating        = bool(entry.get("Flags") & 0x00100000)
    being_interdicted  = bool(entry.get("Flags") & 0x00800000)
    hardpoints         = bool(entry.get("Flags") & 0x00000040)
    
    global eliteduino_instance
    eliteduino_instance.update_stat(StatType.FSD_CHARGING, fsd_charging)
    eliteduino_instance.update_stat(StatType.FSD_COOLDOWN, fsd_cooldown)
    eliteduino_instance.update_stat(StatType.MASS_LOCKED, masslocked)
    eliteduino_instance.update_stat(StatType.OVERHEATING, overheating)
    eliteduino_instance.update_stat(StatType.BEING_INTERDICTED, being_interdicted)
    eliteduino_instance.update_stat(StatType.HARDPOINTS_DEPLOYED, hardpoints)
