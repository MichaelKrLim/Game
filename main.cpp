#include <raylib.h>

int main()
{
	InitWindow(1920, 1080, "Game");
	SetExitKey(KEY_SPACE); 
	Texture	texture =  LoadTexture("assets/tileset.png"); 
	while(!WindowShouldClose())
	{
		BeginDrawing();
		// rendering 
		ClearBackground(WHITE);
		DrawTextureRec(texture, Rectangle(100,100,100,100), Vector2(0,0), WHITE); 
		//
		EndDrawing();
	}
	UnloadTexture(texture); 
}
