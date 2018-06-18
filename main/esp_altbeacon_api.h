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

/* For ALTBeacon packet format, please refer to https://github.com/AltBeacon/spec
   The big endian representation of the beacon identifier. For interoperability purposes, 
   the first 16+ bytes of the beacon identifier should be unique to the advertiser's organizational unit.
   Any remaining bytes of the beacon identifier may be subdivided as needed for the use case.
 */

typedef struct {
    uint8_t flags[3];
    uint8_t length;
    uint8_t type;
    uint16_t company_id;
    uint16_t beacon_type;
}__attribute__((packed)) esp_ble_altbeacon_head_t;

typedef struct {
    uint8_t beacon_id[16];
    uint8_t beacon_id_extra[4];
    int8_t reference_rssi;
    uint8_t mfg_reserved;
}__attribute__((packed)) esp_ble_altbeacon_vendor_t;


typedef struct {
    esp_ble_altbeacon_head_t   altbeacon_head;
    esp_ble_altbeacon_vendor_t altbeacon_vendor;
}__attribute__((packed)) esp_ble_altbeacon_t;

/**
 */
void altbeacon_config_data(uint8_t *uuid, uint8_t *uuid_extra, uint8_t ref_rssi, uint8_t mfg_reserved);

/**
 */
void altbeacon_get_adv_data(uint8_t *adv_data_ptr);

/**
 */
uint8_t altbeacon_get_adv_data_size(void);