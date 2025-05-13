#ifndef ESP8266_COMMON_H
#define ESP8266_COMMON_H

#define ESP8266_IP_ADDR_INFO_ITEM_SIZE 32U 

typedef enum esp8266_wifi_mode_e
{
    ESP8266_WIFI_MODE_NULL,     // 0: Null mode. Wi-Fi RF will be disabled.
    ESP8266_WIFI_MODE_STATION,  // 1: Station mode.
    ESP8266_WIFI_MODE_SOFT_AP,  // 2: SoftAP mode.
    ESP8266_WIFI_MODE_UNIVERSAL // 3: SoftAP+Station mode.
} esp8266_wifi_mode_t;

typedef struct esp8266_ip_addr_info_s
{
    char softAP_ip[ESP8266_IP_ADDR_INFO_ITEM_SIZE];   // <”APIP”>: IPv4 address of Wi-Fi softAP interface
    char softAP_mac[ESP8266_IP_ADDR_INFO_ITEM_SIZE];  // <”APMAC”>: MAC address of Wi-Fi softAP interface
    char station_ip[ESP8266_IP_ADDR_INFO_ITEM_SIZE];  // <”STAIP”>: IPv4 address of Wi-Fi station interface
    char station_mac[ESP8266_IP_ADDR_INFO_ITEM_SIZE]; // <”STAMAC”>: MAC address of Wi-Fi station interface
} esp8266_ip_addr_info_t;

#endif