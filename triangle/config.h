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





};