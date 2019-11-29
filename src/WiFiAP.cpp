#include "WiFi.h"
#include "WiFiGeneric.h"
#include "WiFiAP.h"
#include "UnifiedAtWifi.h"
#include "UnifiedAtTcpIp.h"

extern "C" {
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
// #include <esp_err.h>
// #include <esp_wifi.h>
// #include <esp_event_loop.h>
// #include <lwip/ip_addr.h>
// #include "dhcpserver/dhcpserver_options.h"
}



// // -----------------------------------------------------------------------------------------------------------------------
// // ---------------------------------------------------- Private functions ------------------------------------------------
// // -----------------------------------------------------------------------------------------------------------------------

// static bool softap_config_equal(const wifi_config_t& lhs, const wifi_config_t& rhs);



// /**
//  * compare two AP configurations
//  * @param lhs softap_config
//  * @param rhs softap_config
//  * @return equal
//  */
// static bool softap_config_equal(const wifi_config_t& lhs, const wifi_config_t& rhs)
// {
//     if(strcmp(reinterpret_cast<const char*>(lhs.ap.ssid), reinterpret_cast<const char*>(rhs.ap.ssid)) != 0) {
//         return false;
//     }
//     if(strcmp(reinterpret_cast<const char*>(lhs.ap.password), reinterpret_cast<const char*>(rhs.ap.password)) != 0) {
//         return false;
//     }
//     if(lhs.ap.channel != rhs.ap.channel) {
//         return false;
//     }
//     if(lhs.ap.ssid_hidden != rhs.ap.ssid_hidden) {
//         return false;
//     }
//     if(lhs.ap.max_connection != rhs.ap.max_connection) {
//         return false;
//     }
//     return true;
// }

// -----------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------- AP function -----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------


/**
 * Set up an access point
 * @param ssid              Pointer to the SSID (max 63 char).
 * @param passphrase        (for WPA2 min 8 char, for open use NULL)
 * @param channel           WiFi channel number, 1 - 13.
 * @param ssid_hidden       Network cloaking (0 = broadcast SSID, 1 = hide SSID)
 * @param max_connection    Max simultaneous connected clients, 1 - 4.
*/
bool WiFiAPClass::softAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection){
    // if(!WiFi.enableAP(true)) {
    //     // enable AP failed
    //     log_e("enable AP first!");
    //     return false;
    // }

    // if(!ssid || *ssid == 0) {
    //     // fail SSID missing
    //     log_e("SSID missing!");
    //     return false;
    // }

    // if(passphrase && (strlen(passphrase) > 0 && strlen(passphrase) < 8)) {
    //     // fail passphrase too short
    //     log_e("passphrase too short!");
    //     return false;
    // }

    // esp_wifi_start();

    // wifi_config_t conf;
    // strlcpy(reinterpret_cast<char*>(conf.ap.ssid), ssid, sizeof(conf.ap.ssid));
    // conf.ap.channel = channel;
    // conf.ap.ssid_len = strlen(reinterpret_cast<char *>(conf.ap.ssid));
    // conf.ap.ssid_hidden = ssid_hidden;
    // conf.ap.max_connection = max_connection;
    // conf.ap.beacon_interval = 100;

    // if(!passphrase || strlen(passphrase) == 0) {
    //     conf.ap.authmode = WIFI_AUTH_OPEN;
    //     *conf.ap.password = 0;
    // } else {
    //     conf.ap.authmode = WIFI_AUTH_WPA2_PSK;
    //     strlcpy(reinterpret_cast<char*>(conf.ap.password), passphrase, sizeof(conf.ap.password));
    // }

    // wifi_config_t conf_current;
    // esp_wifi_get_config(WIFI_IF_AP, &conf_current);
    // if(!softap_config_equal(conf, conf_current) && esp_wifi_set_config(WIFI_IF_AP, &conf) != ESP_OK) {
    //     return false;
    // }

    // return true;

    if (!WiFi.enableAP(true)) {
        // enable AP failed
        log_e("enable AP first!");
        return false;
    }

    if (!ssid || *ssid == 0) {
        // fail SSID missing
        log_e("SSID missing!");
        return false;
    }

    if (passphrase && (strlen(passphrase) > 0 && strlen(passphrase) < 8)) {
        // fail passphrase too short
        log_e("passphrase too short!");
        return false;
    }

    WifiApConfigure config;
    config.ssid = ssid;
    config.channel = channel;
    config.isSsidHidden = ssid_hidden;
    config.maxConnect = max_connection;

    if (!passphrase || strlen(passphrase) == 0) {
        config.pwd = "";
        config.ecn = WIFI_AUTH_OPEN;
    } 
    else {
        config.pwd = passphrase;
        config.ecn = WIFI_AUTH_WPA2_PSK;
    }
    return atWifiApConfigure(config) == success;
}


/**
 * Configure access point
 * @param local_ip      access point IP
 * @param gateway       gateway IP
 * @param subnet        subnet mask
 */
bool WiFiAPClass::softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet){

    // if(!WiFi.enableAP(true)) {
    //     // enable AP failed
    //     return false;
    // }

    // esp_wifi_start();

    // tcpip_adapter_ip_info_t info;
    // info.ip.addr = static_cast<uint32_t>(local_ip);
    // info.gw.addr = static_cast<uint32_t>(gateway);
    // info.netmask.addr = static_cast<uint32_t>(subnet);
    // tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
    // if(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info) == ESP_OK) {
    //     dhcps_lease_t lease;
    //     lease.enable = true;
    //     lease.start_ip.addr = static_cast<uint32_t>(local_ip) + (1 << 24);
    //     lease.end_ip.addr = static_cast<uint32_t>(local_ip) + (11 << 24);

    //     tcpip_adapter_dhcps_option(
    //         (tcpip_adapter_option_mode_t)TCPIP_ADAPTER_OP_SET,
    //         (tcpip_adapter_option_id_t)REQUESTED_IP_ADDRESS,
    //         (void*)&lease, sizeof(dhcps_lease_t)
    //     );

    //     return tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP) == ESP_OK;
    // }
    // return false;

    if (!WiFi.enableAP(true)) {
        // enable AP failed
        return false;
    }

    if (atApIp(*(Ipv4 *)(& local_ip), *(Ipv4 *)(& gateway), *(Ipv4 *)(& subnet)) == fail){
        return false;
    }

    if (atDhcp(disable, 1 << TCPIP_ADAPTER_IF_AP) == fail){
        return false;
    }

    IpRange  range;
    uint32_t start = local_ip + (1 << 24);
    uint32_t end = local_ip + (11 << 24);
    atDhcpIpRange(& range);
    range.startIp = *(Ipv4 *) & start;
    range.endIp = *(Ipv4 *) & end;

    if (atDhcpIpRange(range) == fail){
        return false;
    }
    return atDhcp(enable, 1 << TCPIP_ADAPTER_IF_AP) == success;
}



/**
 * Disconnect from the network (close AP)
 * @param wifioff disable mode?
 * @return one value of wl_status_t enum
 */
bool WiFiAPClass::softAPdisconnect(bool wifioff){
    // bool ret;
    // wifi_config_t conf;

    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return ESP_ERR_INVALID_STATE;
    // }

    // *conf.ap.ssid     = 0;
    // *conf.ap.password = 0;
    // conf.ap.authmode  = WIFI_AUTH_OPEN; // auth must be open if pass=0   
    // ret = esp_wifi_set_config(WIFI_IF_AP, &conf) == ESP_OK;

    // if(wifioff) {
    //     ret = WiFi.enableAP(false) == ESP_OK;
    // }

    // return ret;

    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return ESP_ERR_INVALID_STATE;
    }

    WifiApConfigure config;
    atWifiApConfigure(& config);
    config.ssid = "";
    config.pwd = "";
    config.ecn = WIFI_AUTH_OPEN;
    if (atWifiApConfigure(config) == fail){
        return false;
    }

    return WiFi.enableAP(false) == ESP_OK;
}


/**
 * Get the count of the Station / client that are connected to the softAP interface
 * @return Stations count
 */
uint8_t WiFiAPClass::softAPgetStationNum(){
    // wifi_sta_list_t clients;
    // if (WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return 0;
    // }
    // if (esp_wifi_ap_get_sta_list(&clients) == ESP_OK) {
    //     return clients.num;
    // }
    // return 0;

    std::vector<WifiUser> list;
    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL || atWifiUser(list) == fail){
        return 0;
    }
    return list.size();
}

/**
 * Get the softAP interface IP address.
 * @return IPAddress softAP IP
 */
IPAddress WiFiAPClass::softAPIP(){
    // tcpip_adapter_ip_info_t ip;
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return IPAddress();
    // }
    // tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip);
    // return IPAddress(ip.ip.addr);

    Ipv4 ip;
    
    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL || atApIp(& ip) == fail){
        return IPAddress();
    }
    return IPAddress((ip_addr_t *) & ip);
}

IPAddress softApIp(IPAddress (* call)(IPAddress, IPAddress)){
    Ipv4 ip;
    Ipv4 gw;
    Ipv4 mask;

    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL || atApIp(& ip, & gw, & mask) == fail){
        return IPAddress();
    }
    return call(
        IPAddress((ip_addr_t *) & gw), 
        IPAddress((ip_addr_t *) & mask)
    );
}

IPAddress softApCIDR(IPAddress gw, IPAddress mask){
    return WiFiGenericClass::calculateSubnetCIDR(mask);
}

/**
 * Get the softAP broadcast IP address.
 * @return IPAddress softAP broadcastIP
 */
IPAddress WiFiAPClass::softAPBroadcastIP(){
    // tcpip_adapter_ip_info_t ip;
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return IPAddress();
    // }
    // tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip);
    // return WiFiGenericClass::calculateBroadcast(IPAddress(ip.gw.addr), IPAddress(ip.netmask.addr));

    return softAp(& WiFiGenericClass::calculateBroadcast);
}

/**
 * Get the softAP network ID.
 * @return IPAddress softAP networkID
 */
IPAddress WiFiAPClass::softAPNetworkID(){
    // tcpip_adapter_ip_info_t ip;
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return IPAddress();
    // }
    // tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip);
    // return WiFiGenericClass::calculateNetworkID(IPAddress(ip.gw.addr), IPAddress(ip.netmask.addr));

    return softAp(& WiFiGenericClass::calculateNetworkID);
}

/**
 * Get the softAP subnet CIDR.
 * @return uint8_t softAP subnetCIDR
 */
uint8_t WiFiAPClass::softAPSubnetCIDR(){
    // tcpip_adapter_ip_info_t ip;
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return (uint8_t)0;
    // }
    // tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip);
    // return WiFiGenericClass::calculateSubnetCIDR(IPAddress(ip.netmask.addr));
    return softAp(& softApCIDR);
}

/**
 * Get the softAP interface MAC address.
 * @param mac   pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
 * @return      pointer to uint8_t*
 */
uint8_t* WiFiAPClass::softAPmacAddress(uint8_t* bssid){
    // if(WiFiGenericClass::getMode() != WIFI_MODE_NULL){
    //     esp_wifi_get_mac(WIFI_IF_AP, mac);
    // }
    // return mac;

    if (WiFiGenericClass::getMode() != WIFI_MODE_NULL){
        atApMac((mac *)bssid);
    }
    return bssid;
}

/**
 * Get the softAP interface MAC address.
 * @return String mac
 */
String WiFiAPClass::softAPmacAddress(void){
    // uint8_t mac[6];
    // char macStr[18] = { 0 };
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return String();
    // }
    // esp_wifi_get_mac(WIFI_IF_AP, mac);

    // sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // return String(macStr);
    uint8_t bssid[6] = { 0 };
    char macStr[18] = { 0 };
    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return String();
    }
    atApMac((mac *)bssid);
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    return String(macStr);
}

/**
 * Get the softAP interface Host name.
 * @return char array hostname
 */
const char * WiFiAPClass::softAPgetHostname(){
    // const char * hostname = NULL;
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return hostname;
    // }
    // if(tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_AP, &hostname)) {
    //     return hostname;
    // }
    // return hostname;

    static String hostname;
    WifiApConfigure config;
    
    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL || 
        atWifiApConfigure(& config) == fail){
        return NULL;
    }
    hostname = config.ssid;
    return hostname.c_str();
}

/**
 * Set the softAP    interface Host name.
 * @param  hostname  pointer to const string
 * @return true on   success
 */
bool WiFiAPClass::softAPsetHostname(const char * hostname){
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return false;
    // }
    // return tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_AP, hostname) == ESP_OK;

    WifiApConfigure config;
    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL || 
        atWifiApConfigure(& config) == fail){
        return false;
    }
    config.ssid = hostname;
    return atWifiApConfigure(config) == success;
}

/**
 * Enable IPv6 on the softAP interface.
 * @return true on success
 */
bool WiFiAPClass::softAPenableIpV6(){
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return false;
    // }
    // return tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_AP) == ESP_OK;

    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return false;
    }
    // ESP-AT NOT IMPLEMENT ---------------------------------------------------------------------------------------------------------------------------------
    return false;
}

/**
 * Get the softAP interface IPv6 address.
 * @return IPv6Address softAP IPv6
 */
IPv6Address WiFiAPClass::softAPIPv6(){
    // static ip6_addr_t addr;
    // if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
    //     return IPv6Address();
    // }
    // if(tcpip_adapter_get_ip6_linklocal(TCPIP_ADAPTER_IF_AP, &addr)) {
    //     return IPv6Address();
    // }
    // return IPv6Address(addr.addr);

    // ESP-AT NOT IMPLEMENT ---------------------------------------------------------------------------------------------------------------------------------
    return false;
}
