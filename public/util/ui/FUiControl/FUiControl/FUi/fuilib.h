#pragma once
#include <vector>
#include "FuiBase.h"

// ´°¿Ú9¹¬¸ñ·Ö¸î
void SetMargin(std::vector<int>& vecMargin, int nMargin);
void SetMargin(std::vector<int>& vecMargin, int nLeft, int nTop, int nRight, int nBottom);

void SepRect(CRect& rcWnd, std::vector<RectF>& vecRcf, std::vector<int>& vecMargin);

void Rect2RectF(CRect& rc, RectF& rcF);
void Rect2RectF(RECT& rc, RectF& rcF);

/**
  create a new Gdiplus::Image from a block of memory, you must *delete* the image
  manually, i.e : delete img;
*/
Gdiplus::Image *mi_from_memory( const void *buf, size_t size );

/**
  save the image to a block of memory, you must *free* the outbuf manually, i.e:
  free( outbuf );.The image format is jpeg.

  @return the outbuf pinter
*/
void *mi_to_memory( Gdiplus::Image *image, void **outbuf, size_t *size );

HBITMAP cmi_from_memory(const void *buf, size_t size);
