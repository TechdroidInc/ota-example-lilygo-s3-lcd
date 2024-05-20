Import("env")

try:
    import configparser
except ImportError:
    import ConfigParser as configparser
project_config = configparser.ConfigParser()
project_config.read("platformio.ini")

env.Replace(PROGNAME=env.GetProjectOption("custom_prog_board") + "_" + env.GetProjectOption("custom_prog_version")) 
