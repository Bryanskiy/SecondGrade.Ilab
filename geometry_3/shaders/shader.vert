#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shader_draw_parameters : require

layout(binding = 0) uniform UniformCamera {
    mat4 view;
    mat4 proj;
} uc;


struct StorageModel {
    mat4 model;
};
layout(set = 0, binding = 1) readonly buffer Model {
    StorageModel StorageModelInfo[];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 0) out vec3 fragColor;

const vec4 light = normalize(vec4(-1.0, -1.0, -1.0, 0.0));
const float minLight = 0.2;

void main() {
    gl_Position = uc.proj * uc.view * StorageModelInfo[ gl_BaseInstance ].model * vec4(inPosition, 1.0);

    vec4 validNormal = StorageModelInfo[ gl_BaseInstance ].model * vec4(inNormal, 0.f);

    fragColor = inColor * min(1.f, max(minLight, abs(dot(light, normalize(validNormal)))));
}