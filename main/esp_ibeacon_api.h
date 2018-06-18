/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* Major and Minor part are stored in big endian mode in iBeacon packet,
 * need to use this macro to transfer while creating or processing
 * iBeacon data */
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))
#define IBEACON_UUID_LEN     16

/* The big endian representation of the beacon identifier. For interoperability purposes, 
 * the first 16+ bytes of the beacon identifier should be unique to the advertiser's organizational unit.
 * Any remaining bytes of the beacon identifier may be subdivided as needed for the use case.
 */
typedef struct {
    uint8_t flags[3];
    uint8_t length;
    uint8_t type;
    uint16_t company_id;
    uint16_t beacon_type;
    uint8_t uuid[IBEACON_UUID_LEN];
    uint16_t major;
    uint16_t minor;
    int8_t measured_power;;
}__attribute__((packed)) esp_ble_ibeacon_t;

/**
 */
void ibeacon_config_data(uint8_t *uuid, uint16_t major, uint16_t minor, int8_t measured_power);

/**
 */
void ibeacon_get_adv_data(uint8_t *adv_data_ptr);

/**
 */
uint8_t ibeacon_get_adv_data_size(void);