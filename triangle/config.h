#pragma once
#include "lingeo3D.h"


namespace triGraphic{




// Initial parameters for MeshBuilder class

static const lingeo3D::point_t<float> INITIAL_TETRA[] = {
	lingeo3D::point_t<float>{0.0f,       0.0f, 0.81649f},
	lingeo3D::point_t<float>{0.57735f,   0.0f, 0.0f    },
	lingeo3D::point_t<float>{-0.28868f, -0.5f, 0.0f    },
	lingeo3D::point_t<float>{-0.28868f,  0.5f, 0.0f    },	
};

static const int INITIAL_SIZE = 7.5f;

static const float STRETCH_LEVEL = 0.99f; // used by MeshBuilder while cheking self-intersections

//Input arguement message constants

static const char fullscreen_msg[] = "-fullscreen";
static const char mesh_data_msg[] = "-md";
static const char random_coloring_msg[] = "-rc";



};