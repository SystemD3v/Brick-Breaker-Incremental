#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cjson/cJSON.h>

#include "constants.h"
#include "playerData.h"


char* _data_saveFilePath = "../savefiles/savefile.json";

int _data_cash = 0;
int _data_infinity = 0;

int _data_upgradeBrickCostLevel = 1;
int _data_upgradeWidthLevel = 1;
int _data_upgradeHeightLevel = 1;
int _data_gameSpeed = 5;
int _data_lossPreventionLevel = 1;
int _data_currentPattern = 1;
int _data_rToReset = 0;
int _data_tToPattern = 0;

int UPGRADE_COUNT = 10;
int _data_upgradeBought[10] = {0};




void _data_updateConstantsWithData() {
    NB_BRICK_WIDTH = 20 + _data_upgradeWidthLevel - 1;
    NB_BRICK_HEIGHT = 10 + _data_upgradeHeightLevel - 1;
}


void _data_saveGame() {
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "cash", _data_cash);
    cJSON_AddNumberToObject(json, "infinity", _data_infinity);

    cJSON_AddNumberToObject(json, "upgradeBrickCostLevel", _data_upgradeBrickCostLevel);
    cJSON_AddNumberToObject(json, "upgradeWidthLevel", _data_upgradeWidthLevel);
    cJSON_AddNumberToObject(json, "upgradeHeightLevel", _data_upgradeHeightLevel);
    cJSON_AddNumberToObject(json, "upgradeGameSpeed", _data_gameSpeed);
    cJSON_AddNumberToObject(json, "upgradeLossPrevention", _data_lossPreventionLevel);
    cJSON_AddNumberToObject(json, "upgradeRToReset", _data_rToReset);
    cJSON_AddNumberToObject(json, "upgradeTToPattern", _data_tToPattern);

    cJSON_AddNumberToObject(json, "saveCurrentPattern", _data_currentPattern);

    // Add the _data_upgradeBought array to JSON
    cJSON *upgradeBoughtArray = cJSON_CreateIntArray(_data_upgradeBought, UPGRADE_COUNT);
    cJSON_AddItemToObject(json, "upgradeBought", upgradeBoughtArray);

    char *json_string = cJSON_Print(json);

    FILE *file = fopen(_data_saveFilePath, "w");
    if (file == NULL) {
        perror("Unable to open file for writing");
        cJSON_Delete(json);
        free(json_string);
        return;
    }

    fprintf(file, "%s\n", json_string);

    fclose(file);
    printf("JSON successfully written to 'savefile.json'\n");

    cJSON_Delete(json);
    free(json_string);
}

int _data_loadGame() {
    FILE *file = fopen(_data_saveFilePath, "r");
    if (file == NULL) {
        perror("Unable to open file as no save currently seems to exist.");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(filesize + 1);
    if (buffer == NULL) {
        perror("Unable to allocate memory for file buffer");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        free(buffer);
        return 1;
    }

    cJSON *cash_item = cJSON_GetObjectItem(json, "cash");
    if (cash_item && cJSON_IsNumber(cash_item)) _data_cash = cash_item->valueint;

    cJSON *infinity_item = cJSON_GetObjectItem(json, "infinity");
    if (infinity_item && cJSON_IsNumber(infinity_item)) _data_infinity = infinity_item->valueint;

    cJSON *upgradeBrickCostLevel_item = cJSON_GetObjectItem(json, "upgradeBrickCostLevel");
    if (upgradeBrickCostLevel_item && cJSON_IsNumber(upgradeBrickCostLevel_item)) _data_upgradeBrickCostLevel = upgradeBrickCostLevel_item->valueint;

    cJSON *upgradeWidthLevel_item = cJSON_GetObjectItem(json, "upgradeWidthLevel");
    if (upgradeWidthLevel_item && cJSON_IsNumber(upgradeWidthLevel_item)) _data_upgradeWidthLevel = upgradeWidthLevel_item->valueint;

    cJSON *upgradeHeightLevel_item = cJSON_GetObjectItem(json, "upgradeHeightLevel");
    if (upgradeHeightLevel_item && cJSON_IsNumber(upgradeHeightLevel_item)) _data_upgradeHeightLevel = upgradeHeightLevel_item->valueint;

    cJSON *upgradeGameSpeed_item = cJSON_GetObjectItem(json, "upgradeGameSpeed");
    if (upgradeGameSpeed_item && cJSON_IsNumber(upgradeGameSpeed_item)) _data_gameSpeed = upgradeGameSpeed_item->valueint;

    cJSON *upgradeLossPrevention_item = cJSON_GetObjectItem(json, "upgradeLossPrevention");
    if (upgradeLossPrevention_item && cJSON_IsNumber(upgradeLossPrevention_item)) _data_lossPreventionLevel = upgradeLossPrevention_item->valueint;

    cJSON *upgradeRToReset_item = cJSON_GetObjectItem(json, "upgradeRToReset");
    if (upgradeRToReset_item && cJSON_IsNumber(upgradeRToReset_item)) _data_rToReset = upgradeRToReset_item->valueint;

    cJSON *upgradeTToPattern_item = cJSON_GetObjectItem(json, "upgradeTToPattern");
    if (upgradeTToPattern_item && cJSON_IsNumber(upgradeTToPattern_item)) _data_tToPattern = upgradeTToPattern_item->valueint;

    cJSON *saveCurrentPattern_item = cJSON_GetObjectItem(json, "saveCurrentPattern");
    if (saveCurrentPattern_item && cJSON_IsNumber(saveCurrentPattern_item)) _data_currentPattern = saveCurrentPattern_item->valueint;

    // Load _data_upgradeBought array
    cJSON *upgradeBoughtArray = cJSON_GetObjectItem(json, "upgradeBought");
    if (upgradeBoughtArray && cJSON_IsArray(upgradeBoughtArray)) {
        for (int i = 0; i < UPGRADE_COUNT; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeBoughtArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeBought[i] = item->valueint;
            }
        }
    }

    cJSON_Delete(json);
    free(buffer);

    return 0;
}

