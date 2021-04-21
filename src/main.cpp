#include "App.h"
#include "Shader.h"

int main() {
    AppFactory().createApp("App", 1280, 720)
        .throwIfError()
        .onSuccess([](Result<App>& result) {
            App app(std::move(result.data()));

            std::vector<ShaderProgram> shaderPrograms;
            ShaderManager::open().throwIfError();
            ShaderFactory().createShaderProgram("shader/vertex.glsl", "shader/fregment.glsl")
                .throwIfError()
                .onSuccess([&shaderPrograms](Result<ShaderProgram>& result) {
                    shaderPrograms.emplace_back(std::move(result.data()));
                });

            app.loop(shaderPrograms);
        });

    return 0;
}
