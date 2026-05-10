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

#define GAME_SCALE 4.0f
void drawTexture(char* identifier, float x, float y, Color color) {
	DrawTextureEx(getTexture(identifier), (Vector2){x * GAME_SCALE, y * GAME_SCALE}, 0.0f, GAME_SCALE, color);
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


struct WeaponMaterial {
	Color color;
};
typedef struct WeaponMaterial WeaponMaterial;

#define MAX_WEAPON_MATERIALS 32
WeaponMaterial weaponMaterials[MAX_WEAPON_MATERIALS];
int nextWeaponMaterialIndex = 0;

void loadWeaponMaterial(Color color) {
	WeaponMaterial* weaponMaterial = &weaponMaterials[nextWeaponMaterialIndex++];

	weaponMaterial->color = color;
}


struct WeaponPartInstance {
	WeaponPart* part;
	WeaponMaterial* material;
};
typedef struct WeaponPartInstance WeaponPartInstance;


struct Weapon {
	WeaponPartInstance weaponParts[2];
};
typedef struct Weapon Weapon;


Weapon makeWeapon(WeaponPartInstance handle, WeaponPartInstance blade) {
	Weapon weapon;

	weapon.weaponParts[0] = handle;
	weapon.weaponParts[1] = blade;


	return weapon;
}

WeaponPartInstance makeWeaponPartInstance(int partIndex, int materialIndex) {
	return (WeaponPartInstance) {
		.part = &weaponParts[partIndex],
		.material = &weaponMaterials[materialIndex]
	};
}

void drawWeapon(Weapon* weapon, float x, float y) {
	
	float current_x = x;
	float current_y = y;
	for (int i = 0; i < 2; ++i) {
		WeaponPartInstance* part = &weapon->weaponParts[i];


		
		drawTexture(part->part->part_name, current_x, current_y, part->material->color);

		current_x += (float) part->part->next_x;
		current_y -= (float) part->part->next_y;
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

	// weapon materials
	// light wood
	loadWeaponMaterial((Color){167, 123, 91, 255});
	// dark wood
	loadWeaponMaterial((Color){128, 73, 58, 255});
	// copper
	loadWeaponMaterial((Color){211, 160, 104, 255});
	// iron
	loadWeaponMaterial((Color){134, 126, 136, 255});
	// gold
	loadWeaponMaterial((Color){237, 225, 158, 255});

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


	const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	loadGame();
  
  	// blade type, handle type, blade material, handle material
	Weapon weapons[6][3][nextWeaponMaterialIndex][nextWeaponMaterialIndex];


	for (int blade = 0; blade < 6; ++blade) {
		for (int handle = 0; handle < 3; ++handle) {
			for (int bladeMaterial = 0; bladeMaterial < nextWeaponMaterialIndex; ++bladeMaterial) {
				for (int handleMaterial = 0; handleMaterial < nextWeaponMaterialIndex; ++handleMaterial) {
					weapons[blade][handle][bladeMaterial][handleMaterial] = makeWeapon(
						makeWeaponPartInstance(6+handle, handleMaterial), 
						makeWeaponPartInstance(blade, bladeMaterial)
					);
				}
			}
			
		}
	}

	int cameraX = 0;
	int cameraY = 0;

	SetTargetFPS(60);
    while (!WindowShouldClose())
    {

		if (IsKeyDown(KEY_UP)) {
			cameraY += 1;
		}

		if (IsKeyDown(KEY_DOWN)) {
			cameraY -= 1;
		}

		if (IsKeyDown(KEY_LEFT)) {
			cameraX += 1;
		}


		if (IsKeyDown(KEY_RIGHT)) {
			cameraX -= 1;
		}


        BeginDrawing();

		ClearBackground(RAYWHITE);


		for (int blade = 0; blade < 6; ++blade) {
			for (int handle = 0; handle < 3; ++handle) {
				for (int bladeMaterial = 0; bladeMaterial < nextWeaponMaterialIndex; ++bladeMaterial) {
					for (int handleMaterial = 0; handleMaterial < nextWeaponMaterialIndex; ++handleMaterial) {

						drawWeapon(&weapons[blade][handle][bladeMaterial][handleMaterial], 
							cameraX + 10 + blade * 30 + bladeMaterial * 30 * 6, 
							cameraY + 10 + handle * 30 + handleMaterial * 30 * 3);
					}
				}
			}
		}
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
