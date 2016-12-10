#include "espressif/esp_common.h"
#include "esp/uart.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "private_ssid_config.h"
#include "reciver.h"

void user_init(void) {
  uart_set_baud(0, 115200);
  printf("SDK version:%s\n", sdk_system_get_sdk_version());

  struct sdk_station_config config = {
    .ssid     = WIFI_SSID,
    .password = WIFI_PASS,
  };

  /* required to call wifi_set_opmode before station_set_config */
  sdk_wifi_set_opmode(STATION_MODE);
  sdk_wifi_station_set_config(&config);

  xTaskCreate(&Task_ServerTCP, "Echo_Server", 256, NULL, s 2, NULL);
}
