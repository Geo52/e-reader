## hardware
- esp32
- 4.2 inch waveshare e-ink display module
## software
- esp idf

## setup (so far)
0. reference this to install esp idf on your machine  
<mark>*NOTE: If your on a linux/mac you have have to add your user to the : sudo usermod -aG dialout $USER
</mark>
- https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html

- https://www.youtube.com/watch?v=J8zc8mMNKtc&t=595s

1. once you have idf on your machine clone the repo
> git clone https://github.com/Geo52/e-reader.git
2. setup up the setting for wifi, and a custom partition
>idf.py menuconfig
- WIFI: menuconfig->example connection config->specify ssid and password
- SPIFFS: https://www.youtube.com/watch?v=V9-cgXag4Ko&t=621s (reference the project setup chapter)
3. flash the code to the esp  
> idf.py flash
4. monitor and look for the ip
>idf.py monitor
* should look something like whats below
    * esp_netif_handlers: example_netif_sta ip: 192.168.1.71
5. enter the ip into browser
6. use the form to upload your books