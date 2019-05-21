//
// Created by dominik on 21.03.19.
//

#include "Entity.h"

#include "Source/DataLayer/DataTypes/Assets/Model.h"

Entity::Entity(std::shared_ptr<Model> model, glm::mat4 **modelMatrixPtrPtr, glm::vec3 position, glm::vec3 rotation,
               glm::vec3 scale)
        : model(std::move(model)),
          modelMatrixPtrPtr(modelMatrixPtrPtr),
          position(position),
          rotation(rotation),
          scale(scale){

    auto rotationMatrix  =  glm::mat4(1.0f);
    rotationMatrix  =  glm::rotate(rotationMatrix, rotation.x, glm::vec3(1,0,0));
    rotationMatrix  *= glm::rotate(rotationMatrix, rotation.y, glm::vec3(0,1,0));
    rotationMatrix  *= glm::rotate(rotationMatrix, rotation.z, glm::vec3(0,0,1));


    **modelMatrixPtrPtr = glm::translate(**modelMatrixPtrPtr, position);
    **modelMatrixPtrPtr = **modelMatrixPtrPtr * rotationMatrix;
    **modelMatrixPtrPtr = glm::scale(**modelMatrixPtrPtr, scale);

}

const std::shared_ptr<Model> Entity::getModel() const {
    return model;
}

const glm::mat4 Entity::getModelSpaceMatrix() const {
    return **modelMatrixPtrPtr;
}

void Entity::setModel(const std::shared_ptr<Model> &model) {
    Entity::model = model;
}

void Entity::setModelMatrix(glm::mat4 modelMatrixPtrPtr) {
    **Entity::modelMatrixPtrPtr = modelMatrixPtrPtr;
}
