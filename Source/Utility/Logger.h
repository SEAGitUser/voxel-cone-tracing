//
//  Logger.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 2/28/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

class Logger {

public:
    static inline void print(glm::mat4& matrix);
    static inline void print(glm::vec2& vector);
    static inline void print(glm::vec3& vector);
    static inline void print(glm::vec4& vector);
    static inline void print(glm::mat3& matrix);
};

#ifdef DEBUG
void Logger::print(glm::mat4& matrix)
{
    std::cout << "MATRIX:" << std::endl;
    std::cout << matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " " << matrix[3][0] << std::endl;
    std::cout << matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " " << matrix[3][1] << std::endl;
    std::cout << matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " " << matrix[3][2] << std::endl;
    std::cout << matrix[0][3] << " " << matrix[1][3] << " " << matrix[2][3] << " " << matrix[3][3] << std::endl;
}

void Logger::print(glm::mat3& matrix)
{
    std::cout << "MATRIX:" << std::endl;
    std::cout << matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " "  << std::endl;
    std::cout << matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " "  << std::endl;
    std::cout << matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " "  << std::endl;
}


void Logger::print(glm::vec2& vector)
{
    std::cout << glm::to_string(vector) << std::endl;
}

void Logger::print(glm::vec3& vector)
{
    std::cout << glm::to_string(vector) << std::endl;
}

void Logger::print(glm::vec4& vector)
{
    std::cout << glm::to_string(vector) << std::endl;
}

#else

void Logger::printMatrix(glm::mat4& matrix)
{}

void Logger::printVector(glm::vec2& vector)
{}

void Logger::printVector(glm::vec3& vector)
{}

void Logger::printVector(glm::vec4& vector)
{}

void Logger::print(glm::mat3& matrix)
{}

#endif
