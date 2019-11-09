#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

const float avg_fail_level = 0.5;
const float abs_fail_level = 2.0;

int main(int argc, char** argv){
	if(argc != 2){
		std::cout << "No input file. Usage: \"*filepath*/name\" (without .ans .rans)" << std::endl;
		return 0;
	}


	std::string filename =  argv[1];
	std::string filename_ans  = filename + ".ans";
	std::string filename_rans = filename + ".rans";

	std::ifstream ans_file{filename_ans};
	
	if(!ans_file){
		std::cout << "Error: can't open file: " << filename_ans << std::endl;
		return 0;
	}

	std::ifstream rans_file{filename_rans};

	if(!rans_file){
		std::cout << "Error: can't open file: " << filename_rans << std::endl;
		return 0;
	}

	std::vector<glm::vec3> ans_pnts,rans_pnts;

	glm::vec3 pnt;
	ans_file >> pnt.x >> pnt.y >> pnt.z;
	
	while(ans_file.good()){
		ans_pnts.push_back(pnt);
		ans_file >> pnt.x >> pnt.y >> pnt.z;
	}

	rans_file >> pnt.x >> pnt.y >> pnt.z;
	while(rans_file.good()){
		rans_pnts.push_back(pnt);
		rans_file >> pnt.x >> pnt.y >> pnt.z;
	}


	int pnts_count = ans_pnts.size();
	if(pnts_count != rans_pnts.size()){
		std::cout << "Error: data size differs in input files" << std::endl;
		return 0;
	}

	std::vector<float> distances;

	float sum = 0.0f;
	float max_dif = 0.0f;

	for(int i = 0; i < pnts_count; i++){
		distances.push_back(glm::length(ans_pnts[i] - rans_pnts[i]));
		sum += distances[i];
		if(max_dif < distances[i])
			max_dif = distances[i];
	}

	float avg_dif = sum / pnts_count;
	
	sum = 0.0f;

	for(int i = 0; i < pnts_count; i++){
		sum += std::pow(avg_dif - distances[i], 2.0f);
	}

	float avg_square_error_dif = sum / pnts_count;

	
	std::cout << "Test " << filename << " analyzis:" << std::endl <<
				"	Total points: " << pnts_count << std::endl <<
				"	Maximum difference: " << max_dif << std::endl <<
				"	Average difference with right answer: " << avg_dif << " (+/-" << avg_square_error_dif << ")"<< std::endl;

	std::string conclusion = (avg_dif > avg_fail_level) ? "Test failed: average difference is too high": (max_dif > abs_fail_level) ? "Test failed: maximum difference is too high" : "Test completed!";

	std::cout << conclusion << std::endl;


}