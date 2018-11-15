#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"

typedef void (*APP_CallbackTypeDef)(char *params);

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

/////////////////////
//  Rendering
////////////////////

#define APP_Draw_FillRect_FUNC_PTR  void (*rect)(int16_t , int16_t , int16_t , int16_t , uint16_t)
#define APP_Draw_FillCircle_FUNC_PTR  void (*circle)(int16_t, int16_t, int16_t, uint16_t)
#define APP_Draw_GetColor_FUNC_PTR  uint16_t (*color)(uint8_t , uint8_t , uint8_t)
#define APP_Draw_Printf_FUNC_PTR  void (*printf)(const char *fmt, ...)
#define APP_Draw_SetCursor_FUNC_PTR  void (*cursor)(uint16_t x, uint16_t y)
#define APP_Draw_FillBackground_FUNC_PTR  void (*background)(uint16_t color)

typedef struct
{
    APP_Draw_FillRect_FUNC_PTR;
    APP_Draw_FillCircle_FUNC_PTR;
    APP_Draw_GetColor_FUNC_PTR;
    APP_Draw_Printf_FUNC_PTR;
    APP_Draw_SetCursor_FUNC_PTR;
    APP_Draw_FillBackground_FUNC_PTR;

    uint16_t backgroundColor;
    int16_t screenWidth;
    int16_t screenHeight;

} APP_RenderingEngineTypeDef;
