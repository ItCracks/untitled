#ifndef TRANSFORM
#define TRANSFORM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class transform(){

public:

    void rotate(glm::vec3& axis, int& degree);
    void translate(glm::vec3& translation);
    void scale(int& scale);

    glm::vec3 GetTranslation();
    glm::vec3 GetRotationAxis();
    float GetRotationAngleDegree();
    glm::vec3 GetScale();

    void SetTranslation(glm::vec3& translation){this.}

private:
    glm::vec3 v3_translation;
    glm::vec3 v3_rotationAxis;
    float rotationAngleDegree;
    glm::vec3 v3_scale;

}

#endif // TRANSFORM

