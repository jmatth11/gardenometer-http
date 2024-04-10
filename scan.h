#ifndef GARDENOMETER_SCAN_H
#define GARDENOMETER_SCAN_H

#include <ESP8266HTTPClient.h>
#include <WString.h>

/**
 * Scan all local IPs for home website.
 * @param[in] port The port to listen to.
 * @param[in] path The uri.
 * @param[in] key The key for verification of home website.
 * @param[out] out The HTTPClient object to populate if home website is found.
 * @return 1 for success, 0 for failure.
 */
int scan_ips_for_website(int port, String path, String key, HTTPClient *out);

#endif
