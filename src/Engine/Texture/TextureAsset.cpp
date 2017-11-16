#include "TextureAsset.hpp"

#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include <DefaultAlbedo.png.hpp>
#include <Video/Texture/Texture2D.hpp>

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Video;

TextureAsset::TextureAsset() {
    texture = new Texture2D(DEFAULTALBEDO_PNG, DEFAULTALBEDO_PNG_LENGTH);
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
    
    // Load texture from disk.
    texture->Load((filename + ".png").c_str());
}

Texture2D* TextureAsset::GetTexture() const {
    return texture;
}
