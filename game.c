#include "raylib.h"
#include "stdio.h"
#include "dirent.h"
#include "stdlib.h"
#include "string.h"



// test foo
// foo foo


// -------------------------------------------------------------------------------------
// Textures
// -------------------------------------------------------------------------------------
#define MAX_TEXTURES 32
#define MAX_ASSET_NAME_LENGTH 32
#define MAX_HASHMAP_SIZE 521

// returns a hash of a string
// giga lame implementation
unsigned int hashName(char* name) {
	unsigned int output = 1;

	int i = 0;

	while (name[i]) {
		output *= name[i] + name[i-1 + (i == 0)];
		++i;
	}

	return output % MAX_HASHMAP_SIZE;
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


		unsigned int hash = hashName(identifier);
		printf("Loading texture [%s] hash[%d]\n", identifier, hash);
			
		// load to memmory
		TextureMapNode* node = &textureMap.textures[hash];
		node->hash = hash;
		node->texture = LoadTexture(texturePath);
	
	}
    closedir(dr);    

}

Texture2D getTexture(char* identifier){
	unsigned int hash = hashName(identifier);
	return textureMap.textures[hash].texture;	
}

#define GAME_SCALE 4.0f
void drawTexture(char* identifier, float x, float y, Color color) {
	if (x > -32 && x < 1300 && y > -32 && y < 800) {
		DrawTextureEx(getTexture(identifier), (Vector2){x * GAME_SCALE, y * GAME_SCALE}, 0.0f, GAME_SCALE, color);
	}

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


struct GameMaterial {
	Color color;
};
typedef struct GameMaterial GameMaterial;

#define MAX_WEAPON_MATERIALS 32
GameMaterial weaponGameMaterials[MAX_WEAPON_MATERIALS];
int nextGameMaterialIndex = 0;

void loadGameMaterial(Color color) {
	GameMaterial* weaponGameMaterial = &weaponGameMaterials[nextGameMaterialIndex++];

	weaponGameMaterial->color = color;
}


struct WeaponPartInstance {
	WeaponPart* part;
	GameMaterial* material;
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
		.material = &weaponGameMaterials[materialIndex]
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
	loadGameMaterial((Color){167, 123, 91, 255});
	// dark wood
	loadGameMaterial((Color){128, 73, 58, 255});
	// copper
	loadGameMaterial((Color){180, 82, 82, 255});
	// iron
	loadGameMaterial((Color){134, 126, 136, 255});
	// gold
	loadGameMaterial((Color){237, 225, 158, 255});
	// human skin
	loadGameMaterial((Color){229, 206, 180, 255});
	// bone
	loadGameMaterial((Color){242, 240, 209, 255});
	// goblin skin
	loadGameMaterial((Color){138, 176, 96, 255});
	
}


// -------------------------------------------------------------------------------------
// Body parts
// -------------------------------------------------------------------------------------

typedef struct {
	char* part_name;
	char next_x;
	char next_y;
} BodyPart;

typedef struct {
	BodyPart* body_part;
	GameMaterial* material;
} BodyPartInstance;


#define MAX_BODY_PARTS 32
BodyPart bodyParts[MAX_BODY_PARTS];
int nextBodyPartIndex = 0;

void loadBodyPart(
	char* part_name,
	char next_x,
	char next_y
) {
	bodyParts[nextBodyPartIndex++] = (BodyPart) {
		.part_name = part_name,
		.next_x = next_x,
		.next_y = next_y
	};
}


#define MAX_BODY_PARTS_PER_BODY 8
typedef struct {
	BodyPartInstance body_parts[MAX_BODY_PARTS_PER_BODY];
	int body_part_count;
} Body;

BodyPartInstance makeBodyPartInstance(int bodyPartIndex, int materialIndex) {
	return (BodyPartInstance){
		.body_part = &bodyParts[bodyPartIndex],
		.material = &weaponGameMaterials[materialIndex]
	};
}

Body makeBody(BodyPartInstance body_parts[MAX_BODY_PARTS_PER_BODY], int count) {
	Body body = {0};


	memcpy(body.body_parts, body_parts, sizeof(BodyPartInstance) * MAX_BODY_PARTS_PER_BODY);
	body.body_part_count = count;


	return body; 
}


void drawBody(Body* body, float x, float y) {
	
	float current_x = x;
	float current_y = y;

	for (int i = 0; i < body->body_part_count; ++i) {
		BodyPartInstance* part = &body->body_parts[i];
		drawTexture(part->body_part->part_name, current_x, current_y, part->material->color);
		

		current_x += (float) part->body_part->next_x;
		current_y -= (float) part->body_part->next_y;
	}

}


void loadBodyParts() {
	loadBodyPart(
		"body_parts_0001",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0002",
		0,
		4
	);
		
	loadBodyPart(
		"body_parts_0003",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0004",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0005",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0006",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0007",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0008",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0009",
		0,
		4
	);
		
	loadBodyPart(
		"body_parts_0010",
		0,
		4
	);
	
	loadBodyPart(
		"body_parts_0011",
		0,
		2
	);
	
}


Body makeAGuy(int legsMaterialIndex, int bodyMaterialIndex, int headMaterialIndex, int headShapeIndex, int headWearMaterialIndex, int headWearShapeIndex) {


	int count = 3 + (headWearShapeIndex != 0);
	
	BodyPartInstance bodyParts[MAX_BODY_PARTS_PER_BODY];

	bodyParts[0] = makeBodyPartInstance(10, legsMaterialIndex);
	bodyParts[1] = makeBodyPartInstance(9, bodyMaterialIndex);
	bodyParts[2] = makeBodyPartInstance(headShapeIndex, headMaterialIndex);
	
	if (headWearShapeIndex != -1) {
		bodyParts[3] = makeBodyPartInstance(headWearShapeIndex, headWearMaterialIndex);
	}

	return makeBody(bodyParts, count);
}

// -------------------------------------------------------------------------------------
// Loading
// -------------------------------------------------------------------------------------
void loadGame() {
	loadSprites();
	loadWeapons();
	loadBodyParts();
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
	Weapon weapons[6][3][nextGameMaterialIndex][nextGameMaterialIndex];


	for (int blade = 0; blade < 6; ++blade) {
		for (int handle = 0; handle < 3; ++handle) {
			for (int bladeGameMaterial = 0; bladeGameMaterial < nextGameMaterialIndex; ++bladeGameMaterial) {
				for (int handleGameMaterial = 0; handleGameMaterial < nextGameMaterialIndex; ++handleGameMaterial) {
					weapons[blade][handle][bladeGameMaterial][handleGameMaterial] = makeWeapon(
						makeWeaponPartInstance(6+handle, handleGameMaterial), 
						makeWeaponPartInstance(blade, bladeGameMaterial)
					);
				}
			}
			
		}
	}

	int cameraX = 0;
	int cameraY = 0;


	Body testBody = makeAGuy(3, 3, 4, 4, 1, 1);
		

	Body people[10000];
	int nextIndex = 0;
	for (int shoeMaterial = 0; shoeMaterial < nextGameMaterialIndex; ++shoeMaterial) {
		for (int shirtMaterial = 0; shirtMaterial < nextGameMaterialIndex; ++shirtMaterial) {
			for (int skinColor = 0; skinColor < nextGameMaterialIndex; ++skinColor)  {
				for (int headShape = 0; headShape < 5; ++headShape ) {
					for (int hatWearMaterial = 0; hatWearMaterial < nextGameMaterialIndex; ++hatWearMaterial) {
						for (int hatShape = 0; hatShape < 5; ++hatShape) {
							people[nextIndex++] = makeAGuy(
								shoeMaterial,
								shirtMaterial,
								skinColor,
								headShape + 5,
								hatWearMaterial,
								hatShape == 0 ? -1 : hatShape - 1
							);
							if (nextIndex >= 10000) {
								goto bepis;
							}
						}
					}
				}
			}
		}
	}

	bepis:

	printf("loaded\n");

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
		
		// render weapons
		for (int blade = 0; blade < 6; ++blade) {
			for (int handle = 0; handle < 3; ++handle) {
				for (int bladeGameMaterial = 0; bladeGameMaterial < nextGameMaterialIndex; ++bladeGameMaterial) {
					for (int handleGameMaterial = 0; handleGameMaterial < nextGameMaterialIndex; ++handleGameMaterial) {

						drawWeapon(&weapons[blade][handle][bladeGameMaterial][handleGameMaterial], 
							cameraX + 10 + blade * 30 + bladeGameMaterial * 32 * 6, 
							cameraY + 10 + handle * 30 + handleGameMaterial * 32 * 3);
					}
				}
			}
		}

		// render guys
		for (int i = 0; i < 10000; i++) {
			int x = i % 100 * -30 + cameraX;
			int y = i / 100 * 30 + cameraY;
			drawBody(&people[i], (float)x, (float)y);

		}


		DrawText("Rendering weapons", 530, 10, 30, BLACK);

		drawBody(&testBody, -30 + cameraX, 0 + cameraY);
		EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
