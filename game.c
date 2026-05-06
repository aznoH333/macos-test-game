#include "raylib.h"
#include "stdio.h"
#include "dirent.h"
#include "stdlib.h"

// -------------------------------------------------------------------------------------
// Textures
// -------------------------------------------------------------------------------------
#define MAX_TEXTURES 32
#define MAX_ASSET_NAME_LENGTH 32


// returns a hash of a string
// giga lame implementation
int hashName(char* name) {
	int output = 0;

	int i = 0;

	while (name[i]) {
		output += name[i];
		++i;
	}

	return output * 97 % 301;
}


struct TextureMapNode{
	int hash;
	Texture2D texture;
};
typedef struct TextureMapNode TextureMapNode;



struct TextureMap{
	TextureMapNode textures[MAX_TEXTURES];
	int textureCount;
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
		TextureMapNode* node = &textureMap.textures[textureMap.textureCount];
	
		char texturePath[128];	
		snprintf(texturePath, 128, "./resources/sprites/%s", de->d_name);
		
		int hash = hashName(texturePath);
		printf("Loading texture #%d [%s] hash[%d]\n", textureMap.textureCount, texturePath, hash);
			
		node->hash = hash;
		node->texture = LoadTexture(texturePath);
	

		textureMap.textureCount++;
	}
    closedir(dr);    

}
/*
Texture2D getTexture(char* identifier){
	for (int i = 0; i < textureMap.textureCount; ++i) {
		TextureMapNode* node = &textureMap.textures[i];


	}
}
*/

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

		

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
