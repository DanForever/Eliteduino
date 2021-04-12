import eliteduino
import msvcrt
from defines import StatType

# There appears to be a bug in the edmc OptionMenu that means the first option is not selected
stat_type_options = [ "" ]
for stat_type in StatType:
    if stat_type >= StatType.FSD_CHARGING:
        stat_type_options.append(stat_type.name)

def default_journal_dump_filepath():
    import os
    desktop = os.path.join(os.path.join(os.environ['USERPROFILE']), 'Desktop')
    return os.path.join(desktop,"edmc-journal_entry.txt")

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
    
    import tkinter as tk
    global selected_stat_type
    selected_stat_type = tk.StringVar()
    selected_stat_type.set(stat_type_options[1])
    
    global eliteduino_instance
    nb.Button(frame, text="Send dummy data", command=eliteduino_instance.dummy_values).grid()
    
    nb.Button(frame, text="Set", command=set_dummy_status).grid()
    nb.Button(frame, text="Clear", command=clear_dummy_status).grid(row=2, column=2)
    nb.OptionMenu(frame, selected_stat_type, *stat_type_options).grid(row=2, column=3)

    from config import config
    stored_path = config.get("ed_journal_dump_path" )
    dump_path = config.get("ed_journal_dump_path" ) if stored_path is not None else default_journal_dump_filepath()

    global dump_path_var
    dump_path_var = tk.StringVar()
    dump_path_var.set(dump_path)
    
    global dump_setting_var
    dump_setting_var = tk.IntVar()
    dump_setting_var.set(config.getint("ed_journal_dump_enabled") if not None else 0)
    
    nb.Checkbutton(frame, text="Dump journal entries to file", variable = dump_setting_var).grid()
    nb.Entry(frame, textvariable = dump_path_var).grid(row=3, column=2, columnspan=2)
    nb.Button(frame, text="Browse", command=choose_dump_file_path).grid(row=3, column=4)
    return frame

def prefs_changed(cmdr, is_beta):
    """
    Save settings.
    """
    from config import config
    
    print("Saving eliteduino settings")
    
    global dump_setting_var
    config.set('ed_journal_dump_enabled', dump_setting_var.get())
    
    global dump_path_var
    config.set('ed_journal_dump_path', dump_path_var.get())

def set_dummy_status():
    global selected_stat_type
    stat_type = StatType[selected_stat_type.get()]
    
    global eliteduino_instance
    eliteduino_instance.update_stat(stat_type, True, True)

def clear_dummy_status():
    global selected_stat_type
    stat_type = StatType[selected_stat_type.get()]
    
    global eliteduino_instance
    eliteduino_instance.update_stat(stat_type, False, True)

def choose_dump_file_path():
    from tkinter.filedialog import asksaveasfilename
    path = asksaveasfilename()
    
    global dump_path_var
    dump_path_var.set(path)

def debug_write_journal_entry_to_log(entry):
    from config import config
    
    dump_setting_var = config.getint("ed_journal_dump_enabled")
    dumpfile_path = config.get("ed_journal_dump_path")
    
    if dump_setting_var is None or dumpfile_path is None:
        return
        
    if dump_setting_var == 0:
        return
    
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
    
    menu_galaxy_map    = entry.get("GuiFocus") is 6
    menu_system_map    = entry.get("GuiFocus") is 7
    
    global eliteduino_instance
    eliteduino_instance.update_stat(StatType.FSD_CHARGING, fsd_charging)
    eliteduino_instance.update_stat(StatType.FSD_COOLDOWN, fsd_cooldown)
    eliteduino_instance.update_stat(StatType.MASS_LOCKED, masslocked)
    eliteduino_instance.update_stat(StatType.OVERHEATING, overheating)
    eliteduino_instance.update_stat(StatType.BEING_INTERDICTED, being_interdicted)
    eliteduino_instance.update_stat(StatType.HARDPOINTS_DEPLOYED, hardpoints)
    eliteduino_instance.update_stat(StatType.MENU_GALAXY_MAP, menu_galaxy_map)
    eliteduino_instance.update_stat(StatType.MENU_SYSTEM_MAP, menu_system_map)
