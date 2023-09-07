void CreateMenu();
void BackButton();
void GameScreen();
void GameFunction();
void CheckGhostCollision();
void GhostAi();
void BoundaryCollision(int prevx, int prevy);
void CreatePellets();
void PelletCollide();
bool GhostBoundaryCollision();

class Player
{
    public: 
        float x_velocity;
        float y_velocity;
        float radius;
        float color;
        int x_pos;
        int y_pos;
        float x_offest[60];
        float y_offset[60];
    Player(float x_vel, float y_vel, float radius, float color, float x_pos, float y_pos);
    void CreatePlayer();
};

void CreateMaze();

class Maze
{
    public:
        int width;
        int height;
        int x_center;
        int y_center;
    void Rect();
    void SetValues(int x_center, int y_center, int width, int height);
};

class Pellet
{
    public: 
        int x_pos;
        int y_pos;
        int radius;
    void SetValues(int x_pos, int y_pos, int radius);
    void DrawPellet();
};