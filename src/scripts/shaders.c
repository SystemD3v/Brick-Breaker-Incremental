#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "shaders.h"



// Utility function to read shader source
char* readFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filepath);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

// Shader compilation
GLuint compileShader(const char* filepath, GLenum shaderType) {
    char* source = readFile(filepath);
    if (!source) return 0;

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const char**)&source, NULL);
    glCompileShader(shader);
    free(source);

    // Check for errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("Shader compilation error (%s): %s\n", filepath, log);
        return 0;
    }
    return shader;
}

// Shader program creation
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    GLuint vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("Shader linking error: %s\n", log);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void _shader_run() {

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        return;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return;
    }

    char filepath[300];
    sprintf(filepath, "../assets/shaders/shader%d", shaderSelected);

    char vertexPath[500];
    sprintf(vertexPath, "%s/vertex_shader.glsl", filepath);
    char fragmentPath[500];
    sprintf(fragmentPath, "%s/fragment_shader.glsl", filepath);


    // Compile shaders
    GLuint shaderProgram = createShaderProgram(vertexPath, fragmentPath);
    if (!shaderProgram) return;

    // Define fullscreen quad
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,   1.0f, -1.0f, 0.0f,  -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    };
    GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Get uniform locations
    GLuint iResolutionLoc = glGetUniformLocation(shaderProgram, "iResolution");
    GLuint iTimeLoc = glGetUniformLocation(shaderProgram, "iTime");

    // Main loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                running = 0;
            }
        }

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Pass uniforms
        glUniform2f(iResolutionLoc, 1920.0f, 1080.0f);  // Replace with actual screen size if different
        glUniform1f(iTimeLoc, SDL_GetTicks() / 1000.0f);  // Pass elapsed time

        // Render the quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Clean up
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    SDL_GL_DeleteContext(glContext);

    return;
}
