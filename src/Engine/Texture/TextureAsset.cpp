#include "TextureAsset.hpp"

#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include <DefaultAlbedo.png.hpp>
#include <Video/Texture/TexturePNG.hpp>
#include <Video/Texture/TextureHCT.hpp>
#include "../Manager/Managers.hpp"
#include "../Manager/RenderManager.hpp"

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Video;

TextureAsset::TextureAsset() {
    texture = new TexturePNG(DEFAULTALBEDO_PNG, DEFAULTALBEDO_PNG_LENGTH);
}

TextureAsset::TextureAsset(const char* source, int sourceLength) {
    texture = new TexturePNG(source, sourceLength);
}

TextureAsset::~TextureAsset() {
    delete texture;
}

void TextureAsset::Save() const {
    
}

void TextureAsset::Load(const std::string& name) {
    std::size_t pos = name.find_last_of('/');
    this->name = name.substr(pos + 1);
    path = name.substr(0, pos + 1);
    std::string filename = Hymn().GetPath() + "/" + name;
    
    // Delete old texture.
    if (texture != nullptr)
        delete texture;
    
    // Load texture from disk.
    texture = new TextureHCT((filename + ".hct").c_str(), Managers().renderManager->GetTextureReduction());
}

Texture2D* TextureAsset::GetTexture() const {
    return texture;
}
