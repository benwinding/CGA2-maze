
class Maze
{
    float gridRows, gridCols;
    float mazeX, mazeY, mazeZ;

    int* mazeLayout;

	unsigned int vaoHandle;
	void createVAO(int programID);
public:
    Maze(int gridRows, int gridCols, int* mazeLayout, int id);
    void render(int programID);
};
