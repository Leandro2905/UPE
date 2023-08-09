/*
 * PROJECT:     PAINT for ReactOS
 * LICENSE:     LGPL
 * FILE:        base/applications/mspaint/mouse.h
 * PURPOSE:     Things which should not be in the mouse event handler itself
 * PROGRAMMERS: Benedikt Freisen
 */

#pragma once

void placeSelWin(void);

void startPaintingL(HDC hdc, LONG x, LONG y, COLORREF fg, COLORREF bg);

void whilePaintingL(HDC hdc, LONG x, LONG y, COLORREF fg, COLORREF bg);

void endPaintingL(HDC hdc, LONG x, LONG y, COLORREF fg, COLORREF bg);

void startPaintingR(HDC hdc, LONG x, LONG y, COLORREF fg, COLORREF bg);

void whilePaintingR(HDC hdc, LONG x, LONG y, COLORREF fg, COLORREF bg);

void endPaintingR(HDC hdc, LONG x, LONG y, COLORREF fg, COLORREF bg);
