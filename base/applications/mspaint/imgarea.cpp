/*
 * PROJECT:     PAINT for ReactOS
 * LICENSE:     LGPL
 * FILE:        base/applications/mspaint/imgarea.cpp
 * PURPOSE:     Window procedure of the main window and all children apart from
 *              hPalWin, hToolSettings and hSelection
 * PROGRAMMERS: Benedikt Freisen
 *              Katayama Hirofumi MZ
 */

/* INCLUDES *********************************************************/

#include "precomp.h"

#include "dialogs.h"

/* FUNCTIONS ********************************************************/

void
updateCanvasAndScrollbars()
{
    selectionWindow.ShowWindow(SW_HIDE);

    int zoomedWidth = imageModel.GetWidth() * toolsModel.GetZoom() / 1000;
    int zoomedHeight = imageModel.GetHeight() * toolsModel.GetZoom() / 1000;
    imageArea.MoveWindow(3, 3, zoomedWidth, zoomedHeight, FALSE);

    scrollboxWindow.Invalidate(TRUE);
    imageArea.Invalidate(FALSE);

    scrollboxWindow.SetScrollPos(SB_HORZ, 0, TRUE);
    scrollboxWindow.SetScrollPos(SB_VERT, 0, TRUE);
}

void CImgAreaWindow::drawZoomFrame(int mouseX, int mouseY)
{
    HDC hdc;
    HPEN oldPen;
    HBRUSH oldBrush;
    LOGBRUSH logbrush;
    int rop;

    RECT clientRectScrollbox;
    RECT clientRectImageArea;
    int x, y, w, h;
    scrollboxWindow.GetClientRect(&clientRectScrollbox);
    GetClientRect(&clientRectImageArea);
    w = clientRectImageArea.right * 2;
    h = clientRectImageArea.bottom * 2;
    if (!w || !h)
    {
        return;
    }
    w = clientRectImageArea.right * clientRectScrollbox.right / w;
    h = clientRectImageArea.bottom * clientRectScrollbox.bottom / h;
    x = max(0, min(clientRectImageArea.right - w, mouseX - w / 2));
    y = max(0, min(clientRectImageArea.bottom - h, mouseY - h / 2));

    hdc = GetDC();
    oldPen = (HPEN) SelectObject(hdc, CreatePen(PS_SOLID, 0, 0));
    logbrush.lbStyle = BS_HOLLOW;
    oldBrush = (HBRUSH) SelectObject(hdc, CreateBrushIndirect(&logbrush));
    rop = SetROP2(hdc, R2_NOT);
    Rectangle(hdc, x, y, x + w, y + h);
    SetROP2(hdc, rop);
    DeleteObject(SelectObject(hdc, oldBrush));
    DeleteObject(SelectObject(hdc, oldPen));
    ReleaseDC(hdc);
}

LRESULT CImgAreaWindow::OnSize(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int imgXRes = imageModel.GetWidth();
    int imgYRes = imageModel.GetHeight();
    sizeboxLeftTop.MoveWindow(
               0,
               0, 3, 3, TRUE);
    sizeboxCenterTop.MoveWindow(
               imgXRes * toolsModel.GetZoom() / 2000 + 3 * 3 / 4,
               0, 3, 3, TRUE);
    sizeboxRightTop.MoveWindow(
               imgXRes * toolsModel.GetZoom() / 1000 + 3,
               0, 3, 3, TRUE);
    sizeboxLeftCenter.MoveWindow(
               0,
               imgYRes * toolsModel.GetZoom() / 2000 + 3 * 3 / 4, 3, 3, TRUE);
    sizeboxRightCenter.MoveWindow(
               imgXRes * toolsModel.GetZoom() / 1000 + 3,
               imgYRes * toolsModel.GetZoom() / 2000 + 3 * 3 / 4, 3, 3, TRUE);
    sizeboxLeftBottom.MoveWindow(
               0,
               imgYRes * toolsModel.GetZoom() / 1000 + 3, 3, 3, TRUE);
    sizeboxCenterBottom.MoveWindow(
               imgXRes * toolsModel.GetZoom() / 2000 + 3 * 3 / 4,
               imgYRes * toolsModel.GetZoom() / 1000 + 3, 3, 3, TRUE);
    sizeboxRightBottom.MoveWindow(
               imgXRes * toolsModel.GetZoom() / 1000 + 3,
               imgYRes * toolsModel.GetZoom() / 1000 + 3, 3, 3, TRUE);
    UpdateScrollbox();
    return 0;
}

LRESULT CImgAreaWindow::OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DefWindowProc(WM_PAINT, wParam, lParam);
    HDC hdc = GetDC();
    int imgXRes = imageModel.GetWidth();
    int imgYRes = imageModel.GetHeight();
    StretchBlt(hdc, 0, 0, imgXRes * toolsModel.GetZoom() / 1000, imgYRes * toolsModel.GetZoom() / 1000, imageModel.GetDC(), 0, 0, imgXRes,
               imgYRes, SRCCOPY);
    if (showGrid && (toolsModel.GetZoom() >= 4000))
    {
        HPEN oldPen = (HPEN) SelectObject(hdc, CreatePen(PS_SOLID, 1, 0x00a0a0a0));
        int counter;
        for(counter = 0; counter <= imgYRes; counter++)
        {
            MoveToEx(hdc, 0, counter * toolsModel.GetZoom() / 1000, NULL);
            LineTo(hdc, imgXRes * toolsModel.GetZoom() / 1000, counter * toolsModel.GetZoom() / 1000);
        }
        for(counter = 0; counter <= imgXRes; counter++)
        {
            MoveToEx(hdc, counter * toolsModel.GetZoom() / 1000, 0, NULL);
            LineTo(hdc, counter * toolsModel.GetZoom() / 1000, imgYRes * toolsModel.GetZoom() / 1000);
        }
        DeleteObject(SelectObject(hdc, oldPen));
    }
    ReleaseDC(hdc);
    selectionWindow.Invalidate(FALSE);
    miniature.Invalidate(FALSE);
    return 0;
}

LRESULT CImgAreaWindow::OnSetCursor(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    switch (toolsModel.GetActiveTool())
    {
        case TOOL_FILL:
            SetCursor(hCurFill);
            break;
        case TOOL_COLOR:
            SetCursor(hCurColor);
            break;
        case TOOL_ZOOM:
            SetCursor(hCurZoom);
            break;
        case TOOL_PEN:
            SetCursor(hCurPen);
            break;
        case TOOL_AIRBRUSH:
            SetCursor(hCurAirbrush);
            break;
        default:
            SetCursor(LoadCursor(NULL, IDC_CROSS));
    }
    return 0;
}

LRESULT CImgAreaWindow::OnLButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if ((!drawing) || (toolsModel.GetActiveTool() == TOOL_COLOR))
    {
        SetCapture();
        drawing = TRUE;
        startPaintingL(imageModel.GetDC(), GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom(),
                       paletteModel.GetFgColor(), paletteModel.GetBgColor());
    }
    else
    {
        SendMessage(WM_LBUTTONUP, wParam, lParam);
        imageModel.Undo();
    }
    Invalidate(FALSE);
    if ((toolsModel.GetActiveTool() == TOOL_ZOOM) && (toolsModel.GetZoom() < 8000))
        zoomTo(toolsModel.GetZoom() * 2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;
}

LRESULT CImgAreaWindow::OnRButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if ((!drawing) || (toolsModel.GetActiveTool() == TOOL_COLOR))
    {
        SetCapture();
        drawing = TRUE;
        startPaintingR(imageModel.GetDC(), GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom(),
                       paletteModel.GetFgColor(), paletteModel.GetBgColor());
    }
    else
    {
        SendMessage(WM_RBUTTONUP, wParam, lParam);
        imageModel.Undo();
    }
    Invalidate(FALSE);
    if ((toolsModel.GetActiveTool() == TOOL_ZOOM) && (toolsModel.GetZoom() > 125))
        zoomTo(toolsModel.GetZoom() / 2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;
}

LRESULT CImgAreaWindow::OnLButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (drawing)
    {
        endPaintingL(imageModel.GetDC(), GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), paletteModel.GetFgColor(),
                     paletteModel.GetBgColor());
        Invalidate(FALSE);
        if (toolsModel.GetActiveTool() == TOOL_COLOR)
        {
            COLORREF tempColor =
                GetPixel(imageModel.GetDC(), GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom());
            if (tempColor != CLR_INVALID)
                paletteModel.SetFgColor(tempColor);
        }
        SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM) "");
    }
    drawing = FALSE;
    ReleaseCapture();
    return 0;
}

void CImgAreaWindow::cancelDrawing()
{
    POINT pt;
    switch (toolsModel.GetActiveTool())
    {
        case TOOL_FREESEL: case TOOL_RECTSEL:
        case TOOL_TEXT: case TOOL_ZOOM: case TOOL_SHAPE:
            imageModel.ResetToPrevious();
            selectionModel.ResetPtStack();
            pointSP = 0;
            Invalidate(FALSE);
            break;
        default:
            GetCursorPos(&pt);
            ScreenToClient(&pt);
            // FIXME: dirty hack
            if (GetKeyState(VK_LBUTTON) < 0)
            {
                endPaintingL(imageModel.GetDC(), pt.x * 1000 / toolsModel.GetZoom(), pt.y * 1000 / toolsModel.GetZoom(), paletteModel.GetFgColor(),
                             paletteModel.GetBgColor());
            }
            else if (GetKeyState(VK_RBUTTON) < 0)
            {
                endPaintingR(imageModel.GetDC(), pt.x * 1000 / toolsModel.GetZoom(), pt.y * 1000 / toolsModel.GetZoom(), paletteModel.GetFgColor(),
                             paletteModel.GetBgColor());
            }
            imageModel.Undo();
            pointSP = 0;
            selectionModel.ResetPtStack();
    }
}

LRESULT CImgAreaWindow::OnCaptureChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (drawing)
    {
        cancelDrawing();
        drawing = FALSE;
    }
    return 0;
}

LRESULT CImgAreaWindow::OnKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == VK_ESCAPE)
    {
        if (GetCapture() == m_hWnd)
        {
            ReleaseCapture();
        }
        else
        {
            switch (toolsModel.GetActiveTool())
            {
                case TOOL_SHAPE: case TOOL_BEZIER:
                    cancelDrawing();
                    break;
            }
        }
    }
    return 0;
}

LRESULT CImgAreaWindow::OnRButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (drawing)
    {
        endPaintingR(imageModel.GetDC(), GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), paletteModel.GetFgColor(),
                     paletteModel.GetBgColor());
        Invalidate(FALSE);
        if (toolsModel.GetActiveTool() == TOOL_COLOR)
        {
            COLORREF tempColor =
                GetPixel(imageModel.GetDC(), GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom(), GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom());
            if (tempColor != CLR_INVALID)
                paletteModel.SetBgColor(tempColor);
        }
        SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM) "");
    }
    drawing = FALSE;
    ReleaseCapture();
    return 0;
}

LRESULT CImgAreaWindow::OnMouseMove(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LONG xNow = GET_X_LPARAM(lParam) * 1000 / toolsModel.GetZoom();
    LONG yNow = GET_Y_LPARAM(lParam) * 1000 / toolsModel.GetZoom();
    if ((!drawing) || (toolsModel.GetActiveTool() <= TOOL_AIRBRUSH))
    {
        TRACKMOUSEEVENT tme;

        if (toolsModel.GetActiveTool() == TOOL_ZOOM)
        {
            Invalidate(FALSE);
            UpdateWindow();
            drawZoomFrame(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        tme.dwHoverTime = 0;
        TrackMouseEvent(&tme);

        if (!drawing)
        {
            CString strCoord;
            strCoord.Format(_T("%ld, %ld"), xNow, yNow);
            SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM) (LPCTSTR) strCoord);
        }
    }
    if (drawing)
    {
        /* values displayed in statusbar */
        LONG xRel = xNow - start.x;
        LONG yRel = yNow - start.y;
        /* freesel, rectsel and text tools always show numbers limited to fit into image area */
        if ((toolsModel.GetActiveTool() == TOOL_FREESEL) || (toolsModel.GetActiveTool() == TOOL_RECTSEL) || (toolsModel.GetActiveTool() == TOOL_TEXT))
        {
            if (xRel < 0)
                xRel = (xNow < 0) ? -start.x : xRel;
            else if (xNow > imageModel.GetWidth())
                xRel = imageModel.GetWidth() - start.x;
            if (yRel < 0)
                yRel = (yNow < 0) ? -start.y : yRel;
            else if (yNow > imageModel.GetHeight())
                 yRel = imageModel.GetHeight() - start.y;
        }
        /* rectsel and shape tools always show non-negative numbers when drawing */
        if ((toolsModel.GetActiveTool() == TOOL_RECTSEL) || (toolsModel.GetActiveTool() == TOOL_SHAPE))
        {
            if (xRel < 0)
                xRel = -xRel;
            if (yRel < 0)
                yRel =  -yRel;
        }
        /* while drawing, update cursor coordinates only for tools 3, 7, 8, 9, 14 */
        switch(toolsModel.GetActiveTool())
        {
            case TOOL_RUBBER:
            case TOOL_PEN:
            case TOOL_BRUSH:
            case TOOL_AIRBRUSH:
            case TOOL_SHAPE:
            {
                CString strCoord;
                strCoord.Format(_T("%ld, %ld"), xNow, yNow);
                SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM) (LPCTSTR) strCoord);
                break;
            }
        }
        if ((wParam & MK_LBUTTON) != 0)
        {
            whilePaintingL(imageModel.GetDC(), xNow, yNow, paletteModel.GetFgColor(), paletteModel.GetBgColor());
            Invalidate(FALSE);
            if ((toolsModel.GetActiveTool() >= TOOL_TEXT) || (toolsModel.GetActiveTool() == TOOL_RECTSEL) || (toolsModel.GetActiveTool() == TOOL_FREESEL))
            {
                CString strSize;
                if ((toolsModel.GetActiveTool() >= TOOL_LINE) && (GetAsyncKeyState(VK_SHIFT) < 0))
                    yRel = xRel;
                strSize.Format(_T("%ld x %ld"), xRel, yRel);
                SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM) (LPCTSTR) strSize);
            }
        }
        if ((wParam & MK_RBUTTON) != 0)
        {
            whilePaintingR(imageModel.GetDC(), xNow, yNow, paletteModel.GetFgColor(), paletteModel.GetBgColor());
            Invalidate(FALSE);
            if (toolsModel.GetActiveTool() >= TOOL_TEXT)
            {
                CString strSize;
                if ((toolsModel.GetActiveTool() >= TOOL_LINE) && (GetAsyncKeyState(VK_SHIFT) < 0))
                    yRel = xRel;
                strSize.Format(_T("%ld x %ld"), xRel, yRel);
                SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM) (LPCTSTR) strSize);
            }
        }
    }
    return 0;
}

LRESULT CImgAreaWindow::OnMouseLeave(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM) _T(""));
    if (toolsModel.GetActiveTool() == TOOL_ZOOM)
        Invalidate(FALSE);
    return 0;
}

LRESULT CImgAreaWindow::OnImageModelDimensionsChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    updateCanvasAndScrollbars();
    return 0;
}

LRESULT CImgAreaWindow::OnImageModelImageChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    Invalidate(FALSE);
    return 0;
}
