#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"

typedef void (*APP_CallbackTypeDef)(void);

struct __APP_CallbackMapItemStruct
{
    char *key;
    APP_CallbackTypeDef callback;
    struct __APP_CallbackMapItemStruct *nextItem;
};

typedef struct __APP_CallbackMapItemStruct APP_CallbackMapItemTypeDef;

typedef struct
{
    uint32_t size;
    struct __APP_CallbackMapItemStruct **table;
} APP_CallbackMapTypeDef;

APP_CallbackMapTypeDef *APP_CreateCallbackMap(uint32_t size);

int __APP_GetCallbackMapHash(APP_CallbackMapTypeDef *map, char *key);

APP_CallbackMapItemTypeDef *__APP_CreateCallbackMapItem(
    char *key,
    APP_CallbackTypeDef func);

int APP_SetCallbackMapItem(
    APP_CallbackMapTypeDef *map,
    char *key,
    APP_CallbackTypeDef func);

APP_CallbackTypeDef APP_GetCallbackMapItem(
    APP_CallbackMapTypeDef *map,
    char *key);

