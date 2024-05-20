import requests
import sys
from os.path import basename

Import('env')

try:
    import configparser
except ImportError:
    import ConfigParser as configparser
project_config = configparser.ConfigParser(inline_comment_prefixes="#")
private_config = configparser.ConfigParser(inline_comment_prefixes="#")
project_config.read("platformio.ini")
private_config.read("private_config.ini")

ota_config = {k: v for k, v in project_config.items("otabin")}
otaupload_config = {k: v for k, v in private_config.items("otabin")}

def publish_firmware(source, target, env):

    hardware = env.GetProjectOption("custom_prog_board")
    uuid = env.GetProjectOption("custom_hw_uuid")
    version = env.GetProjectOption("custom_prog_version")
    firmware_path = str(source[0])
    firmware_name = basename(firmware_path)

    print("Uploading {0} to otabin.com. Version: {1}".format(firmware_path, version))

    url = "/".join([
        otaupload_config.get("upload_server_url"), "fw/upload"
    ])

    headers = {
        "X-Firmware-Version": version,
        "X-Hardware": hardware,
        "X-Hardware-Uuid": uuid,
        "Authorization": "Bearer " + otaupload_config['api_token']
    }

    r = None
    try:
        files = {'firmware': open(firmware_path, "rb")}
        r = requests.post(url,
                         files = files,
                         headers = headers)
        r.raise_for_status()
    except requests.exceptions.RequestException as e:
        if r.status_code >= 400:
            sys.stderr.write("Failed to submit package: %s\n" % r.text)           
        else:
            sys.stderr.write("Failed to submit package: %s\n" %
                            ("%s\n%s" % (r.status_code, r.text) if r else str(e)))
        env.Exit(1)

    print("The firmware has been successfuly uploaded to otabin.com")

env.Replace(UPLOADCMD=publish_firmware)