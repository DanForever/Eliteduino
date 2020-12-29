from pyhid import pyhid
import msvcrt

def banana():
    with pyhid.Hid() as hid, hid.enumerate() as enumeration:
        for deviceInfo in enumeration:
            if(deviceInfo.product_string):
                print(deviceInfo.product_string)
                print("vendor id: " + hex(deviceInfo.vendor_id))
                print("product id: " + hex(deviceInfo.product_id))
                print("Usage/page:" + hex(deviceInfo.usage) + " / " +  hex(deviceInfo.usage_page))
                print("")
                
            if(deviceInfo.vendor_id == 0x16c0 and deviceInfo.usage_page==0xffab):
                path = deviceInfo.path
                with deviceInfo.connect() as device:
                    a = pyhid.Buffer(3)
                    a[1] = ord("b")
                    device.write(a)


def plugin_start3(plugin_dir: str) -> str:
   """
   Load this plugin into EDMC
   """
   print(f"I am loaded! My plugin folder is {plugin_dir}")
   
   banana()
   
   return "Eliteduino"

banana()