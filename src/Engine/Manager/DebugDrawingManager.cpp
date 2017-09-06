#include "DebugDrawingManager.hpp"

#include "../Entity/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Lens.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../MainWindow.hpp"
#include "Managers.hpp"
#include "ResourceManager.hpp"
#include <Video/Shader/Shader.hpp>
#include <Video/Shader/ShaderProgram.hpp>
#include "DebugDrawing.vert.hpp"
#include "DebugDrawing.frag.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace Video;

DebugDrawingManager::DebugDrawingManager() {
    vertexShader = Managers().resourceManager->CreateShader(DEBUGDRAWING_VERT, DEBUGDRAWING_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Managers().resourceManager->CreateShader(DEBUGDRAWING_FRAG, DEBUGDRAWING_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Managers().resourceManager->CreateShaderProgram({ vertexShader, fragmentShader });
    
    // Create point vertex array.
    glBindVertexArray(0);
    
    glGenBuffers(1, &pointVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(glm::vec3), &glm::vec3(0.f, 0.f, 0.f)[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &pointVertexArray);
    glBindVertexArray(pointVertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, pointVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0));
    
    glBindVertexArray(0);
    
    // Create line vertex array.
    glm::vec3 line[2];
    line[0] = glm::vec3(0.f, 0.f, 0.f);
    line[1] = glm::vec3(1.f, 1.f, 1.f);
    
    glGenBuffers(1, &lineVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lineVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), line, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &lineVertexArray);
    glBindVertexArray(lineVertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, lineVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0));
    
    glBindVertexArray(0);
    
    // Create cuboid vertex array.
    glm::vec3 box[24];
    box[0] = glm::vec3(0.f, 0.f, 0.f);
    box[1] = glm::vec3(1.f, 0.f, 0.f);
    box[2] = glm::vec3(1.f, 0.f, 0.f);
    box[3] = glm::vec3(1.f, 1.f, 0.f);
    box[4] = glm::vec3(1.f, 1.f, 0.f);
    box[5] = glm::vec3(0.f, 1.f, 0.f);
    box[6] = glm::vec3(1.f, 1.f, 0.f);
    box[7] = glm::vec3(1.f, 1.f, 1.f);
    box[8] = glm::vec3(1.f, 1.f, 1.f);
    box[9] = glm::vec3(1.f, 0.f, 1.f);
    box[10] = glm::vec3(1.f, 0.f, 1.f);
    box[11] = glm::vec3(1.f, 0.f, 0.f);
    box[12] = glm::vec3(0.f, 1.f, 0.f);
    box[13] = glm::vec3(0.f, 1.f, 1.f);
    box[14] = glm::vec3(0.f, 1.f, 1.f);
    box[15] = glm::vec3(0.f, 0.f, 1.f);
    box[16] = glm::vec3(0.f, 1.f, 0.f);
    box[17] = glm::vec3(0.f, 0.f, 0.f);
    box[18] = glm::vec3(0.f, 0.f, 1.f);
    box[19] = glm::vec3(0.f, 0.f, 0.f);
    box[20] = glm::vec3(0.f, 1.f, 1.f);
    box[21] = glm::vec3(1.f, 1.f, 1.f);
    box[22] = glm::vec3(0.f, 0.f, 1.f);
    box[23] = glm::vec3(1.f, 0.f, 1.f);
    
    glGenBuffers(1, &cuboidVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cuboidVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), box, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &cuboidVertexArray);
    glBindVertexArray(cuboidVertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, cuboidVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0));
    
    glBindVertexArray(0);
}

DebugDrawingManager::~DebugDrawingManager() {
    glDeleteBuffers(1, &cuboidVertexBuffer);
    glDeleteVertexArrays(1, &cuboidVertexArray);
    
    glDeleteBuffers(1, &pointVertexBuffer);
    glDeleteVertexArrays(1, &pointVertexArray);
    
    glDeleteBuffers(1, &lineVertexBuffer);
    glDeleteVertexArrays(1, &lineVertexArray);
    
    Managers().resourceManager->FreeShader(vertexShader);
    Managers().resourceManager->FreeShader(fragmentShader);
    Managers().resourceManager->FreeShaderProgram(shaderProgram);
}

void DebugDrawingManager::AddPoint(const glm::vec3& position, const glm::vec3& color, float size, float duration, bool depthTesting) {
    DebugDrawing::Point point;
    point.position = position;
    point.color = color;
    point.size = size;
    point.duration = duration;
    point.depthTesting = depthTesting;
    points.push_back(point);
}

void DebugDrawingManager::AddLine(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec3& color, float width, float duration, bool depthTesting) {
    DebugDrawing::Line line;
    line.startPosition = startPosition;
    line.endPosition = endPosition;
    line.color = color;
    line.width = width;
    line.duration = duration;
    line.depthTesting = depthTesting;
    lines.push_back(line);
}

void DebugDrawingManager::AddCuboid(const glm::vec3& minCoordinates, const glm::vec3& maxCoordinates, const glm::vec3& color, float lineWidth, float duration, bool depthTesting) {
    DebugDrawing::Cuboid cuboid;
    cuboid.minCoordinates = minCoordinates;
    cuboid.maxCoordinates = maxCoordinates;
    cuboid.color = color;
    cuboid.lineWidth = lineWidth;
    cuboid.duration = duration;
    cuboid.depthTesting = depthTesting;
    cuboids.push_back(cuboid);
}

void DebugDrawingManager::Update(float deltaTime) {
    // Points.
    for (std::size_t i=0; i < points.size(); ++i) {
        if (points[i].duration < 0.f) {
            points[i] = points[points.size() - 1];
            points.pop_back();
            --i;
        } else {
            points[i].duration -= deltaTime;
        }
    }
    
    // Lines.
    for (std::size_t i=0; i < lines.size(); ++i) {
        if (lines[i].duration < 0.f) {
            lines[i] = lines[lines.size() - 1];
            lines.pop_back();
            --i;
        } else {
            lines[i].duration -= deltaTime;
        }
    }
    
    // Cuboid.
    for (std::size_t i=0; i < cuboids.size(); ++i) {
        if (cuboids[i].duration < 0.f) {
            cuboids[i] = cuboids[cuboids.size() - 1];
            cuboids.pop_back();
            --i;
        } else {
            cuboids[i].duration -= deltaTime;
        }
    }
}

void DebugDrawingManager::Render(World& world, Entity* camera) {
    // Find camera entity.
    if (camera == nullptr) {
        std::vector<Component::Lens*> lenses = world.GetComponents<Component::Lens>();
        for (Component::Lens* lens : lenses) {
            camera = lens->entity;
        }
    }
    
    if (camera != nullptr) {
        glm::mat4 viewMat(camera->GetCameraOrientation() * glm::translate(glm::mat4(), -camera->position));
        glm::mat4 projectionMat(camera->GetComponent<Component::Lens>()->GetProjection(MainWindow::GetInstance()->GetSize()));
        glm::mat4 viewProjectionMat(projectionMat * viewMat);
        
        shaderProgram->Use();
        glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjection"), 1, GL_FALSE, &viewProjectionMat[0][0]);
        
        // Points.
        glBindVertexArray(pointVertexArray);
        for (const DebugDrawing::Point& point : points) {
            glm::mat4 model(glm::translate(glm::mat4(), point.position));
            
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
            point.depthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            glUniform3fv(shaderProgram->GetUniformLocation("color"), 1, &point.color[0]);
            glUniform1f(shaderProgram->GetUniformLocation("size"), point.size);
            glDrawArrays(GL_POINTS, 0, 1);
        }
        
        // Lines.
        glBindVertexArray(lineVertexArray);
        for (const DebugDrawing::Line& line : lines) {
            glm::mat4 model(glm::translate(glm::mat4(), line.startPosition) * glm::scale(glm::mat4(), line.endPosition - line.startPosition));
            
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
            line.depthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            glUniform3fv(shaderProgram->GetUniformLocation("color"), 1, &line.color[0]);
            glLineWidth(line.width);
            glDrawArrays(GL_LINES, 0, 2);
        }
        
        // Cuboids.
        glBindVertexArray(cuboidVertexArray);
        for (const DebugDrawing::Cuboid& cuboid : cuboids) {
            glm::mat4 model(glm::translate(glm::mat4(), cuboid.minCoordinates) * glm::scale(glm::mat4(), cuboid.maxCoordinates - cuboid.minCoordinates));
            
            glUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
            cuboid.depthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            glUniform3fv(shaderProgram->GetUniformLocation("color"), 1, &cuboid.color[0]);
            glUniform1f(shaderProgram->GetUniformLocation("size"), 10.f);
            glLineWidth(cuboid.lineWidth);
            glDrawArrays(GL_LINES, 0, 24);
        }
        
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);
    }
}
