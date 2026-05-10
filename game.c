#include "raylib.h"
#include "stdio.h"
#include "dirent.h"
#include "stdlib.h"
#include "string.h"


// -------------------------------------------------------------------------------------
// Textures
// -------------------------------------------------------------------------------------
#define MAX_TEXTURES 32
#define MAX_ASSET_NAME_LENGTH 32
#define MAX_HASHMAP_SIZE 301

// returns a hash of a string
// giga lame implementation
int hashName(char* name) {
	int output = 0;

	int i = 0;

	while (name[i]) {
		output += name[i];
		++i;
	}

	return output * 97 % MAX_HASHMAP_SIZE;
}


struct TextureMapNode{
	int hash;
	Texture2D texture;
};
typedef struct TextureMapNode TextureMapNode;



struct TextureMap{
	TextureMapNode textures[MAX_HASHMAP_SIZE];
};
typedef struct TextureMap TextureMap;


TextureMap textureMap = {0};


void loadSprites() {
	struct dirent *de;

    DIR *dr = opendir("./resources/sprites");

    if (dr == NULL) {
        printf("Could not open current directory" );
        return;
    }
    
	readdir(dr); readdir(dr); // skip . ..
	while ((de = readdir(dr)) != NULL){
		// build texture path
		char texturePath[128];	
		snprintf(texturePath, 128, "./resources/sprites/%s", de->d_name);
		
		// build identifier
		char identifier[128];
		int copyLength = strlen(de->d_name)-4;
		strncpy(identifier, de->d_name, copyLength);
		identifier[copyLength] = 0;


		int hash = hashName(identifier);
		printf("Loading texture [%s] hash[%d]\n", identifier, hash);
			
		// load to memmory
		TextureMapNode* node = &textureMap.textures[hash];
		node->hash = hash;
		node->texture = LoadTexture(texturePath);
	
	}
    closedir(dr);    

}

Texture2D getTexture(char* identifier){
	int hash = hashName(identifier);
	return textureMap.textures[hash].texture;	
}

#define GAME_SCALE 2.0f
void drawTexture(char* identifier, float x, float y) {
	DrawTextureEx(getTexture(identifier), (Vector2){x * GAME_SCALE, y * GAME_SCALE}, 0.0f, GAME_SCALE, WHITE);
}

// -------------------------------------------------------------------------------------
// Weapon drawing
// -------------------------------------------------------------------------------------
#define WEAPON_PART_HANDLE 0
#define WEAPON_PART_BLADE 1


struct WeaponPart {
	char* part_name;
	char part_type;
	char next_x;
	char next_y;
};
typedef struct WeaponPart WeaponPart;

#define MAX_WEAPON_PARTS 32
WeaponPart weaponParts[MAX_WEAPON_PARTS];
int nextWeaponPartIndex = 0;

void loadWeaponPart(
	char* part_name,
	char part_type,
	char next_x,
	char next_y
) {
	WeaponPart* part = &weaponParts[nextWeaponPartIndex++];

	part->part_name = part_name;
	part->part_type = part_type;
	part->next_x = next_x;
	part->next_y = next_y;
	

}


struct Weapon {
	WeaponPart* weaponParts[2];
};
typedef struct Weapon Weapon;


Weapon makeWeapon(int handleIndex, int bladeIndex) {
	Weapon weapon;

	weapon.weaponParts[0] = &weaponParts[handleIndex];
	weapon.weaponParts[1] = &weaponParts[bladeIndex];


	return weapon;
}

void drawWeapon(Weapon* weapon, float x, float y) {
	
	float current_x = x;
	float current_y = y;
	for (int i = 0; i < 2; ++i) {
		WeaponPart* part = weapon->weaponParts[i];


		
		drawTexture(part->part_name, current_x, current_y);

		current_x += (float) part->next_x;
		current_y -= (float) part->next_y;
	}

}


void loadWeapons() {
	// load weapon parts
	loadWeaponPart(
		"weapon_parts_0001",
		WEAPON_PART_BLADE,
		0,
		0
	);
	loadWeaponPart(
		"weapon_parts_0002",
		WEAPON_PART_BLADE,
		0,
		0
	);
	loadWeaponPart(
		"weapon_parts_0003",
		WEAPON_PART_BLADE,
		0,
		0
	);

	loadWeaponPart(
		"weapon_parts_0004",
		WEAPON_PART_BLADE,
		0,
		0
	);

	loadWeaponPart(
		"weapon_parts_0005",
		WEAPON_PART_BLADE,
		0,
		0
	);
	loadWeaponPart(
		"weapon_parts_0006",
		WEAPON_PART_BLADE,
		0,
		0
	);

	loadWeaponPart(
		"weapon_parts_0007",
		WEAPON_PART_HANDLE,
		0,
		0
	);
	loadWeaponPart(
		"weapon_parts_0008",
		WEAPON_PART_HANDLE,
		2,
		2
	);
	loadWeaponPart(
		"weapon_parts_0009",
		WEAPON_PART_HANDLE,
		9,
		9
	);

}

// -------------------------------------------------------------------------------------
// Loading
// -------------------------------------------------------------------------------------
void loadGame() {
	loadSprites();
	loadWeapons();
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{


	const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	loadGame();
  
	Weapon weapons[6][3];

	Weapon test = makeWeapon(7, 1);

	for (int blade = 0; blade < 6; ++blade) {
		for (int handle = 0; handle < 3; ++handle) {
			weapons[blade][handle] = makeWeapon(6+handle, blade);
		}
	}

	SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

		ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		for (int blade = 0; blade < 6; ++blade) {
			for (int handle = 0; handle < 3; ++handle) {
				drawWeapon(&weapons[blade][handle], 10 + blade * 30, 10 + handle * 30);
			}
		}

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
