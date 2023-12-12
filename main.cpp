#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <graphics.h>
#include <time.h>
#include <string.h>
#include <windows.h>

struct character
{
    char name[50];
    char weapon[50];
    int attackMod;
    int armor;
    int health;
    int dodgeMod;
    int heal = 3;
};
struct player
{
    char name[50];
    char weapon[50];
    int attackMod;
    int armor;
    int health;
    int dodgeMod;
    int heal;
};

void cleanBattleStats(int windowX,int windowY);

void battleScreen(char* machineCharacter, char* playerCharacter, char* playerHealth, char* playerHeal, char*  playerArmor, char* machineHealth, char* machineHeal, char*  machineArmor, int windowX, int windowY);

void showDices(int windowX, int windowY, char* attackDice, char* damageDice );

void battleLog(int windowX, int windowY, char* message);

void cleanBattleLog(int windowX, int windowY);

char* convertValuesToString(int value);

int generateRandomNumber(int range);

char* generateImagePath(const char* path1, const char* image, const char* path2);

void clearWindow();

void selectCharacter(char* machineCharacter, int windowX, int windowY);

void loadingScreen(char* playerCharacter, char* machineCharacter, int delay, int windowX, int windowY);

void characterDamage(player *thePlayer, int damage);

void finalScreen(int windowX, int windowY, bool win);

int rollDice(int range);

int main(void)
{

    int windowX = 1500, windowY = 700;

    //dices
    int dodgeDice, attackDice, damageDice;

    //actions
    int keyPressed, playerSelect, action = 5, defenceLimit = 0;

    //turns
    int turn = 0, machineDecision, attackIncrement;


    initwindow(windowX, windowY, "RPG MEDIEVAL", 0, 0);


    while(1){


        setfillstyle(1, 15);
        bar(0,0,1500,700);


        struct character characters[4] = {
            {"Barbaro", "Machado", 8, 14, 30, 18},
            {"Arqueiro", "Arco longo", 6, 12, 20, 14},
            {"Ladino", "Adaga", 8, 8, 20, 10},
            {"Paladino", "Espada longa", 6, 16, 30, 18}
        };

        int machine = generateRandomNumber(4);

        struct player machineCharacter = {
            "",
            "",
            characters[machine].attackMod,
            characters[machine].armor,
            characters[machine].health,
            characters[machine].dodgeMod,
            characters[machine].heal
        };

        strcpy(machineCharacter.name, characters[machine].name);
        strcpy(machineCharacter.weapon, characters[machine].weapon);



        int x, y;

        while(1){

            selectCharacter(machineCharacter.name, windowX, windowY);

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
            characters[playerSelect].heal
        };
        strcpy(playerCharacter.name, characters[playerSelect].name);
        strcpy(playerCharacter.weapon, characters[playerSelect].weapon);

        clearWindow();
        loadingScreen( playerCharacter.name, machineCharacter.name, 200, windowX, windowY);
        clearWindow();

        char battleStats[8][3], setTurn;

        while(playerCharacter.health > 0 && machineCharacter.health > 0 ){

            setbkcolor(15);
            setcolor(0);
            settextstyle(8, 0, 3);
            if(turn){
                outtextxy(windowX / 2 - 50, windowY / 2 - 250, "Turno inimigo");
            }else{
                outtextxy(windowX / 2 - 50, windowY / 2 - 250, "Seu turno");
            }

            attackDice = rollDice(21);
            dodgeDice = rollDice(21);
            damageDice = rollDice(9);

            attackDice += (turn) ? machineCharacter.attackMod : playerCharacter.attackMod;



            sprintf(battleStats[0], "%d", damageDice);
            sprintf(battleStats[1], "%d", attackDice);

            sprintf(battleStats[2], "%d", playerCharacter.armor);
            sprintf(battleStats[3], "%d", playerCharacter.health);
            sprintf(battleStats[4], "%d", playerCharacter.heal);

            sprintf(battleStats[5], "%d", machineCharacter.armor);
            sprintf(battleStats[6], "%d", machineCharacter.health);
            sprintf(battleStats[7], "%d", machineCharacter.heal);

            battleScreen(machineCharacter.name, playerCharacter.name, battleStats[2], battleStats[3], battleStats[4], battleStats[5], battleStats[6], battleStats[7], windowX, windowY);


            if(turn){
                //Enemy turn
                machineDecision = generateRandomNumber(3);
                if(machineDecision == 2 && (machineCharacter.heal < 1 || machineCharacter.health > characters[machine].health / 2)){
                    machineDecision = generateRandomNumber(2);
                }
                if(machineDecision == 1 && defenceLimit >= 1){
                    defenceLimit = 0;
                    machineDecision = 0;
                }

                switch(machineDecision){
                    case 0:
                        if(attackDice >= playerCharacter.armor && dodgeDice <= playerCharacter.dodgeMod){
                            characterDamage(&playerCharacter, damageDice);
                            battleLog(windowX, windowY, "Você sofreu um ataque");
                        }else if(dodgeDice > playerCharacter.dodgeMod){
                            battleLog(windowX, windowY, "Você desviou do ataque");
                        }else if(attackDice  < playerCharacter.armor){
                            battleLog(windowX, windowY, "O inimigo errou o ataque");
                        }
                        break;
                    case 1:
                        machineCharacter.armor += 5;
                        defenceLimit++;
                        battleLog(windowX, windowY, "Armadura inimiga aumentou em 5 pontos");
                        break;
                    case 2:
                        machineCharacter.heal--;
                        machineCharacter.health += 7;
                        battleLog(windowX, windowY, "O inimigo usou uma poção de cura");
                        break;
                }



                printf("Turno inimigo\n");

            }else{
                printf("Seu turno, pressione uma tecla\n");
                while(1){

                    if(kbhit()){
                        keyPressed = getch();


                        switch(keyPressed){
                            case 101:
                                action = 0;
                                break;
                            case 119:
                                if(playerCharacter.heal > 0 && playerCharacter.health <= characters[playerSelect].health / 2){
                                    action = 1;
                                    break;
                                }else if(playerCharacter.health >= characters[playerSelect].health / 2){
                                    battleLog(windowX, windowY, "Poção de cura indisponível");
                                    Sleep(2000);
                                    cleanBattleLog(windowX, windowY);
                                    break;
                                }
                                else{
                                    battleLog(windowX, windowY,"As suas poções se esgotaram...");
                                    cleanBattleLog(windowX, windowY);
                                    Sleep(2000);
                                    break;
                                }
                            case 113:
                                action = 2;
                                break;
                            default:
                                action = 5;
                                break;
                        }
                    }

                    if(action < 5){
                        break;
                    }

                }

                switch(action){
                    case 0:
                        battleLog(windowX, windowY, "Armadura fortificada em 5 pontos!");
                        playerCharacter.armor += 5;
                        break;
                    case 1:
                        playerCharacter.heal--;
                        playerCharacter.health += 7;
                        battleLog(windowX, windowY,"Voce usou 1 poção de cura");
                        break;
                    case 2:
                        if(attackDice >= machineCharacter.armor && dodgeDice <= machineCharacter.dodgeMod){
                            characterDamage(&machineCharacter, damageDice);
                            battleLog(windowX, windowY,"Você acertou o ataque");
                            break;
                        }else if(dodgeDice > machineCharacter.dodgeMod){
                            battleLog(windowX, windowY,"O inimigo desviou...");
                            break;
                        }else if(attackDice  < machineCharacter.armor){
                            battleLog(windowX, windowY,"Você errou o ataque");
                            break;
                        }
                }

                action = 5;

            }

            if(turn && playerCharacter.armor != characters[playerSelect].armor){
                playerCharacter.armor = characters[playerSelect].armor;
            }else if(!turn && machineCharacter.armor != characters[machine].armor){
                machineCharacter.armor = characters[machine].armor;
            }

            showDices(windowX, windowY, battleStats[1], battleStats[0]);
            turn = (turn == 0) ? 1 : 0;



            Sleep(5000);


            cleanBattleStats(windowX, windowY);
            cleanBattleLog(windowX, windowY);

        }
        clearWindow();
        if(playerCharacter.health <= 0){
            finalScreen(windowX, windowY, false);
        }else{
            finalScreen(windowX, windowY, true);
        }
        while(1){
            if(kbhit()){
                keyPressed = getch();

                if(keyPressed == 114){
                    action = 0;
                    break;
                }
                if(keyPressed == 27){
                    action = 1;
                    break;
                }
            }
        }

        if(!action){
            turn = 0;
            action = 5;
            continue;
        }else{
            break;
        }



    }





    closegraph();



    return 0;
}

void finalScreen(int windowX, int windowY, bool win){
    setbkcolor(15);
    setcolor(0);
    settextstyle(2, 0, 10);
    if(win){
        outtextxy(windowX / 2 - 275, windowY / 2, "Parabéns, você venceu!");
    }else{
        outtextxy(windowX / 2 - 275, windowY / 2, "Que pena, você perdeu...");
    }

        outtextxy(windowX / 2 - 275, windowY / 2 + 100, "R - Tentar novamente");
        outtextxy(windowX / 2 - 275, windowY / 2 + 150, "Esc - Sair");
}

void cleanBattleStats(int windowX,int windowY){
    setfillstyle(1,15);
    bar(windowX / 2 - 75,windowY / 2 - 170,windowX / 2 + 150,windowY / 2 - 150);

    //Clear armors
    bar(246, 280, 270, 300);
    bar(windowX - 55, windowY - 420, windowX-28, windowY - 400);

    //Clear Healths
    bar(198, 240, 240, 260);
    bar(windowX - 102, windowY - 460,windowX - 70, windowY - 439);

    //Clear turns
    bar(windowX / 2 - 50, windowY / 2 - 250, windowX / 2 + 150, windowY / 2 - 220);



}

void cleanBattleLog(int windowX, int windowY){
    //Clear battleLog
    bar(windowX / 2 - 120, windowY / 2 - 120, windowX / 2 + 500, windowY / 2 - 80);
}

void battleLog(int windowX, int windowY, char* message){

    setbkcolor(15);
    setcolor(0);
    settextstyle(8, 0, 3);

    outtextxy(windowX/2 - 120, windowY / 2 - 120, message);

}




void battleScreen(char* machineCharacter, char* playerCharacter, char* playerArmor, char* playerHealth, char*  playerHeal, char* machineArmor, char* machineHealth, char*  machineHeal, int windowX, int windowY){

    char str[10];
    setbkcolor(15);
    setcolor(0);

    readimagefile(generateImagePath("./assets/", playerCharacter, ".jpg"), 50, 50, 230, 230);
    readimagefile(generateImagePath("./assets/", machineCharacter, ".jpg"), windowX - 250, windowY - 650, windowX - 50, windowY - 470);

    settextstyle(8, 0, 2);

    //Player
    strcpy(str, "Vida: ");
    strcat(str, playerHealth);
    outtextxy(125, 240, str);

    strcpy(str, "Cura: ");
    strcat(str, playerHeal);
    outtextxy(125, 260, str);

    strcpy(str, "Armadura: ");
    strcat(str, playerArmor);
    outtextxy(125, 280, str);


    //Machine
    strcpy(str, "Vida: ");
    strcat(str, machineHealth);
    outtextxy(windowX - 175, windowY - 460, str);

    strcpy(str, "Cura: ");
    strcat(str, machineHeal);
    outtextxy(windowX - 175, windowY - 440, str);

    strcpy(str, "Armadura: ");
    strcat(str, machineArmor);
    outtextxy(windowX - 175, windowY - 420, str);


    line(0, windowY - 151, windowX, windowY - 151);
    readimagefile("./assets/commandBar.jpg", 0, windowY - 150, windowX, windowY);

    settextstyle(8, 0, 3);
    outtextxy(windowX/2 - 100, windowY / 2 - 200, "Acerto");
    outtextxy(windowX/2 + 60, windowY / 2 - 200, "Dano");



}

void showDices(int windowX, int windowY, char* attackDice, char* damageDice ){
    setbkcolor(15);
    setcolor(0);
    settextstyle(8, 0, 3);
    outtextxy(windowX / 2 + 90, windowY / 2 - 170, damageDice);
    outtextxy(windowX / 2 - 75, windowY / 2 - 170, attackDice);
}



int generateRandomNumber(int range)
{
    srand(time(NULL));
    return rand() % range;
}

int rollDice(int range){
    int random;
    srand(time(NULL));
    random = rand() % range;

    return (random == 0) ? 1 : random;

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

void clearWindow(){

    bar(0,0,1500,700);

}

void selectCharacter(char* machineCharacter, int windowX, int windowY){

    readimagefile("./assets/x.jpg", windowX / 2 - 30, windowY / 2 - 30, windowX/ 2 + 30, windowY / 2 + 30);
    readimagefile(generateImagePath("./assets/", machineCharacter, ".jpg"), (windowX / 2) + 327 - 180, windowY / 2 - 180,(windowX / 2) + 327 + 180, windowY / 2 + 180);
    readimagefile("./assets/charactersSelect.jpg", (windowX / 2) - 327 - 230, (windowY / 2) - 230,(windowX / 2) - 327 + 230, (windowY / 2 + 90) + 230);
    readimagefile("./assets/selectHeader.jpg", 0, 15, 1500, 105);

}


void loadingScreen(char* playerCharacter, char* machineCharacter, int delay, int windowX, int windowY){


    readimagefile("./assets/x.jpg", windowX / 2 - 30, windowY / 2 - 30, windowX/ 2 + 30, windowY / 2 + 30);
    readimagefile(generateImagePath("./assets/", machineCharacter, ".jpg"), (windowX / 2) + 327 - 180, windowY / 2 - 180,(windowX / 2) + 327 + 180, windowY / 2 + 180);
    readimagefile(generateImagePath("./assets/", playerCharacter, ".jpg"), (windowX / 2) - 327 - 180, windowY / 2 - 180,(windowX / 2) - 327 + 180, windowY / 2 + 180);
    readimagefile("./assets/loading.jpg", 0, windowY / 2 + 210, 1500, windowY);

    Sleep(delay);

}

void characterDamage(player *thePlayer, int damage){

    (*thePlayer).health -= damage;
}
