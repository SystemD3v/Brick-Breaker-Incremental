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

int _data_maxBrickCostLevelValue = 22;

int UPGRADE_BACKGROUND_MAIN = 4;
int _data_upgradeBackgroundMain[4] = {0};
int _data_backgroundMainSelected = 0;
int UPGRADE_BACKGROUND_SHOP = 4;
int _data_upgradeBackgroundShop[4] = {0};
int _data_backgroundShopSelected = 0;
int UPGRADE_BACKGROUND_INF = 4;
int _data_upgradeBackgroundInf[4] = {0};
int _data_backgroundInfSelected = 0;
int UPGRADE_BRICKS_COLOR = 4;
int _data_upgradeBricksColor[4] = {0};
int _data_bricksColorSelected = 0;

int UPGRADE_AUDIO_BG = 3;
int _data_upgradeAudioBG[3] = {0};
int _data_AudioBGSelected = 0;

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
    cJSON_AddNumberToObject(json, "maxBrickCostLevel", _data_maxBrickCostLevelValue);

    cJSON_AddNumberToObject(json, "saveCurrentPattern", _data_currentPattern);


    cJSON *upgradeBoughtArray = cJSON_CreateIntArray(_data_upgradeBought, UPGRADE_COUNT);
    cJSON_AddItemToObject(json, "upgradeBought", upgradeBoughtArray);


    cJSON_AddNumberToObject(json, "backgroundMainSelected", _data_backgroundMainSelected);
    cJSON *upgradeBackgroundMainBoughtArray = cJSON_CreateIntArray(_data_upgradeBackgroundMain, UPGRADE_BACKGROUND_MAIN);
    cJSON_AddItemToObject(json, "upgradeBackgroundMain", upgradeBackgroundMainBoughtArray);


    cJSON_AddNumberToObject(json, "backgroundShopSelected", _data_backgroundShopSelected);
    cJSON *upgradeBackgroundShopBoughtArray = cJSON_CreateIntArray(_data_upgradeBackgroundShop, UPGRADE_BACKGROUND_SHOP);
    cJSON_AddItemToObject(json, "upgradeBackgroundShop", upgradeBackgroundShopBoughtArray);


    cJSON_AddNumberToObject(json, "backgroundInfSelected", _data_backgroundInfSelected);
    cJSON *upgradeBackgroundInfBoughtArray = cJSON_CreateIntArray(_data_upgradeBackgroundInf, UPGRADE_BACKGROUND_INF);
    cJSON_AddItemToObject(json, "upgradeBackgroundInf", upgradeBackgroundInfBoughtArray);


    cJSON_AddNumberToObject(json, "bricksColorSelected", _data_bricksColorSelected);
    cJSON *upgradeBricksColorSelectedArray = cJSON_CreateIntArray(_data_upgradeBricksColor, UPGRADE_BRICKS_COLOR);
    cJSON_AddItemToObject(json, "upgradeBricksColor", upgradeBricksColorSelectedArray);


    cJSON_AddNumberToObject(json, "audioBGSelected", _data_AudioBGSelected);
    cJSON *upgradeAudioBGSelectedArray = cJSON_CreateIntArray(_data_upgradeAudioBG, UPGRADE_AUDIO_BG);
    cJSON_AddItemToObject(json, "upgradeAudioBG", upgradeAudioBGSelectedArray);


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

    cJSON *upgradeMaxBrickCostLevel_item = cJSON_GetObjectItem(json, "maxBrickCostLevel");
    if (upgradeMaxBrickCostLevel_item && cJSON_IsNumber(upgradeMaxBrickCostLevel_item)) _data_maxBrickCostLevelValue = upgradeMaxBrickCostLevel_item->valueint;

    cJSON *saveCurrentPattern_item = cJSON_GetObjectItem(json, "saveCurrentPattern");
    if (saveCurrentPattern_item && cJSON_IsNumber(saveCurrentPattern_item)) _data_currentPattern = saveCurrentPattern_item->valueint;



    cJSON *upgradeBoughtArray = cJSON_GetObjectItem(json, "upgradeBought");
    if (upgradeBoughtArray && cJSON_IsArray(upgradeBoughtArray)) {
        for (int i = 0; i < UPGRADE_COUNT; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeBoughtArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeBought[i] = item->valueint;
            }
        }
    }


    cJSON *upgradeBackgroundMainArray = cJSON_GetObjectItem(json, "upgradeBackgroundMain");
    if (upgradeBackgroundMainArray && cJSON_IsArray(upgradeBackgroundMainArray)) {
        for (int i = 0; i < UPGRADE_BACKGROUND_MAIN; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeBackgroundMainArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeBackgroundMain[i] = item->valueint;
            }
        }
    }
    cJSON *saveBackgroundMainSelected_item = cJSON_GetObjectItem(json, "backgroundMainSelected");
    if (saveBackgroundMainSelected_item && cJSON_IsNumber(saveBackgroundMainSelected_item)) _data_backgroundMainSelected = saveBackgroundMainSelected_item->valueint;


    cJSON *upgradeBackgroundShopArray = cJSON_GetObjectItem(json, "upgradeBackgroundShop");
    if (upgradeBackgroundShopArray && cJSON_IsArray(upgradeBackgroundShopArray)) {
        for (int i = 0; i < UPGRADE_BACKGROUND_SHOP; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeBackgroundShopArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeBackgroundShop[i] = item->valueint;
            }
        }
    }
    cJSON *saveBackgroundShopSelected_item = cJSON_GetObjectItem(json, "backgroundShopSelected");
    if (saveBackgroundShopSelected_item && cJSON_IsNumber(saveBackgroundShopSelected_item)) _data_backgroundShopSelected = saveBackgroundShopSelected_item->valueint;


    cJSON *upgradeBackgroundInfArray = cJSON_GetObjectItem(json, "upgradeBackgroundInf");
    if (upgradeBackgroundInfArray && cJSON_IsArray(upgradeBackgroundInfArray)) {
        for (int i = 0; i < UPGRADE_BACKGROUND_INF; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeBackgroundInfArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeBackgroundInf[i] = item->valueint;
            }
        }
    }
    cJSON *saveBackgroundInfSelected_item = cJSON_GetObjectItem(json, "backgroundInfSelected");
    if (saveBackgroundInfSelected_item && cJSON_IsNumber(saveBackgroundInfSelected_item)) _data_backgroundInfSelected = saveBackgroundInfSelected_item->valueint;


    cJSON *upgradeBrickColorArray = cJSON_GetObjectItem(json, "upgradeBricksColor");
    if (upgradeBrickColorArray && cJSON_IsArray(upgradeBrickColorArray)) {
        for (int i = 0; i < UPGRADE_BRICKS_COLOR; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeBrickColorArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeBricksColor[i] = item->valueint;
            }
        }
    }
    cJSON *saveBrickColorSelected_item = cJSON_GetObjectItem(json, "bricksColorSelected");
    if (saveBrickColorSelected_item && cJSON_IsNumber(saveBrickColorSelected_item)) _data_bricksColorSelected = saveBrickColorSelected_item->valueint;


    cJSON *upgradeAudioBGArray = cJSON_GetObjectItem(json, "upgradeAudioBG");
    if (upgradeAudioBGArray && cJSON_IsArray(upgradeAudioBGArray)) {
        for (int i = 0; i < UPGRADE_AUDIO_BG; i++) {
            cJSON *item = cJSON_GetArrayItem(upgradeAudioBGArray, i);
            if (item && cJSON_IsNumber(item)) {
                _data_upgradeAudioBG[i] = item->valueint;
            }
        }
    }
    cJSON *saveAudioBGSelected_item = cJSON_GetObjectItem(json, "audioBGSelected");
    if (saveAudioBGSelected_item && cJSON_IsNumber(saveAudioBGSelected_item)) _data_AudioBGSelected = saveAudioBGSelected_item->valueint;


    cJSON_Delete(json);
    free(buffer);

    return 0;
}

