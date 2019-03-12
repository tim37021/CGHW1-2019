#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <cstdint>
#include <string>

enum class WrapMode {
    eRepeat, eMirrorRepeat, eClampToEdge, eClampToBorder
};

enum class FilterMode {
    eNearest, eLinear, eNearestMipmapLinear, eLinearMipmapLinear, eNearestMipmapNearest, eLinearMipmapNearest
};

enum class ColorType {
    eRed, eRG, eRGB, eRGBA
};


class ITexture {
public:
    virtual void bindToChannel(GLuint channel)=0;
    virtual void setWrap(WrapMode s, WrapMode t, WrapMode r)=0;
    virtual void setFilter(FilterMode min, FilterMode mag)=0;
    virtual void release()=0;
    virtual bool hasMipmap()=0;
};

class Texture2D : public ITexture {
public:
    Texture2D(const Texture2D &rhs) = default;
    Texture2D(uint32_t width, uint32_t height, ColorType type=ColorType::eRGBA);
    static Texture2D LoadFromFile(const std::string &pngfile);

    virtual void bindToChannel(GLuint channel) override;
    virtual void setWrap(WrapMode s, WrapMode t, WrapMode r = WrapMode::eRepeat) override;
    virtual void setFilter(FilterMode min, FilterMode mag) override;
    virtual void release() override;
    virtual bool hasMipmap() override;

    GLuint id() const
    { return m_id; }


    bool operator!=(const Texture2D &rhs) const
    { return m_id != rhs.m_id; }

private:
    Texture2D(GLuint id);
    GLuint m_id;
    uint32_t m_width, m_height;
    bool m_mipmap;
};

#endif