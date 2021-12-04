// AppErrorCode App::destroy() noexcept {
//     for (auto& it : fontInfos_) {
//         FontInfo& fontInfo = it.second;

//         hb_font_destroy(fontInfo.hbFont);
//         fontInfo.hbFont = nullptr;

//         if (const FT_Error error = FT_Done_Face(fontInfo.ftFace)) {
//             return AppErrorCode::FreeTypeDoneFaceFailed;
//         }
//         fontInfo.ftFace = nullptr;
//     }
//     fontInfos_.clear();

//     hb_buffer_destroy(hbBuffer_);
//     hbBuffer_ = nullptr;

//     if (const FT_Error error = FT_Done_FreeType(ftLibrary_)) {
//         return AppErrorCode::FreeTypeDoneFailed;
//     }
//     ftLibrary_ = nullptr;

//     glfwTerminate();

//     for (auto& textRenderingInfo : textRenderingInfos_) {
//         textRenderingInfo.clear();
//     }
//     textRenderingInfos_.clear();
    
//     if (0 != programId_) {
//         glDeleteProgram(programId_);
//     }
//     programId_ = 0;

//     return AppErrorCode::NoError;
// }

// bool open = false;
// GLuint textureId = 0;
// GLuint vao = 0;
// void App::loop() noexcept {
//     if (!open) {
//         open = true;
//         std::ifstream imageFile("image/sample.jpg", std::ios::binary);
//         if (false == imageFile.is_open()) {
//             int i = 0;
//         }

//         imageFile.seekg(0, std::ios::end);
//         const size_t fileSize = imageFile.tellg();
//         imageFile.seekg(0);
//         std::string fileContent(fileSize, 0);
//         imageFile.read(fileContent.data(), fileContent.size());
//         glGenTextures(1, &textureId);
//         glBindTexture(GL_TEXTURE_2D, textureId);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, fileContent.data());

//         glGenVertexArrays(1, &vao);
//         glBindVertexArray(vao);
//     }

//     while (GLFW_FALSE == glfwWindowShouldClose(window_)) {
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glUseProgram(programId_);

//         glActiveTexture(0);
//         glBindTexture(GL_TEXTURE_2D, textureId);
//         glBindB

//         // for (auto& textRenderingInfo : textRenderingInfos_) {
//         //     glBindTexture(GL_TEXTURE_2D, textRenderingInfo.texture);
//         //      glBindBuffer(GL_ARRAY_BUFFER, textRenderingInfo.vertexBuffer);
//         // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textRenderingInfo.indexBuffer);
//         //     // glBindVertexArray(textRenderingInfo.vertexArray);
//         //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//         //     // glBindVertexArray(0);
//         // }

//         glUseProgram(0); 
//         glfwSwapBuffers(window_);
//         glfwPollEvents();
//     }
// }

// AppErrorCode App::createFontInfo(const FontInfoCreationParameter& parameter) noexcept {
//     const std::string fontFileName = std::filesystem::path(parameter.fontPath).filename().string();
//     if (fontInfos_.end() != fontInfos_.find(fontFileName)) {
//         return AppErrorCode::FontInfoAlreadyExist;
//     }

//     FontInfo fontInfo;

//     if (const FT_Error error = FT_New_Face(ftLibrary_, parameter.fontPath.c_str(), 0, &fontInfo.ftFace)) {
//         return AppErrorCode::FreeTypeNewFaceFailed;
//     }

//     if (const FT_Error error = FT_Set_Char_Size(fontInfo.ftFace, 0, 64 * parameter.fontSize, parameter.screenWidthDpi, parameter.screenHeightDpi)) {
//         return AppErrorCode::FreeTypeSetCharSizeFailed;
//     }

//     fontInfo.hbFont = hb_ft_font_create(fontInfo.ftFace, nullptr);

//     fontInfos_.emplace(fontFileName, std::move(fontInfo));
//     return AppErrorCode::NoError;
// }

// AppErrorCode App::createTextRenderingInfo(const TextRenderingInfoCreationParameter& parameter) noexcept {
//     hb_buffer_reset(hbBuffer_);

//     const hb_direction_t direction = hb_direction_from_string(parameter.direction.c_str(), parameter.direction.size());
//     hb_buffer_set_direction(hbBuffer_, direction);
//     const hb_script_t script = hb_script_from_string(parameter.script.c_str(), parameter.script.size());
//     hb_buffer_set_script(hbBuffer_, script);
//     const hb_language_t language = hb_language_from_string(parameter.language.c_str(), parameter.language.size());
//     hb_buffer_set_language(hbBuffer_, language);

//     hb_buffer_add_utf8(hbBuffer_, parameter.text.c_str(), parameter.text.size(), 0, parameter.text.size());

//     std::vector<hb_feature_t> features(parameter.features.size(), hb_feature_t{});
//     for (int32_t i = 0; i < parameter.features.size(); ++i) {
//         const std::string& feature = parameter.features[i];
//         hb_feature_t hbFeature{};
//         if (0 == hb_feature_from_string(feature.c_str(), feature.size(), &hbFeature)) {
//             return AppErrorCode::HarfBuzzFeatureFromStringFailed;
//         }
//         features[i] = hbFeature;
//     }

//     auto it = fontInfos_.find(parameter.fontName);
//     if (fontInfos_.end() == it) {
//         return AppErrorCode::FontInfoDoesNotExist;
//     }
//     const FontInfo& fontInfo = it->second;
//     hb_shape(fontInfo.hbFont, hbBuffer_, features.empty() ? nullptr : features.data(), features.size());

//     uint32_t glyphInfoCount = 0;
//     hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(hbBuffer_, &glyphInfoCount);
//     uint32_t glyphPositionCount = 0;
//     hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(hbBuffer_, &glyphPositionCount);
//     if (glyphInfoCount != glyphPositionCount) {
//         return AppErrorCode::HarfBuzzGlyphInfoCountInvalid;
//     }

//     float xCurrent = static_cast<float>(parameter.x);
//     float yCurrent = static_cast<float>(parameter.y);
//     for(uint32_t i = 0; i < glyphInfoCount; ++i) {
//         if (const FT_Error error = FT_Load_Glyph(fontInfo.ftFace, glyphInfos[i].codepoint, FT_LOAD_DEFAULT)) {
//             return AppErrorCode::FreeTypeLoadGlyphFailed;
//         }

//         const FT_GlyphSlot slot = fontInfo.ftFace->glyph;
//         if (const FT_Error error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL)) {
//             return AppErrorCode::FreeTypeRenderGlyphFailed;
//         }

//         const FT_Bitmap ftBitmap = slot->bitmap;

//         const int32_t textureWidth = pow(2, ceil(log(ftBitmap.width)/log(2)));
//         const int32_t textureHeight = pow(2, ceil(log(ftBitmap.rows)/log(2)));

//         std::vector<uint8_t> textureData(textureWidth * textureHeight, 0);
//         for(uint32_t i = 0; i < ftBitmap.rows; ++i) {
//             memcpy(textureData.data() + i * textureWidth, ftBitmap.buffer + i * textureWidth, ftBitmap.width);
//         }

//         constexpr float s0 = 0.0f;
//         constexpr float t0 = 0.0f;
//         const float s1 = static_cast<float>(ftBitmap.width) / static_cast<float>(textureWidth);
//         const float t1 = static_cast<float>(ftBitmap.rows) / static_cast<float>(textureHeight);
//         const float xAdvance = static_cast<float>(glyphPositions[i].x_advance) / 64.0f;
//         const float yAdvance = static_cast<float>(glyphPositions[i].y_advance) / 64.0f;
//         const float xOffset = static_cast<float>(glyphPositions[i].x_offset) / 64.0f;
//         const float yOffset = static_cast<float>(glyphPositions[i].y_offset) / 64.0f;
//         const float x0 = xCurrent + xOffset + static_cast<float>(slot->bitmap_left);
//         const float y0 = floor(yCurrent + yOffset + static_cast<float>(slot->bitmap_top));
//         const float x1 = x0 + static_cast<float>(ftBitmap.width);
//         const float y1 = floor(y0 - static_cast<float>(ftBitmap.rows));

//         xCurrent += xAdvance;
//         yCurrent += yAdvance;

//         const float vertices[] = {
//             x0, y0, s0, t0,
//             x0, y1, s0, t1,
//             x1, y1, s1, t1,
//             x1, y0, s1, t0
//         };

//         const unsigned int indices[] = {  
//             0, 1, 3,
//             1, 2, 3
//         };

//         GLuint texture = 0;
//         glGenTextures(1, &texture);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, textureData.data());

//         // GLuint vertexArray = 0;
//         // glGenVertexArrays(1, &vertexArray);
//         GLuint vertexBuffer = 0;
//         glGenBuffers(1, &vertexBuffer);
//         GLuint indexBuffer = 0;
//         glGenBuffers(1, &indexBuffer);

//         // glBindVertexArray(vertexArray);
//         glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//         glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)2);
//         glEnableVertexAttribArray(1);

//         textRenderingInfos_.emplace_back(TextRenderingInfo {
//             .texture = texture,
//             .vertexBuffer = vertexBuffer,
//             .indexBuffer = indexBuffer
//         });
//     }

//     return AppErrorCode::NoError;
// }


/*

    if (const FT_Error error = FT_Init_FreeType(&ftLibrary_)) {
        return AppErrorCode::FreeTypeInitFailed;
    }

    hbBuffer_ = hb_buffer_create();
    if (0 == hb_buffer_allocation_successful(hbBuffer_)) {
        return AppErrorCode::HarfBuzzBufferCreateFailed;
    }

    return AppErrorCode::NoError;


struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;
struct FT_LibraryRec_;
typedef struct FT_LibraryRec_* FT_Library;
struct hb_font_t;
struct hb_buffer_t;

struct FontInfoCreationParameter {
    std::string fontPath = "font/SourceCodePro-Regular.ttf";
    int32_t fontSize = 50;
    int32_t screenWidthDpi = 72;
    int32_t screenHeightDpi = 72;
};

struct FontInfo {
    FT_Face ftFace = nullptr;
    hb_font_t* hbFont = nullptr;
};

struct TextRenderingInfoCreationParameter {
    // utf-8 encoding.
    std::string text;
    std::string fontName;
    std::string language = "en";
    std::string script = "Zyyy";
    std::string direction = "ltr";
    std::vector<std::string> features;
    int32_t x = 0;
    int32_t y = 0;
};

struct Vertex {
    float x = 0.0f;
    float y = 0.0f;
    float s = 0.0f;
    float t = 0.0f;
};

struct TextRenderingInfo {
    uint32_t texture = defaultValue;
    uint32_t vertexBufferObject = defaultValue;
    uint32_t indexBufferObject = defaultValue;
    uint32_t vertexArrayObject = defaultValue;

    void clear() noexcept;

    static constexpr uint32_t defaultValue = std::numeric_limits<uint32_t>::max();
};

    // App app;
    // AppCreationParameter appParameter;
    // if (const AppErrorCode errorCode = app.create(appParameter)) {
    //     std::cout << "app creation failed. " << errorCode << std::endl;
    //     return 1;
    // }

    // FontInfoCreationParameter fontInfoParameter;
    // if (const AppErrorCode errorCode = app.createFontInfo(fontInfoParameter)) {
    //     std::cout << "font info creation failed. " << errorCode << std::endl;
    //     return 1;
    // }

    // TextRenderingInfoCreationParameter textRenderingInfoParameter;
    // textRenderingInfoParameter.x = 0;
    // textRenderingInfoParameter.y = 0;
    // textRenderingInfoParameter.fontName = "SourceCodePro-Regular.ttf";
    // textRenderingInfoParameter.language = "en";
    // textRenderingInfoParameter.script = "arab";
    // textRenderingInfoParameter.direction = "rtl";
    // textRenderingInfoParameter.text = "A";//"تسجّل يتكلّم";
    // if (const AppErrorCode errorCode = app.createTextRenderingInfo(textRenderingInfoParameter)) {
    //     std::cout << "text rendering info creation failed. " << errorCode << std::endl;
    //     return 1;
    // }

    // app.loop();

    // if (const AppErrorCode errorCode = app.destroy()) {
    //     std::cout << "app destroying failed. " << errorCode << std::endl;            
    // }



*/