#include "app_util.h"

APP_CallbackMapTypeDef *APP_CreateCallbackMap(uint32_t size)
{
    APP_CallbackMapTypeDef *map = malloc(sizeof(APP_CallbackMapTypeDef));
    if (map == NULL) {
        return NULL;
    }

    map->table = malloc(sizeof(APP_CallbackMapItemTypeDef *) * size);
    if (map->table == NULL) {
        free(map);
        return NULL;
    }

    for (uint32_t i = 0; i < size; i++) {
        map->table[i] = NULL;
    }

    map->size = size;

    return map;
}

/* Hash a string for a particular hash table. */
int __APP_GetCallbackMapHash(APP_CallbackMapTypeDef *map, char *key)
{
    unsigned long int hashval = 0;
    unsigned int i = 0;

    /* Convert our string to an integer */
    while (hashval < ULONG_MAX && i < strlen(key)) {
        hashval = hashval << 8;
        hashval += key[i];
        i++;
    }

    return hashval % map->size;
}

/* Create a key-value pair. */
APP_CallbackMapItemTypeDef *__APP_CreateCallbackMapItem(
    char *key,
    APP_CallbackTypeDef callback)
{
    APP_CallbackMapItemTypeDef *item = malloc(
        sizeof(APP_CallbackMapItemTypeDef));

    if (item == NULL) {
        return NULL;
    }

    item->key = strdup(key);
    if (item->key == NULL) {
        free(item);
        return NULL;
    }

    item->callback = callback;
    if (item->callback == NULL) {
        free(item->key);
        free(item);
        return NULL;
    }

    item->nextItem = NULL;

    return item;
}

/* Insert a key-value pair into a hash table. */
int APP_SetCallbackMapItem(
    APP_CallbackMapTypeDef *map,
    char *key,
    APP_CallbackTypeDef callback)
{
    APP_CallbackMapItemTypeDef *newItem = NULL;
    APP_CallbackMapItemTypeDef *next = NULL;
    APP_CallbackMapItemTypeDef *last = NULL;

    int hash = __APP_GetCallbackMapHash(map, key);

    next = map->table[hash];

    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->nextItem;
    }

    /* There's already a pair.  Let's replace that string. */
    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
        free(next->callback);
        next->callback = callback;
        return 1;
    }

    newItem = __APP_CreateCallbackMapItem(key, callback);
    if (newItem == NULL) {
        return 0;
    }

    if (next == map->table[hash]) { // We're at the start of the linked list in this bin.
        newItem->nextItem = next;
        map->table[hash] = newItem;
    } else if (next == NULL) { // We're at the end of the linked list in this bin.
        last->nextItem = newItem;
    } else { // We're in the middle of the list.
        newItem->nextItem = next;
        last->nextItem = newItem;
    }

    return 1;
}

/* Retrieve a key-value pair from a hash table. */
APP_CallbackTypeDef APP_GetCallbackMapItem(
    APP_CallbackMapTypeDef *map,
    char *key)
{
    int hash = __APP_GetCallbackMapHash(map, key);
    APP_CallbackMapItemTypeDef *item = map->table[hash];

    while (item != NULL && item->key != NULL) {
        if (strcmp(key, item->key) == 0) {
            return item->callback;
        }

        item = item->nextItem;
    }

    return NULL;
}

uint32_t APP_ADC_GetValue(ADC_HandleTypeDef* hadc, uint32_t channel)
{
    /* Config ADC channel */
    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
    /* Start conversion */
    HAL_ADC_Start(hadc);
    /* Wait until finish */
    HAL_ADC_PollForConversion(hadc, 100);
    uint32_t value = HAL_ADC_GetValue(hadc);

    return value;
}
