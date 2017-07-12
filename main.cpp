#include <iostream>
#include <SDL2/SDL.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <memory>

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	std::vector<Vertex> vert_vec;
	std::vector<unsigned int> idx_vec;
	float x_min = -5.0, y_min = -5.0, grid_size = 0.5, L_x = 10, L_y = 10;
	int N = round(L_x/grid_size);
	int N_z = 50;
	std::vector<glm::vec3> voxel_pos;
	for(int k = 0; k < N_z; ++k)
	{
		for(int i = 0; i < N; ++i){
			for(int j = 0; j < N; ++j)
			{
				voxel_pos.push_back(glm::vec3(L_y * (float)j/N + x_min, L_x * (float)i/N + y_min, k*0.5));
			}
		}
	}

	Vertex vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0,0), glm::vec3(0.0f,0.0f,1.0f));
	for(int i = 0; i < N+1 ; ++i)
	{
		vertex.pos.y = y_min;
		vertex.pos.x = L_x * (float)i/N + x_min;
		vert_vec.push_back(vertex);
		vertex.pos.y = L_y + y_min;
		vert_vec.push_back(vertex);

		vertex.pos.x = x_min;
		vertex.pos.y = L_y * (float)i/N + y_min;
		vert_vec.push_back(vertex);
		vertex.pos.x = L_x + x_min;
		vert_vec.push_back(vertex);

	}

	for(unsigned int i = 0 ; i < vert_vec.size() ; i+=2)
	{
		auto idx = i;
		idx_vec.push_back(idx);
		idx_vec.push_back(idx+1);
	}

	std::vector<Vertex> cube_vertex;


	cube_vertex.push_back(Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	cube_vertex.push_back(Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)));
	cube_vertex.push_back(Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)));

	cube_vertex.push_back(Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	cube_vertex.push_back(Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
	cube_vertex.push_back(Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));

	cube_vertex.push_back(Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)));

	cube_vertex.push_back(Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)));

	cube_vertex.push_back(Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	cube_vertex.push_back(Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	cube_vertex.push_back(Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	std::vector<unsigned int> cube_index  =
								{
									0, 1, 2,0, 2, 3,
							  		6, 5, 4,7, 6, 4,
									10, 9, 8, 11, 10, 8,
									12, 13, 14,12, 14, 15,
									16, 17, 18,16, 18, 19,
									22, 21, 20, 23, 22, 20
	                          	};


	Mesh mesh(vert_vec, vert_vec.size(), idx_vec, idx_vec.size());
	Mesh cube(cube_vertex, cube_vertex.size(), cube_index, cube_index.size());
	Mesh monkey("./res/monkey3.obj");
	Shader shader("./res/basicShader");
	Shader cube_shader("./res/cubeShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;
	Camera camera(glm::vec3(0.0f, 0.0f, -15.0f), 70.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 100.0f);

	SDL_Event e;
	auto isRunning = true;
	auto counter = 0.0f;
	while(isRunning)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				isRunning = false;
		}

		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		// auto sinCounter = sinf(counter);
		// auto absSinCounter = std::abs(sinCounter);

		//transform.GetPos()->x = sinCounter;
		transform.GetRot()->y = 0;
		//transform.GetRot()->z = counter * 100;
		//transform.GetScale()->x = absSinCounter;
		//transform.GetScale()->y = absSinCounter;

		shader.Bind();
		texture.Bind();
		transform.GetPos()->x = 0;
		transform.GetPos()->y = 0;
		shader.Update(transform, camera);
		// monkey.Draw();
		mesh.Draw();
		cube_shader.Bind();
		// transform.GetPos()->x = 5;
		// cube_shader.Update(transform, camera);
		// cube.Draw_cube();

		// transform.GetRot()->y = 0;
		transform.GetRot()->y = counter * 2;
		transform.GetScale()->x = 1;
		transform.GetScale()->y = 1;
		transform.GetPos()->z = 1;

		for(auto vox_pos: voxel_pos){
			transform.GetPos()->x = vox_pos.x;
			transform.GetPos()->y = vox_pos.y;
			transform.GetPos()->z = vox_pos.z;
			cube_shader.Update(transform, camera);
			cube.Draw_cube();
		}

		display.SwapBuffers();
		SDL_Delay(1);
		counter += 0.01f;
	}

	return 0;
}
