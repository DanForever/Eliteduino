import eliteduino
import msvcrt

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

import myNotebook as nb
def plugin_prefs(parent, cmdr, is_beta):
    """
    Return a TK Frame for adding to the EDMC settings dialog.
    """
    frame = nb.Frame(parent)

    global eliteduino_instance
    nb.Button(frame, text="Send dummy data", command=eliteduino_instance.dummy_values).grid()

    return frame

def journal_entry(cmdr, is_beta, system, station, entry, state) -> None:
    print("")
    print("Eliteduino journal_entrys")
    data = { "cmdr" : cmdr, "system" : system, "station" : station, "entry" : entry, "state" : state }
    
    global eliteduino_instance
    eliteduino_instance.update(data)