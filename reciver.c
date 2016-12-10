#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "lwip/opt.h"
#include "reciver.h"


void Task_ServerTCP(void *pvParameters) {
  struct netconn *conn, *newconn;
  err_t err;

  LWIP_UNUSED_ARG(pvParameters);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);

  /* Bind connection to well known port number 7. */
  netconn_bind(conn, NULL, PORT);
  printf("Binded Port:%d", PORT);

  /* Tell connection to go into listening mode. */
  netconn_listen(conn);

  while (1) {
    /* Grab new connection. */
    err = netconn_accept(conn, &newconn);

    // Set No blocking
    netconn_set_recvtimeout(newconn, TIMEOUT_SELECT);
    netconn_set_nonblocking(newconn, true);


    printf("Accepted new connection\n");

    /* Process the new connection. */
    if (err == ERR_OK) {
      struct netbuf *buf;
      void  *data;
      u16_t  len;
      size_t Writed;

      while ((err = netconn_recv(newconn, &buf)),
             (err == ERR_OK || err == ERR_TIMEOUT)) {
        /*printf("Recved\n");*/
        switch (err) {
        case ERR_OK: {
          do {
            netbuf_data(buf, &data, &len);
            Writed = 0;

            do { // Flush Buffer
              netconn_write_partly(newconn, data, len, NETCONN_COPY, &Writed);
            } while (Writed != len);

            // err = netconn_write(newconn, data, len, NETCONN_COPY);
          } while (netbuf_next(buf) >= 0);
          netbuf_delete(buf);
        } break;

        case ERR_TIMEOUT: {
          // printf("Time out\r\n");

          // Here you can perform you time operatin
          break;
        }

        default: continue;
        }
      }

      netconn_close(newconn);
      netconn_delete(newconn);
    }
  }
}
