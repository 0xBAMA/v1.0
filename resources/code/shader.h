//  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐
//  ╚═╗├─┤├─┤ ││├┤ ├┬┘
//  ╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─

#ifndef SHADER_H
#define SHADER_H

#include "includes.h"

#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;

using std::flush;
using std::endl;

class Shader
{
  public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader( const GLchar *vertexPath, const GLchar *fragmentPath, bool verbose=false)
    {

        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions ( std::ifstream::badbit );
        fShaderFile.exceptions ( std::ifstream::badbit );
        try
        {
            // Open files
            vShaderFile.open( vertexPath );
            fShaderFile.open( fragmentPath );
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf( );
            fShaderStream << fShaderFile.rdbuf( );
            // close file handlers
            vShaderFile.close( );
            fShaderFile.close( );
            // Convert stream into string
            vertexCode = vShaderStream.str( );
            fragmentCode = fShaderStream.str( );
        }
        catch ( std::ifstream::failure &e )
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }


        const GLchar *vShaderCode = vertexCode.c_str( );
        const GLchar *fShaderCode = fragmentCode.c_str( );
        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];


        // Vertex Shader
        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &vShaderCode, NULL );
        glCompileShader( vertex );

        // Print compile errors if any
        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( vertex, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }



        // Fragment Shader
        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &fShaderCode, NULL );
        glCompileShader( fragment );

        // Print compile errors if any
        glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( fragment, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        // Shader Program
        this->Program = glCreateProgram( );
        glAttachShader( this->Program, vertex );
        glAttachShader( this->Program, fragment );
        glLinkProgram( this->Program );


        // Print linking errors if any
        glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }


        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader( vertex );
        glDeleteShader( fragment );

    }
    // Uses the current shader
    void Use( )
    {
        glUseProgram( this->Program );
    }
};


class CShader //very similar to above, but for compute shader instead of vertex/fragment
{
  public:
    GLuint Program;
    // Constructor generates the shader on the fly
    CShader( const GLchar *Path, bool verbose=false)
    {

        // 1. Retrieve the compute shader source code from Path
        std::string Code;
        std::ifstream File;
        // ensures ifstream objects can throw exceptions:
        File.exceptions ( std::ifstream::badbit );
        try
        {
            // Open files
            File.open( Path );
            std::stringstream ShaderStream;
            // Read file's buffer contents into streams
            ShaderStream << File.rdbuf( );
            // close file handlers
            File.close( );
            // Convert stream into string
            Code = ShaderStream.str( );
        }
        catch ( std::ifstream::failure &e )
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const GLchar *cstrCode = Code.c_str( );
        // 2. Compile shaders
        GLuint shader;
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        shader = glCreateShader( GL_COMPUTE_SHADER );
        glShaderSource( shader, 1, &cstrCode, NULL );
        glCompileShader( shader );

        // Print compile errors if any
        glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( shader, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Shader Program
        this->Program = glCreateProgram( );
        glAttachShader( this->Program, shader );
        glLinkProgram( this->Program );

        // Print linking errors if any
        glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader( shader );

    }
    // Uses the current shader
    void Use( )
    {
        glUseProgram( this->Program );
    }
};

#endif
