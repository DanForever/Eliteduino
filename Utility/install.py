import os
import shutil
import requests

user_path = os.path.expanduser("~")
plugin_directory = os.path.join(user_path, "AppData", "Local", "EDMarketConnector", "plugins", "Eliteduino")

# First we make sure the destination is clear
print("Clearing existing install")
shutil.rmtree(plugin_directory, True)

# Copy our plugin to the edmc plugins folder
print("Copying plugin")
script_directory = os.path.dirname(os.path.realpath(__file__))
source_directory = os.path.join(script_directory, "..", "EDMC")
shutil.copytree(source_directory, plugin_directory, ignore=shutil.ignore_patterns("Submodules", "__pycache__"))

# Copy pyhid from the submodules folder into the plugins folder
# We do a bit of path modification here as we don't actually want
# the whole pyhid repository copied into the plugin 
print("Copying pyhid")
pyhid_source_directory = os.path.join(source_directory, "Submodules", "PyHid", "pyhid")
pyhid_target_directory = os.path.join(plugin_directory, "pyhid")
shutil.copytree(pyhid_source_directory, pyhid_target_directory, ignore=shutil.ignore_patterns("__pycache__"))

# Same again for all the other submodules
print("Copying pyserial")
pyserial_source_directory = os.path.join(source_directory, "Submodules", "pyserial", "serial")
pyserial_target_directory = os.path.join(plugin_directory, "serial")
shutil.copytree(pyserial_source_directory, pyserial_target_directory, ignore=shutil.ignore_patterns("__pycache__"))

print("Ensuring the downloads folder exists")
downloads_directory = os.path.join(script_directory, "downloads")

try:
    os.mkdir(downloads_directory)
except:
    pass

print("Checking latest hidapi release")
response = requests.get("https://api.github.com/repos/libusb/hidapi/releases/latest")
data = response.json()

def download_file(url, target_path):
    # NOTE the stream=True parameter below
    with requests.get(url, stream=True) as r:
        r.raise_for_status()
        with open(target_path, 'wb') as f:
            for chunk in r.iter_content(chunk_size=8192): 
                # If you have chunk encoded response uncomment if
                # and set chunk_size parameter to None.
                #if chunk: 
                f.write(chunk)

hidapi_download_url = data["assets"][0]["browser_download_url"]
filename = os.path.basename(hidapi_download_url)
download_path = os.path.join(downloads_directory, filename)

if(os.path.isfile(download_path) == False):
    print("Downloading latest hidapi release")
    download_file(hidapi_download_url, download_path)
else:
    print("latest hidapi release has already been downloaded")

#hidapi.dll
print("Extracting hidapi.dll and copying it to the install directory")
import zipfile
hidapi_dll_target_directory = os.path.join(pyhid_target_directory, "lib")
hidapi_dll_target_path = os.path.join(hidapi_dll_target_directory, "hidapi.dll")
file = zipfile.ZipFile(download_path)

try:
    os.mkdir(hidapi_dll_target_directory)
except:
    pass
    
with file.open("x86/hidapi.dll") as source, open(hidapi_dll_target_path, "wb") as target:
    shutil.copyfileobj(source, target)
