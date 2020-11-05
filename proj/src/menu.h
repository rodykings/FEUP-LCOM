#pragma once

//MODULES
#include "game.h"
#include "player.h"
#include "rtc.h"

//IMAGES

#include "img/m.h"
#include "img/btn_sp.h"
#include "img/btn_mp.h"
#include "img/btn_ex.h"
#include "img/btn_sp_hover.h"
#include "img/btn_mp_hover.h"
#include "img/btn_ex_hover.h"


int menu(char* buffer);
int select_menu_opt(int opt, char *buffer);
int game_main_cicle();

