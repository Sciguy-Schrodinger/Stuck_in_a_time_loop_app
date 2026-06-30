#include "raylib.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cmath>
#include <algorithm>

using namespace std;

int width = 1000;
int height = 1000;

int level = 1;

Sound door_open;
Sound hit_button;
Sound warp;
Music game_music;

int char_width = 100;
int char_height = 100;

const int max_time_loops = 100;
int current_time_loop = 0;

const int max_frames = 10000;
float x_history[max_time_loops][max_frames];
float y_history[max_time_loops][max_frames];

bool clone0_init = false;
clock_t clone_start_time[max_time_loops];
int clone_lifetime[max_time_loops];
bool clone_active[max_time_loops] = {true};  

int frame_count = 0;
int button1_clones = 0;
int button2_clones = 0;
int RNGdoor = 0;
int RNGclones1 = 0;
int RNGclones2 = 0;
bool RNG_init_door = false;
bool RNG_init_clones = false;
bool lvl4_clone0_initialized = false;

float x_p = width/2;
float y_p = height/2;

float x_ps[max_time_loops];
float y_ps[max_time_loops];

struct Wall{
  float x;
  float y;
  float height;
  float width;
  Rectangle rect;
};

struct Button{
  float x;
  float y;
  float Bheight;
  float Bwidth;
  Rectangle rect;
};

struct Doors{
  int level;
  Vector2 Lopen;
  Vector2 Lclosed;
  Vector2 Ropen;
  Vector2 Rclosed;
  bool unlocked = false;
  bool play_unlock_door_sound = false;
};

const int Num_of_doors = 12;

Doors door[Num_of_doors];

void DrawDoor(Doors &door, Texture2D opendoor, Texture2D closeddoor, Sound openSound){
  if(door.unlocked){
    if(!door.play_unlock_door_sound){
      PlaySound(openSound);
      door.play_unlock_door_sound = true;
    }

    DrawTextureV(opendoor,door.Lopen,WHITE);
    DrawTextureV(opendoor,door.Ropen,WHITE);
}
  else{
    DrawTextureV(closeddoor,door.Lopen,WHITE);
    DrawTextureV(closeddoor,door.Ropen,WHITE);
  }
}

void Init_doors(){
  
door[0].level = 1;
door[0].Lopen = {2*width/5.0,0};
door[0].Lclosed = {2*width/5.0,0};
 door[0].Ropen = {width/2.0,0};
 door[0].Rclosed = {width/2.0,0};

door[1].level = 2;
 door[1].Lopen = {2*width/5.0,0};
 door[1].Lclosed = {2*width/5.0,0};
 door[1].Ropen = {width/2.0,0};
 door[1].Rclosed = {width/2.0,0};

door[2].level = 3;
 door[2].Lopen = {2*width/5.0,0};
 door[2].Lclosed = {2*width/5.0,0};
 door[2].Ropen = {width/2.0,0};
 door[2].Rclosed = {width/2.0,0};

door[3].level = 4;
 door[3].Lopen = {2*width/5.0,0};
 door[3].Lclosed = {2*width/5.0,0};
 door[3].Ropen = {width/2.0,0};
 door[3].Rclosed = {width/2.0,0};

// Door 1 (top-left)
door[4].level = 5;
 door[4].Lopen = {width/5.0,0};
 door[4].Lclosed = {width/5.0,0};
 door[4].Ropen = {3*width/10.0,0};
 door[4].Rclosed = {3*width/10.0,0};

// Door 2 (top-right)
door[5].level = 5;
 door[5].Lopen = {3*width/5.0,0};
 door[5].Lclosed = {6*width/10.0,0};
 door[5].Ropen = {7*width/10.0,0};
 door[5].Rclosed = {7*width/10.0,0};

// Door 3 (bottom-left)
door[6].level = 5;
 
 door[6].Lopen = {width/5.0,97*height/100.0};
 door[6].Lclosed = {width/5.0,99*height/100.0};
 door[6].Ropen = {3*width/10.0,97*height/100.0};
 door[6].Rclosed = {3*width/10.0,99*height/100.0};

// Door 4 (bottom-right)
door[7].level = 5;
 door[7].Lopen = {3*width/5.0,97*height/100.0};
 door[7].Lclosed = {3*width/5.0,99*height/100.0};
 door[7].Ropen = {7*width/10.0,97*height/100.0};
 door[7].Rclosed = {7*width/10.0,99*height/100.0};

// Door 5 (left-top)
door[8].level = 5;
 door[8].Lopen = {0,height/5.0};
 door[8].Lclosed = {0,height/5.0};
 door[8].Ropen = {0,3*height/10.0};
 door[8].Rclosed = {0,3*height/10.0};

// Door 6 (left-bottom)
door[9].level = 5;
 door[9].Lopen = {0,3*height/5.0};
 door[9].Lclosed = {0,3*height/5.0};
 door[9].Ropen = {0,7*height/10.0};
 door[9].Rclosed = {0,7*height/10.0};

// Door 7 (right-top)
door[10].level = 5;
 door[10].Lopen = {97*width/100.0,height/5.0};
 door[10].Lclosed = {90*width/100.0,height/5.0};
 door[10].Ropen = {97*width/100.0,3*height/10.0};
 door[10].Rclosed = {90*width/100.0,3*height/10.0};

// Door 8 (right-bottom)
 door[11].level = 5;
 door[11].Lopen = {97*width/100.0,3*height/5.0};
 door[11].Lclosed = {97*width/100.0,3*height/5.0};
 door[11].Ropen = {97*width/100.0,7*height/10.0};
 door[11].Rclosed = {97*width/100.0,7*height/10.0};
}

bool unlock_door_level1;
bool unlock_door1_level2;
bool unlock_door2_level2;
bool unlock_door_level3;
bool unlock_door_level4;
bool unlock_doors_level5;

Texture2D player_right_texture;
Texture2D player_left_texture;
Texture2D player_front_texture;
Texture2D player_back_texture;
Texture2D player_back_left_texture;
Texture2D player_back_right_texture;
Texture2D player_front_right_texture;
Texture2D player_front_left_texture;
Texture2D floor_texture;
Texture2D door_closed_texture;
Texture2D door_open_texture;
Texture2D button_texture;
Texture2D horizontal_wall_texture;
Texture2D vertical_wall_texture;

void Textures(){
  Image floor = LoadImage("floor.png");
  ImageResize(&floor,width/10,height/10);
  floor_texture = LoadTextureFromImage(floor);
  UnloadImage(floor);

  Image door_closed = LoadImage("door_closed.png");
  ImageResize(&door_closed,width/10,height/50);
  door_closed_texture = LoadTextureFromImage(door_closed);
  UnloadImage(door_closed);

  Image door_open = LoadImage("door_open.png");
  ImageResize(&door_open,width/50,height/10);
  door_open_texture = LoadTextureFromImage(door_open);
  UnloadImage(door_open);

  Image button = LoadImage("key.png");
  ImageResize(&button,width/10,height/10);
  button_texture = LoadTextureFromImage(button);
  UnloadImage(button);

  Image player_front = LoadImage("Character_front_view.png");
  ImageResize(&player_front,width/10,height/10);
  player_front_texture = LoadTextureFromImage(player_front);
  UnloadImage(player_front);

  Image player_back = LoadImage("Character_back_view.png");
  ImageResize(&player_back,width/10,height/10);
  player_back_texture = LoadTextureFromImage(player_back);
  UnloadImage(player_back);

  Image player_left = LoadImage("Character_leftside_view.png");
  ImageResize(&player_left,width/10,height/10);
  player_left_texture = LoadTextureFromImage(player_left);
  UnloadImage(player_left);

  Image player_right = LoadImage("Character_rightside_view.png");
  ImageResize(&player_right,width/10,height/10);
  player_right_texture = LoadTextureFromImage(player_right);
  UnloadImage(player_right);

  Image player_front_right = LoadImage("Character_frontright_view.png");
  ImageResize(&player_front_right,width/10,height/10);
  player_front_right_texture = LoadTextureFromImage(player_front_right);
  UnloadImage(player_front_right);

  Image player_front_left = LoadImage("Character_frontleft_view.png");
  ImageResize(&player_front_left,width/10,height/10);
  player_front_left_texture = LoadTextureFromImage(player_front_left);
  UnloadImage(player_front_left);

  Image player_back_right = LoadImage("Character_backright_view.png");
  ImageResize(&player_back_right,width/10,height/10);
  player_back_right_texture = LoadTextureFromImage(player_back_right);
  UnloadImage(player_back_right);

  Image player_back_left = LoadImage("Character_backleft_view.png");
  ImageResize(&player_back_left,width/10,height/10);
  player_back_left_texture = LoadTextureFromImage(player_back_left);
  UnloadImage(player_back_left);
  
  Image horizontal_wall = LoadImage("wall_h.png");
  ImageResize(&horizontal_wall,width/5,height/3);
  horizontal_wall_texture = LoadTextureFromImage(horizontal_wall);
  UnloadImage(horizontal_wall);

  Image vertical_wall = LoadImage("wall_v.png");
  ImageResize(&vertical_wall,width/3,height/5);
  vertical_wall_texture = LoadTextureFromImage(vertical_wall);
  UnloadImage(vertical_wall);
  
}

void scene(Texture2D background){
  for(int i = 0; i < width; i += (int)width/10){
    for(int j = 0; j < height; j += (int)height/10){
      DrawTexture(background,i,j,WHITE);
    }
  }
}

bool CheckCollision(Rectangle a, Rectangle b){
    return a.x < b.x + b.width &&
           a.x + a.width > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}

void mechanics(){
  if(level == 1){
    Button button;
    button.x = 7*width/8.0;
    button.y = 0.0;
    button.Bheight = height/15.0;
    button.Bwidth = width/15.0;
    bool is_on_button[max_time_loops];
    
    DrawTexture(button_texture,button.x,button.y,WHITE);
    
    unlock_door_level1 = false;
  
   for(int i = 0; i < current_time_loop + 1; i++){
	is_on_button[i] = (x_ps[i] >= button.x - button.Bwidth && x_ps[i] <= button.x + button.Bwidth && y_ps[i] >= button.y - button.Bheight && y_ps[i] <= button.y + button.Bheight);
      
      if(is_on_button[i]){
	PlaySound(hit_button);
	unlock_door_level1 = true;
      }
   }

   door[0].unlocked = unlock_door_level1;
   DrawDoor(door[0], door_open_texture, door_closed_texture, door_open);

     for(int i = 0; i < current_time_loop + 1; i++){
     	if((x_ps[i] >= door[0].Lclosed.x && x_ps[i] <= door[0].Rclosed.x && y_ps[i] <= height/10000) && !unlock_door_level1){
	DrawText("Hmm... This door is locked...How can I escape?",x_ps[i],y_ps[i]+100,15,BLACK);
      }

	else if((x_ps[i] >= door[0].Lclosed.x && x_ps[i] <= door[0].Rclosed.x && y_ps[i] <= height/10000) && unlock_door_level1){
	current_time_loop = 0;
	unlock_door_level1 = false;
	level++;
      }
      }
}

  if(level == 2){
    Button button1;
    Button button2;
    button1.x = 7*width/8.0;
    button1.y = 7*height/8.0;
    button2.x = width/2.0;
    button2.y = height/2.0;
    button1.Bheight = height/15.0;
    button1.Bwidth = width/15.0;
    button2.Bheight = height/15.0;
    button2.Bwidth = width/15.0;
    bool is_on_button1[max_time_loops];
    bool is_on_button2[max_time_loops];

    Wall wall;
    wall.x = button1.x-width/20;
    wall.y = button1.y-4*height/25;
    wall.height = 2*height/12.0;
    wall.width = 3*width/3.0;

    for(int i = 0; i < current_time_loop + 1; i++){
    Rectangle player_rect = {x_ps[i], y_ps[i], char_width, char_height};
}
	 
    DrawTexture(button_texture,button1.x,button1.y,WHITE);
    DrawTexture(button_texture,button2.x,button2.y,WHITE);

    unlock_door1_level2 = false;
    unlock_door2_level2 = false;
    
      for(int i = 0; i < current_time_loop + 1; i++){
	is_on_button1[i] = (x_ps[i] >= button1.x - button1.Bwidth && x_ps[i] <= button1.x + button1.Bwidth && y_ps[i] >= button1.y - button1.Bheight && y_ps[i] <= button1.y + button1.Bheight);
	is_on_button2[i] = (x_ps[i] >= button2.x - button2.Bwidth && x_ps[i] <= button2.x + button2.Bwidth && y_ps[i] >= button2.y - button2.Bheight && y_ps[i] <= button2.y + button2.Bheight);
      
      if(is_on_button1[i]){
	PlaySound(hit_button);
	unlock_door2_level2 = true;
      }

      if(is_on_button2[i]){
	PlaySound(hit_button);
	unlock_door1_level2 = true;
      }
      }
      
      if(!unlock_door1_level2){
    DrawTexture(horizontal_wall_texture,wall.x,wall.y-height/50,WHITE);
    DrawTexture(vertical_wall_texture,wall.x-wall.width/4+19*width/200,wall.y+13*height/100,WHITE);
    }
	
      for(int i = 0; i < current_time_loop + 1; i++){
	if((x_ps[i] >= door[1].Lclosed.x && x_ps[i] <= door[1].Rclosed.x && y_ps[i] <= height/10000) && !unlock_door2_level2){
	DrawText("Hmm... This door is locked...How can I escape?",x_ps[i],y_ps[i]+100,15,BLACK);
      }

	else if((x_ps[i] >= door[1].Lclosed.x && x_ps[i] <= door[1].Rclosed.x && y_ps[i] <= height/10000) && unlock_door2_level2){
	  current_time_loop = 0;
	  unlock_door2_level2 = false;
	  unlock_door1_level2 = false;
	  level++;
      }
      }

   door[1].unlocked = unlock_door2_level2;
   DrawDoor(door[1], door_open_texture, door_closed_texture, door_open);

  }

  if(level == 3){
    if(!RNG_init_clones){
      RNGclones1 = 1 + rand()%5;
      RNGclones2 = 1 + rand()%5;
      button1_clones = RNGclones1;
      button2_clones = RNGclones2;
      RNG_init_clones = true;
    }

    bool is_on_button1[max_time_loops];
    bool is_on_button2[max_time_loops];
    static bool button1_used[max_time_loops] = {false};
    static bool button2_used[max_time_loops] = {false};
    
     for(int i = 0; i < current_time_loop + 1; i++){
     	if((x_ps[i] >= door[2].Lclosed.x && x_ps[i] <= door[2].Rclosed.x && y_ps[i] <= height/10000) && !unlock_door_level3){
	DrawText("Hmm... This door is locked...How can I escape?",x_ps[i],y_ps[i]+100,15,BLACK);
      }

	else if((x_ps[i] >= door[2].Lclosed.x && x_ps[i] <= door[2].Rclosed.x && y_ps[i] <= height/10000) && unlock_door_level3){
	current_time_loop = 0;
        RNG_init_clones = false;
	unlock_door_level3 = false;
	level++;
	clone_active[0] = true;
	clone_start_time[0] = clock();
      }
      }

     Button button1;
     Button button2;
     button1.x = width/4.0;
     button1.y = height/2.0;
     button2.x = 3*width/4.0;
     button2.y = height/2.0;
     button1.Bheight = height/15.0;
     button1.Bwidth = width/15.0;
     button2.Bheight = height/15.0;
     button2.Bwidth = width/15.0;

     DrawTexture(button_texture,button1.x,button1.y,WHITE);
     DrawText(to_string(button1_clones).c_str(),button1.x,button1.y,15,BLACK);
     DrawTexture(button_texture,button2.x,button2.y,WHITE);
     DrawText(to_string(button2_clones).c_str(),button2.x,button2.y,15,BLACK);
     
     for(int i = 0; i < current_time_loop + 1; i++){
    is_on_button1[i] = (x_ps[i] >= button1.x - button1.Bwidth && x_ps[i] <= button1.x + button1.Bwidth && y_ps[i] >= button1.y - button1.Bheight && y_ps[i] <= button1.y + button1.Bheight);

    is_on_button2[i] = (x_ps[i] >= button2.x - button2.Bwidth && x_ps[i] <= button2.x + button2.Bwidth && y_ps[i] >= button2.y - button2.Bheight && y_ps[i] <= button2.y + button2.Bheight);
}
     
if(is_on_button1[current_time_loop] && !button1_used[current_time_loop] && button1_clones > 0){
  PlaySound(hit_button);
    button1_clones--;
    button1_used[current_time_loop] = true;
}

if(is_on_button2[current_time_loop] && !button2_used[current_time_loop] && button2_clones > 0){
  PlaySound(hit_button);
    button2_clones--;
    button2_used[current_time_loop] = true;
}
	
if(button1_clones == 0 && button2_clones == 0){
	unlock_door_level3 = true;
	RNG_init_clones = false;
      }

    door[2].unlocked = unlock_door_level3;
   DrawDoor(door[2], door_open_texture, door_closed_texture, door_open);

}

  if(level == 4){
    if(!lvl4_clone0_initialized){
    clone_start_time[0] = clock();
    clone_lifetime[0] = 1 + rand() % 2; 
    clone_active[0] = true;
    lvl4_clone0_initialized = true;
}

    bool is_on_button[max_time_loops];
    Button button;
    button.x = width/2.0;
    button.y = height/2.0;
    button.Bheight = height/15.0;
    button.Bwidth = width/15.0;

    DrawTexture(button_texture,button.x,button.y,WHITE);

    unlock_door_level4 = false;
  
   for(int i = 0; i < current_time_loop + 1; i++){
	is_on_button[i] = (x_ps[i] >= button.x - button.Bwidth && x_ps[i] <= button.x + button.Bwidth && y_ps[i] >= button.y - button.Bheight && y_ps[i] <= button.y + button.Bheight);
      
      if(is_on_button[i]){
	PlaySound(hit_button);
	unlock_door_level4 = true;
      }
   }

     for(int i = 0; i < current_time_loop + 1; i++){
     	if((x_ps[i] >= door[3].Lclosed.x && x_ps[i] <= door[3].Rclosed.x && y_ps[i] <= height/10000) && !unlock_door_level4){
	DrawText("Hmm... This door is locked...How can I escape?",x_ps[i],y_ps[i]+100,15,BLACK);
      }

	else if((x_ps[i] >= door[3].Lclosed.x && x_ps[i] <= door[3].Rclosed.x && y_ps[i] <= height/10000) && unlock_door_level4){
	  current_time_loop = 0;
	  unlock_door_level4 = false;
	  lvl4_clone0_initialized = false;
	  level++;
	}
      }

     for(int i = 0; i < current_time_loop + 1; i++){
       if(!clone_active[i]){
	 x_ps[i] = -1000;
	 y_ps[i] = -1000;
       }
       
    double elapsed = (double)(clock() - clone_start_time[i])/CLOCKS_PER_SEC;

    if(elapsed >= clone_lifetime[i]){
        clone_active[i] = false;
        continue;
    }
    
    string timer_str = to_string((int)(clone_lifetime[i] - elapsed));
    DrawText(timer_str.c_str(), x_ps[i], y_ps[i] - 20, 15, BLACK);
}

     door[3].unlocked = unlock_door_level4;
   DrawDoor(door[3], door_open_texture, door_closed_texture, door_open);

  }

  if(level == 5){
    if(!RNG_init_door){
      RNGdoor = 1 + rand()%8;
      RNG_init_door = true;
    }
    
    Button button;
    button.x = width/2.0;
    button.y = height/2.0;
    button.Bheight = height/15.0;
    button.Bwidth = width/15.0;
    bool is_on_button[max_time_loops];
    
    DrawTexture(button_texture,button.x,button.y,WHITE);
    
    unlock_doors_level5 = false;
    
   for(int i = 0; i < current_time_loop + 1; i++){
	is_on_button[i] = (x_ps[i] >= button.x - button.Bwidth && x_ps[i] <= button.x + button.Bwidth && y_ps[i] >= button.y - button.Bheight && y_ps[i] <= button.y + button.Bheight);
      
      if(is_on_button[i]){
	PlaySound(hit_button);
	unlock_doors_level5 = true;
      }
   }
   
for(int i = 0; i < current_time_loop + 1; i++){
  for(int j = 4; j < Num_of_doors; j++){
    if(((x_ps[i] > door[j].Lclosed.x && x_ps[i] < door[j].Rclosed.x) && y_ps[1] <= 0.001) || ((x_ps[i] > door[j].Lclosed.x && x_ps[i] < door[j].Rclosed.x) && y_ps[1] >= 99*height/100) || ((y_ps[i] > door[j].Lopen.x && y_ps[i] < door[j].Ropen.x) && x_ps[1] <= 0.001) || ((y_ps[i] > door[j].Lopen.x && y_ps[i] < door[j].Ropen.x) && x_ps[1] >= 99*height/100) && !unlock_doors_level5){

      if(y_ps[i] <= 0.001){
        DrawText("Hmm... This door is locked... How can I escape?", x_ps[i], y_ps[i] + 100, 15, BLACK);
    }
      else if(y_ps[i] >= 99*height/100){
	 DrawText("Hmm... This door is locked... How can I escape?", x_ps[i], y_ps[i], 15, BLACK);
    }
      else{
	 DrawText("Hmm... This door is locked... How can I escape?", x_ps[i], y_ps[i], 15, BLACK);
      }
      }
    }
      }

 for(int j = 4; j < Num_of_doors; j++){
    door[j].unlocked = unlock_doors_level5;
    if(j >= 4 && j <= 7){
    DrawDoor(door[j], door_open_texture, door_closed_texture, door_open);
}
    else if(j >= 8){
      DrawDoor(door[j], door_closed_texture, door_open_texture, door_open);
    }
 }

 bool hit_door = false;
 
 if(unlock_doors_level5){
   for(int i = 0; i < current_time_loop + 1; i++){
     if(x_ps[i] <= 0.01 && (y_ps[i] > door[8].Lopen.y && y_ps[i] < door[8].Ropen.y)){
       hit_door = true;
     }
     
     if(x_ps[i] <= 0.01 && (y_ps[i] > door[9].Lopen.y && y_ps[i] < door[9].Ropen.y)){
       hit_door = true;
     }

     if(x_ps[i] >= 9*width/10 && (y_ps[i] > door[10].Lopen.y && y_ps[i] < door[10].Ropen.y)){
       hit_door = true;
     }
   
     if(x_ps[i] >= 9*width/10 && (y_ps[i] > door[11].Lopen.y && y_ps[i] < door[11].Ropen.y)){
      hit_door = true;
   }

   if(y_ps[i] >= 9*height/10 && (x_ps[i] > door[9].Lopen.y && x_ps[i] < door[9].Ropen.y)){
     hit_door = true;
   }

   if(y_ps[i] >= 9*height/10 && (x_ps[i] > door[6].Lopen.x && x_ps[i] < door[6].Ropen.x)){
     hit_door = true;
   }

   if(y_ps[i] <= 0.01 && (x_ps[i] > door[4].Lopen.x && x_ps[i] < door[1].Ropen.x)){
     hit_door = true;
   }

   if(y_ps[i] <= 0.01 && (x_ps[i] > door[5].Lopen.x && x_ps[i] < door[5].Ropen.x)){
     hit_door = true;
   }

   if(hit_door){
 switch(RNGdoor){
 case 1:
   y_ps[i] = 93*height/100.0f;
   x_ps[i] = 3*width/5.0f;
   break;
 case 2:
   y_ps[i] = 97*height/100.0f;
   x_ps[i] = 3*width/5.0f;
   break;
 case 3:
   BeginDrawing();
   ClearBackground(RAYWHITE);
   DrawText("Congratulations! You got out!",width/2.0,height/2.0,15,BLACK);
   EndDrawing();
   WaitTime(3.0);
   CloseWindow();
   break;
 case 4:
   y_ps[i] = 10;
   x_ps[i] = width/5.0f;
   break;
 case 5:
   unlock_doors_level5 = false;
   RNG_init_door = false;
   level = 4;
   current_time_loop = 0;
   break;
 case 6:
   unlock_doors_level5 = false;
   RNG_init_door = false;
   level = 2;
   current_time_loop = 0;
   break;
 case 7:
   unlock_doors_level5 = false;
   RNG_init_door = false;
   level = 3;
   button1_clones = 3;
   button2_clones = 2;
   current_time_loop = 0;
   break;
 case 8:
   unlock_doors_level5 = false;
   RNG_init_door = false;
   level = 1;
   current_time_loop = 0;
   break;
 }
}
   }
}
}
}

void movement(Texture2D player_left,Texture2D player_right,Texture2D player_front,Texture2D player_back, Texture2D player_back_left, Texture2D player_back_right, Texture2D player_front_right, Texture2D player_front_left){
    Rectangle player_next = {
        .x = x_ps[current_time_loop],
        .y = y_ps[current_time_loop],
        .width = char_width,
        .height = char_height
    };

    if(level == 1){
      float speed = 1.5f;
      float dx = 0.0f;
      float dy = 0.0f;
      
      if(IsKeyDown(KEY_UP)){
	dy -= 1.0f;
      }

      if(IsKeyDown(KEY_DOWN)){
	dy += 1.0f;
      }

      if(IsKeyDown(KEY_LEFT)){
	dx -= 1.0f;
      }

      if(IsKeyDown(KEY_RIGHT)){
	dx += 1.0f;
    }

      float norm = sqrt(dx*dx + dy*dy);
      
      if(norm != 0){
	dx /= norm;
	dy /= norm;
      }

      x_ps[current_time_loop] += dx*speed;
      y_ps[current_time_loop] += dy*speed;
    }

    if(level == 2){
        Button button;
        button.x = 7*width/8.0;
        button.y = 7*height/8.0;
        button.Bwidth = width/15.0;
        button.Bheight = height/15.0;

        Wall wall;
        wall.x = 7*width/8.0;
        wall.y = button.y + 5*height/1000;
        wall.width = width/10.0;
        wall.height = height/3.0;

        Rectangle wall_rect = { wall.x, wall.y, wall.width, wall.height };

	float speed = 1.5f;
	float dx = 0.0f;
	float dy = 0.0f;
      
      if(IsKeyDown(KEY_UP)){
	Rectangle test = player_next;
            test.y -= 1.5f;
            if(unlock_door1_level2 || !CheckCollision(test, wall_rect)){
                dy -= 1.0f;
            }
      }

      if(IsKeyDown(KEY_DOWN)){
	Rectangle test = player_next;
            test.y += 1.5f;
            if(unlock_door1_level2 || !CheckCollision(test, wall_rect)){
                dy += 1.0f;
            }
      }

      if(IsKeyDown(KEY_LEFT)){
	Rectangle test = player_next;
            test.x -= 1.5f;
            if(unlock_door1_level2 || !CheckCollision(test, wall_rect)){
                dx -= 1.0f;
            }
      }

      if(IsKeyDown(KEY_RIGHT)){
	Rectangle test = player_next;
            test.x += 1.5f;
            if(unlock_door1_level2 || !CheckCollision(test, wall_rect)){
                dx += 1.0f;
            }
    }

      float norm = sqrt(dx*dx + dy*dy);
      
      if(norm != 0){
	dx /= norm;
	dy /= norm;
      }

      x_ps[current_time_loop] += dx*speed;
      y_ps[current_time_loop] += dy*speed;
    }

    if(level == 3){
      float speed = 1.5f;
      float dx = 0.0f;
      float dy = 0.0f;
      
      if(IsKeyDown(KEY_UP)){
	dy -= 1.0f;
      }

      if(IsKeyDown(KEY_DOWN)){
	dy += 1.0f;
      }

      if(IsKeyDown(KEY_LEFT)){
	dx -= 1.0f;
      }

      if(IsKeyDown(KEY_RIGHT)){
	dx += 1.0f;
    }

      float norm = sqrt(dx*dx + dy*dy);
      
      if(norm != 0){
	dx /= norm;
	dy /= norm;
      }

      x_ps[current_time_loop] += dx*speed;
      y_ps[current_time_loop] += dy*speed;
    }

        if(level == 4){
      float speed = 1.5f;
      float dx = 0.0f;
      float dy = 0.0f;
      
      if(IsKeyDown(KEY_UP)){
	dy -= 1.0f;
      }

      if(IsKeyDown(KEY_DOWN)){
	dy += 1.0f;
      }

      if(IsKeyDown(KEY_LEFT)){
	dx -= 1.0f;
      }

      if(IsKeyDown(KEY_RIGHT)){
	dx += 1.0f;
    }

      float norm = sqrt(dx*dx + dy*dy);
      
      if(norm != 0){
	dx /= norm;
	dy /= norm;
      }

      x_ps[current_time_loop] += dx*speed;
      y_ps[current_time_loop] += dy*speed;
    }

	if(level == 5){
      float speed = 1.5f;
      float dx = 0.0f;
      float dy = 0.0f;
      
      if(IsKeyDown(KEY_UP)){
	dy -= 1.0f;
      }

      if(IsKeyDown(KEY_DOWN)){
	dy += 1.0f;
      }

      if(IsKeyDown(KEY_LEFT)){
	dx -= 1.0f;
      }

      if(IsKeyDown(KEY_RIGHT)){
	dx += 1.0f;
    }

      float norm = sqrt(dx*dx + dy*dy);
      
      if(norm != 0){
	dx /= norm;
	dy /= norm;
      }

      x_ps[current_time_loop] += dx*speed;
      y_ps[current_time_loop] += dy*speed;
    }
    
    if(x_ps[current_time_loop] < 0){
      x_ps[current_time_loop] = 0;
    }
    
    if(x_ps[current_time_loop] > width - char_width){
      x_ps[current_time_loop] = width - char_width;
    }
    
    if(y_ps[current_time_loop] < 0){
      y_ps[current_time_loop] = 0;
    }
    
    if(y_ps[current_time_loop] > height - char_height){
      y_ps[current_time_loop] = height - char_height;
    }
    
    x_history[current_time_loop][frame_count] = x_ps[current_time_loop];
    y_history[current_time_loop][frame_count] = y_ps[current_time_loop];

    if(IsKeyPressed(KEY_SPACE) && current_time_loop < max_time_loops){
      DrawText("Going into a time loop...",width/2.0,height/2.0, 15, BLACK);
      PlaySound(warp);
        current_time_loop++;
        x_ps[current_time_loop] = rand()%width;
        y_ps[current_time_loop] = rand()%height;
        x_history[current_time_loop][frame_count] = x_ps[current_time_loop];
        y_history[current_time_loop][frame_count] = y_ps[current_time_loop];

	if(level == 4){;
	  clone_lifetime[0] = 1 + rand() % 2;
	  clone_start_time[current_time_loop] = clock();
	  clone_lifetime[current_time_loop] = 1 + rand() % 2; 
	  clone_active[current_time_loop] = true;
	}
    }

    for(int i = 0; i < current_time_loop; i++){
      if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT)){
    DrawTexture(player_front_right, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT)){
    DrawTexture(player_front_left, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT)){
    DrawTexture(player_back_right, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT)){
    DrawTexture(player_back_left, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_DOWN)){
    DrawTexture(player_front, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_UP)){
    DrawTexture(player_back, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_LEFT)){
    DrawTexture(player_left, x_ps[i], y_ps[i], WHITE);
	}
     else if(IsKeyDown(KEY_RIGHT)){
    DrawTexture(player_right, x_ps[i], y_ps[i], WHITE);
	}
      else{
	DrawTexture(player_front, x_ps[i], y_ps[i], WHITE);
      }
    }

    if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT)){
    DrawTexture(player_front_right, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
      else if(IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT)){
    DrawTexture(player_front_left, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
      else if(IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT)){
    DrawTexture(player_back_right, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
      else if(IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT)){
    DrawTexture(player_back_left, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
    
    else if(IsKeyDown(KEY_DOWN)){
    DrawTexture(player_front, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
    else if(IsKeyDown(KEY_UP)){
    DrawTexture(player_back, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
    else if(IsKeyDown(KEY_LEFT)){
    DrawTexture(player_left, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
      else if(IsKeyDown(KEY_RIGHT)){
    DrawTexture(player_right, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
	}
      
      else{
	DrawTexture(player_front, x_ps[current_time_loop], y_ps[current_time_loop], WHITE);
      }
}

int run_game(){
  
  InitWindow(width,height, "Stuck in a time loop!");
  InitAudioDevice();
  Init_doors();
  srand(time(NULL));
  
  door_open = LoadSound("door_open.mp3");
  game_music = LoadMusicStream("game_music.mp3");
  hit_button = LoadSound("hit_button.mp3");
  warp = LoadSound("warp.mp3");
  game_music.looping = true;
  PlayMusicStream(game_music);
    
  SetTargetFPS(60);

  Textures();

  x_ps[current_time_loop] = x_p;
  y_ps[current_time_loop] = y_p;
 
  while(!WindowShouldClose()){
      UpdateMusicStream(game_music);
      ClearBackground(BLACK);

      BeginDrawing();
      scene(floor_texture);
      DrawText(TextFormat("Level: %i",level),10,10,50,BLACK);
      DrawText(TextFormat("Clones: %i",current_time_loop),10,0.9*height,50,BLACK);
      mechanics();
      movement(player_left_texture,player_right_texture,player_front_texture,player_back_texture,player_back_left_texture,player_back_right_texture,player_front_right_texture, player_front_left_texture);
      frame_count++;
      EndDrawing();
}
  
CloseWindow();
  return 0; 
}
