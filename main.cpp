#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <graphics.h>
#include <time.h>
#include <string.h>

struct character
{
    char name[50];
    char weapon[50];
    int attackMod;
    int armor;
    int health;
    int dodgeMod;
};
struct player
{
    char name[50];
    char weapon[50];
    int attackMod;
    int armor;
    int health;
    int dodgeMod;
};

int generateRandomNumber(int range)
{
    srand(time(NULL));
    return rand() % range;
}

char* generateImagePath(const char* path1, const char* image, const char* path2) {

    size_t totalLength = strlen(path1) + strlen(image) + strlen(path2) + 1;

    char* result = (char*)malloc(totalLength);


    if (result != NULL) {

        strcpy(result, path1);


        strcat(result, image);


        strcat(result, path2);
    }

    return result;
}

int main(void)
{

    int windowX = 1500, windowY = 700;

    int dodgeDice, attackDice, damageDice, keyPressed, playerSelect;
    bool turn = 0;


    initwindow(1500, 700, "RPG MEDIEVAL", 0, 0);
    setfillstyle(1, 15);
    bar(0,0,1500,700);

    struct character characters[4] = {
        {"Barbaro", "Machado", 4, 14, 60, 16},
        {"Arqueiro", "Arco longo", 2, 12, 40, 7},
        {"Ladino", "Adaga", 2, 8, 40, 7},
        {"Paladino", "Espada longa", 4, 18, 60, 16}
    };

    int machine = generateRandomNumber(4);

    struct player machineCharacter = {
        "",
        "",
        characters[machine].attackMod,
        characters[machine].armor,
        characters[machine].health,
        characters[machine].dodgeMod,
    };

    strcpy(machineCharacter.name, characters[machine].name);
    strcpy(machineCharacter.weapon, characters[machine].weapon);



    int x, y;

    printf("%s\n", generateImagePath("./assets/", machineCharacter.name, ".jpg"));

    //1173 310 coordenadas
    readimagefile("./assets/x.jpg", windowX / 2 - 30, windowY / 2 - 30, windowX/ 2 + 30, windowY / 2 + 30);
    readimagefile(generateImagePath("./assets/", machineCharacter.name, ".jpg"), 1173 - 180, windowY / 2 - 180, 1173 + 180, windowY / 2 + 180);
    while(1){

        x = mousex();
        y = mousey();
        if (ismouseclick(WM_LBUTTONDOWN))
            {
            printf("%d %d\n", x, y);
            clearmouseclick(WM_LBUTTONDOWN);
            }

        if(kbhit()){
            keyPressed = getch();

            switch(keyPressed){
                case 97:
                    playerSelect = 1;
                    break;
                case 98:
                    playerSelect = 0;
                    break;
                case 108:
                    playerSelect = 2;
                    break;
                case 112:
                    playerSelect = 3;
                    break;
                default:
                    playerSelect = 5;
            }


            if(playerSelect <= 3){
                break;
            }

        }

    }

    struct player playerCharacter = {
        "",
        "",
        characters[playerSelect].attackMod,
        characters[playerSelect].armor,
        characters[playerSelect].health,
        characters[playerSelect].dodgeMod,
    };
    strcpy(playerCharacter.name, characters[playerSelect].name);
    strcpy(playerCharacter.weapon, characters[playerSelect].weapon);

    readimagefile(generateImagePath("./assets/", playerCharacter.name, ".jpg"),400, 400, 10, 10);

    while(!kbhit()){

    }

    closegraph();



    return 0;
}
