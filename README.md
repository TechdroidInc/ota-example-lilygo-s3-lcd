## Otabin Example Project

## Getting started:

  # Copy private config:
  ```
    cp private_config.ini.dist private_config.ini
  ``` 
  # Update private_config.ini with your wifi credentials
  
  # Edit the otabin section in private_config.ini to your credentials:
  ```
  [otabin]
  api_token = [your otabin api token]
  upload_server_url = https://app.otabin.com
  ```
  # Update the hardware section in platform.ini with your version, HW UUID and Hardware Name
  ```
  custom_prog_version = 1.5.0
  custom_hw_uuid =  5d7aba61-690e-4842-ba79-7638b224343f
  custom_prog_board = esp32-s3-rev-a  
  ```