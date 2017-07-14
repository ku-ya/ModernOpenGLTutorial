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

using namespace std;
using namespace glm;

vector<vec3> getPosition(vec3 pos, float voxel_size)
{
	float half_size = voxel_size /2;
	vector<vec3> translate;
	translate.push_back(vec3(-half_size + pos.x, -half_size + pos.y, half_size + pos.z));
	translate.push_back(vec3(-half_size + pos.x, half_size + pos.y, half_size + pos.z));
	translate.push_back(vec3(half_size + pos.x, half_size + pos.y, half_size + pos.z));
	translate.push_back(vec3(half_size + pos.x, -half_size + pos.y, half_size + pos.z));
	translate.push_back(vec3(-half_size + pos.x, -half_size + pos.y, -half_size + pos.z));
	translate.push_back(vec3(-half_size + pos.x, half_size + pos.y, -half_size + pos.z));
	translate.push_back(vec3(half_size + pos.x, half_size + pos.y, -half_size + pos.z));
	translate.push_back(vec3(half_size + pos.x, -half_size + pos.y, -half_size + pos.z));
	return translate;
};

vector<vec3> normal =
{
	vec3(0.0, 0.0, 1.0),
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, -1.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, -1.0),
	vec3(-1.0, 0.0, 0.0)
};

std::vector<Vertex> cube_vertex;
std::vector<unsigned int> cube_index;

void quad( int a, int b, int c, int d, int face, vector<vec3> positions, vec2 texture, vec4 color)
{
	cube_index.push_back(a);cube_index.push_back(b);cube_index.push_back(c);
	cube_index.push_back(a);cube_index.push_back(c);cube_index.push_back(d);

	cube_vertex.push_back(Vertex(positions[a], texture, normal[face], color));
	cube_vertex.push_back(Vertex(positions[b], texture, normal[face], color));
	cube_vertex.push_back(Vertex(positions[c], texture, normal[face], color));

	cube_vertex.push_back(Vertex(positions[a], texture, normal[face], color));
	cube_vertex.push_back(Vertex(positions[c], texture, normal[face], color));
	cube_vertex.push_back(Vertex(positions[d], texture, normal[face], color));
};



void colorcube(vec3 min, vec3 range, float grid_size){
	vec3 N(range.x/grid_size, range.y/grid_size,range.z/grid_size);
	cube_vertex.reserve(N.x * N.y * N.z);
	vec2 texture;
	vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
	float scale = grid_size;
	vector<vec3> pos = getPosition(vec3(0,0,0), 0.2);
	for(int i = 0; i < N.z; ++i){
		for(int j = 0; j < N.y; ++j){
			for(int k = 0; k < N.x; ++k){
			pos = getPosition(vec3(range.x * (float)k/N.x + min.x,
				range.y * (float)j/N.y + min.y, range.z * (float)i/N.z + min.z), scale*0.7);
			color.x = (float)j/N.y;
			color.y = (float)k/N.x;
			quad(1,0,3,2,0, pos, texture, color); // front
			quad(2,3,7,6,1, pos, texture, color); // right
			quad(3,0,4,7,2, pos, texture, color); // bottom
			quad(6,5,1,2,3, pos, texture, color); // top
			quad(4,5,6,7,4, pos, texture, color); // back
			quad(5,4,0,1,5, pos, texture, color); // left
			}
		}
	}

};

int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	std::vector<Vertex> vert_vec;
	std::vector<unsigned int> idx_vec;

	std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 1; i < argc; ++i) {
        std::cout << atof(argv[i]) << std::endl;
    }
	// exit(0);

	float x_min = atof(argv[1]), y_min = atof(argv[2]), z_min = atof(argv[3]);
	float grid_size = atof(argv[4]);
	float L_x = atof(argv[5]), L_y = atof(argv[6]), L_z = atof(argv[7]);
	int N = round(L_x/grid_size);
	int N_z = 1;
	std::vector<glm::vec3> voxel_pos;
	voxel_pos.reserve(N_z*N*N);
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


	// int i_test = 0;
	// for(auto position: voxel_pos)
	// {
	// 	vertex.pos = position + glm::vec3(L_x * (float)i_test/N + x_min, y_min, 0.0);
	// 	vert_vec.push_back(vertex);
	// 	vertex.pos.y = L_y + y_min;
	// 	vert_vec.push_back(vertex);
	//
	// 	vertex.pos = position + glm::vec3(x_min, L_y * (float)i_test/N + x_min, 0.0);
	// 	vert_vec.push_back(vertex);
	// 	vertex.pos.y = L_x + x_min;
	// 	vert_vec.push_back(vertex);
	//
	// 	i_test++;
	//
	// }

	colorcube(vec3(x_min, y_min, z_min), vec3(L_x, L_y, L_z), grid_size);

	Mesh mesh(vert_vec, vert_vec.size(), idx_vec, idx_vec.size());
	Mesh cube(cube_vertex, cube_vertex.size(), cube_index, cube_index.size());
	//Mesh monkey("./res/monkey3.obj");
	Shader shader("./res/basicShader");
	Shader cube_shader("./res/cubeShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;
	Camera camera(glm::vec3(0.0f, 2.0f, -20.0f), 70.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	vector<vec4> color_RGBA;
	color_RGBA.reserve(cube_vertex.size());
	int N_v = cube_vertex.size();
	for(int i = 0; i < cube_vertex.size(); ++i)
	{
		color_RGBA.push_back(vec4((float)i/N_v,1.0 - (float)i/N_v,1.0,1.0));
	}
	cout<<cube_vertex.size()<<'\n';
	cout<<color_RGBA.size()<<'\n';
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
		transform.GetRot()->x = -90;
	 	transform.GetRot()->y = 0;
		transform.GetRot()->z = counter * 1;
		shader.Update(transform, camera);
		// monkey.Draw();
		mesh.Draw();
		auto colorMem = cube.getColorMem();
		for(int i = 0; i < color_RGBA.size(); ++i)
		{
			// color_RGBA[i].w = (sin(counter * 10)+1)/2;
			colorMem[i].x = 1;
			colorMem[i].y = 0;
			colorMem[i].z = (sin(counter * 10)+1)/2;
			colorMem[i].w = (sin(counter * 10)+1)/2;
		}

		cube_shader.Bind();
		cube_shader.Update(transform, camera);
		// cube.Update_value(color_RGBA, color_RGBA.size());
		cube.Draw_cube();

		display.SwapBuffers();
		SDL_Delay(1);
		counter += 0.01f;
	}

	return 0;
}
