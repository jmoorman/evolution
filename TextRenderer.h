#pragma once

#include "stdafx.h"

class TextRenderer
{
public:
   enum FontType {
      kOpenSans
   };

   TextRenderer();
   ~TextRenderer();
   bool renderText(std::string &text);
   bool renderText(const char *text);
   void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a=255);
   void setFont(TextRenderer::FontType fontType, unsigned int fontSize);

private:
   FontType mFontType;
   int mFontSize;
   SDL_Color mFontColor;
   TTF_Font *mFont;
};

