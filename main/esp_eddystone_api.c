/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/****************************************************************************
*
* This file is for eedystone uuid beacons
*
****************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "esp_log.h"

#include "esp_eddystone_api.h"

#define ENDIAN_CHANGE_U16(x) ( (((x)&0xFF00)>>8) + (((x)&0xFF)<<8) )
#define ENDIAN_CHANGE_U32(x) ( (((x)&0xFF000000)>>24) + (((x)&0x00FF0000)>>16) + (((x)&0x0000FF00)<<16) + (((x)&0x000000FF)<<24) )

static const char* TAG = "EDDYSTONE_API";

static esp_ble_eddystone_uid_t eddystone_uid_advertising_data = {
        .flags = {0x02, 0x01, 0x00},
        .length = 21,
        .type = 0x16,
        .beacon_type = 0xFEAA,
        .frame_type = 0x00,
        .ranging_data = 0xC5,
        .id_namespace = {0xFD, 0xA5, 0x06, 0x93, 0xA4, 0xE2, 0x4F, 0xB1, 0xAF, 0xCF},
        .id_instance = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        .rfu = {0x03,0xFF,0xE0,0x00}
};

static esp_ble_eddystone_tlm_t eddystone_tlm_advertising_data = {
        .flags = {0x02, 0x01, 0x00},
        .length = 17,
        .type = 0x16,
        .beacon_type = 0xFEAA,
        .frame_type = 0x20,
        .version = 0x00,
        .vbatt = 0,
        .temp_ent = 0,
        .temp_dec = 0,
        .adv_cnt = 0,
        .sec_cnt = 0
};


void eddystone_tlm_config_data(uint16_t vbatt, float temp, uint32_t adv_cnt, uint32_t sec_cnt)
{
    int8_t temp_88_h = 0;
    uint8_t temp_88_l = 0;

    eddystone_tlm_advertising_data.vbatt = ENDIAN_CHANGE_U16(vbatt);

    if(temp >= -128 && temp <= 127){
      temp_88_h = (int8_t) temp;
      temp_88_l = (uint8_t) ((temp - temp_88_h)*100);
      temp_88_l = (temp_88_l * 256)/100;
      ESP_LOGI(TAG, "Telemetry temperature: %d  %d  ", temp_88_h, temp_88_l);
    } 

    eddystone_tlm_advertising_data.temp_ent = temp_88_h;
    eddystone_tlm_advertising_data.temp_dec = temp_88_l;
    eddystone_tlm_advertising_data.adv_cnt = ENDIAN_CHANGE_U32(adv_cnt);
    eddystone_tlm_advertising_data.sec_cnt = ENDIAN_CHANGE_U32(sec_cnt);
}

void eddystone_uid_config_data(uint8_t *id_namespace, uint8_t *id_instance, uint8_t ranging_data)
{
    if( id_namespace!=NULL ){
        memcpy( eddystone_uid_advertising_data.id_namespace,
                id_namespace,
                EDDYSTONE_UID_NAMESPACE_LEN
               );
    }

    if( id_instance!=NULL ){
        memcpy( eddystone_uid_advertising_data.id_instance,
                id_instance,
                EDDYSTONE_UID_INSTANCE_LEN
               );
    }

    eddystone_uid_advertising_data.ranging_data = ranging_data;
}

void eddystone_get_adv_data(uint8_t *adv_data_ptr, uint8_t frame_type){
    if( adv_data_ptr != NULL)
    {
        switch(frame_type)
        {
          case FRAME_TYPE_UID:
              memcpy(adv_data_ptr, &eddystone_uid_advertising_data, sizeof(eddystone_uid_advertising_data));
            break;
          case FRAME_TYPE_TLM:
              memcpy(adv_data_ptr, &eddystone_tlm_advertising_data, sizeof(eddystone_tlm_advertising_data));
            break;
          default:
            break;
        }

    }
}

uint8_t eddystone_get_adv_data_size(uint8_t frame_type){
    switch(frame_type)
    {
      case FRAME_TYPE_UID:
        return sizeof(eddystone_uid_advertising_data);
      case FRAME_TYPE_TLM:
        return sizeof(eddystone_tlm_advertising_data);
      default:
        return 0;
    }
}
