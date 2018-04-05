
class Maze
{
    float gridRows, gridCols;
    float mazeX, mazeY, mazeZ;
    float wallThickness;

    int currentX;
    int currentY;
    float currentAngle;

    int* mazeLayout;

	unsigned int vaoHandle;
	void createVAO();
public:
    Maze(int gridRows, int gridCols, int* mazeLayout);
    void renderWalls(int shaderID);
    void renderGoal(int shaderID);
    void SetPosition(int currentX, int currentY, float currentAngle);
    bool IsCollision(int i, int j);
    int GetCurrentX();
    int GetCurrentY();
    float GetCurrentAngle();
};
