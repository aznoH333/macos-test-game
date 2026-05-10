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


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{


	const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	loadSprites();
    
	SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

		ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		DrawTexture(getTexture("weapon_parts_0001"), 50, 50, WHITE);
		

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
