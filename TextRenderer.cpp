#include "stdafx.h"
#include "TextRenderer.h"

TextRenderer::TextRenderer() : mFontType(kOpenSans), mFontSize(12) {
   mFontColor.r = 255;
   mFontColor.g = 255;
   mFontColor.b = 255;
   mFontColor.a = 255;

   mFont = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-Regular.ttf", mFontSize);
   if (mFont == NULL) {
      std::cerr << "Failed to open font" << mFontType << ": " << TTF_GetError() << std::endl;
   }
}

TextRenderer::~TextRenderer() {
   if (mFont != NULL) {
      TTF_CloseFont(mFont);
   }
}

bool TextRenderer::renderText(std::string &text) {
   return renderText(text.c_str());
}

bool TextRenderer::renderText(const char *text) {
   if (mFont == NULL) {
      std::cerr << "Unable to render text: no font has been set" << std::endl;
      return false;
   }

   SDL_Surface *textSurface = TTF_RenderText_Blended(mFont, text, mFontColor);
   if (textSurface == NULL) {
      std::cerr << "Unable to render text: " << TTF_GetError() << std::endl;
      return false;
   }
   GLenum textFormat;
   if(textSurface->format->BytesPerPixel == 4) {
      if(textSurface->format->Rmask == 0x000000ff) {
         textFormat = GL_RGBA;
      } else {
         textFormat = GL_BGRA_EXT;
      }
   }

   GLuint fontTextureId;
   glGenTextures(1, &fontTextureId);
   glBindTexture(GL_TEXTURE_2D, fontTextureId);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, textSurface->format->BytesPerPixel,
         textSurface->w, textSurface->h,
         0, textFormat, GL_UNSIGNED_BYTE, textSurface->pixels);
   glBindTexture(GL_TEXTURE_2D, fontTextureId);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0.0f, textSurface->h);

      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(textSurface->w, textSurface->h);

      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(textSurface->w, 0.0f);
   glEnd();
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   SDL_FreeSurface(textSurface);
   glDeleteTextures(1, &fontTextureId);
   return true;
}
                              
void TextRenderer::setFont(TextRenderer::FontType fontType, unsigned int fontSize) {
   mFontType = fontType;
   mFontSize = fontSize;
   if (mFont != NULL) {
      TTF_CloseFont(mFont);
   }
   mFont = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-Regular.ttf", fontSize);
   if (mFont == NULL) {
      std::cerr << "Failed to open font" << fontType << ": " << TTF_GetError() << std::endl;
   }
}

void TextRenderer::setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
   if (r > 255 || g > 255 || b > 255 || a > 255) {
      std::cerr << "Color values must be in the range 0-255" << std::endl;
   } else {
      mFontColor.r = r;
      mFontColor.g = g;
      mFontColor.b = b;
      mFontColor.a = a;
   }
}

