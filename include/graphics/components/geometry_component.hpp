#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/scene_object.hpp"
#include "graphics/shader_program.hpp"
#include "graphics/slice.hpp"
#include "graphics/textures.hpp"
#include "object_component.hpp"

namespace tomovis {

struct projection {
    projection(int id_) : id(id_), data_texture(32, 32) {
        set_orientation(glm::vec3(-4.0f, -1.0f, -1.0f),
                        glm::vec3(0.0f, 0.0f, 2.0f),
                        glm::vec3(0.0f, 2.0f, 0.0f));
    }

    projection(projection&& other) : data_texture(std::move(other.data_texture)) {
        source_position = other.source_position;
        detector_orientation = other.detector_orientation;
        parallel = other.parallel;
        id = other.id;
    }

    void set_orientation(glm::vec3 base, glm::vec3 x, glm::vec3 y) {
        float orientation_matrix[16] = {x.x,  y.x,  base.x, 0.0f,   // 1
                                        x.y,  y.y,  base.y, 0.0f,   // 2
                                        x.z,  y.z,  base.z, 0.0f,   // 3
                                        0.0f, 0.0f, 0.0f,   1.0f};  // 4
        detector_orientation =
            glm::transpose(glm::make_mat4(orientation_matrix));
    }

    glm::vec3 source_position = {4.0f, 0.0f, 0.0f};
    bool parallel = false;

    int id;
    texture<uint32_t> data_texture;
    glm::mat4 detector_orientation;
};

class GeometryComponent : public ObjectComponent {
   public:
    GeometryComponent(SceneObject& object, int scene_id);
    ~GeometryComponent();

    void draw(glm::mat4 world_to_screen) override;
    std::string identifier() const override { return "geometry"; }

    void tick(float time_elapsed) override;
    void describe() override;
    void add_projection(projection&& proj) { projections_.push_back(std::move(proj)); }

    int priority() const override { return 0; }

   private:
    SceneObject& object_;
    int scene_id_;

    GLuint vao_handle_;
    GLuint vbo_handle_;
    std::unique_ptr<ShaderProgram> program_;

    GLuint cube_vao_handle_;
    GLuint cube_vbo_handle_;
    std::unique_ptr<ShaderProgram> cube_program_;

    GLuint beam_vao_handle_;
    GLuint beam_vbo_handle_;
    std::unique_ptr<ShaderProgram> beam_program_;

    GLuint colormap_texture_;

    float speed_ = 0.0f;
    float total_time_elapsed_ = -1.0f;
    int current_projection_ = -1;
    std::vector<projection> projections_;

    bool show_ = false;
};

}  // namespace tomovis
