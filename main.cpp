#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <bits/stdc++.h>
#include "header.h"
using namespace std;
vector<pair<int,int>> clicks;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Drawing Shapes"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
//The line functions
/*  1- Regular Equation */
void DrawRegularLine(HDC& hdc, int xs, int ys, int xe, int ye)
{
    int dx = xe - xs;
    int dy = ye - ys;

    if(abs(dy) <= abs(dx))  //slop < 1
    {
        double slop = (double)dy/dx;
        if(xs>xe)
        {
            swap(xs,xe);
            swap(ys,ye);
        }
        int x = xs;
        double y = 0;
        while(x<=xe)
        {
            y = round(ys + (x-xs) * slop);
            SetPixel(hdc, x, y, RGB(5, 63, 255));
            x++;
        }
    }
    else                  //slop > 1
    {
        double InSlop = (double)dx/dy;
        if(ys>ye)
        {
            swap(xs,xe);
            swap(ys,ye);
        }
        int y = ys;
        double x = 0;
        while (y<=ye)
        {
            x = round(xs + (y-ys) * InSlop);
            SetPixel(hdc, x, y, RGB(5, 63, 255));
            y++;
        }
    }
}

/*  2- DDA Algorithm*/
void DrawLineWithDDA(HDC& hdc, int xs, int ys, int xe, int ye)
{
    int dx = xe - xs;
    int dy = ye - ys;
    if(abs(dy) < abs(dx))    //slop < 1
    {
        if(xs>xe)
        {
            swap(xs,xe);
            swap(ys,ye);
            dx *= -1;
            dy = ye - ys;
        }
        int x = xs;
        double slop = (double)dy/dx;
        double y = ys;
        while(x <= xe)
        {   SetPixel(hdc, x, round(y), RGB(0, 26, 112));
            x++;
            y+=slop;
        }
    }
    else                   //slop < 1
    {
        if(ys>ye)
        {   swap(xs,xe);
            swap(ys,ye);
            dy *= -1;
            dx = xe - xs;
        }

        double invSlop = (double)dx/dy;
        double x = xs;
        int y = ys;
        while(y <= ye)
        {
            SetPixel(hdc, round(x), y, RGB(0, 26, 112));
            y++;
            x += invSlop;
        }
    }
}

/*  3- MidPoint Algorithm*/
void DrawLineWithMidPoint(HDC& hdc, int xs, int ys, int xe, int ye)
{
    int dx = xe - xs;
    int dy = ye - ys;
    if(abs(dy) < abs(dx))   //slop < 1
    {
        if(xs>xe)
        {   swap(xs,xe);
            swap(ys,ye);
            dx = -dx;
            dy = -dy;
        }
        int des = dx - abs(2*dy);
        int ch1 = -2 * abs(dy);
        int ch2 = 2*dx - abs(2*dy);
        int incY = (dy > 0) ? 1 : -1;   //if dy > 0 increment y+=1 else decrement y-=1
        int x = xs;
        int y = ys;
        while(x <= xe)
        {
            SetPixel(hdc, x, y, RGB(71, 82, 117));
            if(des<=0)
            {
                des+=ch2;
                y+=incY;
            }

            else
                des+=ch1;
            x++;
        }
    }
    else                      //slop > 1
    {
        if(ys>ye)
        {   swap(xs,xe);
            swap(ys,ye);
            dy = -dy;
            dx = -dx;
        }
        int des = dy - abs(2*dx);
        int ch1 = 2 * dy - abs(2*dx);
        int ch2 = -2 * abs(dx);
        int x = xs;
        int y = ys;
        int incX = dx >=0 ? 1 : -1;
        while(y<=ye)
        {
            SetPixel(hdc, x, y, RGB(71, 82, 117));
            if(des <= 0)
            {
                des+=ch1;
                x+=incX;
            }
            else
                des+=ch2;
            y++;
        }
    }
}



//Drawing Circle
//Drawing the 8 symmetric points of circle
void Draw8Points(HDC& hdc, int xc, int yc, int x, int y)
{
    SetPixel(hdc, xc+x, yc+y, RGB(135, 24, 163));
    SetPixel(hdc, xc-x, yc+y, RGB(135, 24, 163));
    SetPixel(hdc, xc+x, yc-y, RGB(135, 24, 163));
    SetPixel(hdc, xc-x, yc-y, RGB(135, 24, 163));
    SetPixel(hdc, xc+y, yc+x, RGB(135, 24, 163));
    SetPixel(hdc, xc-y, yc+x, RGB(135, 24, 163));
    SetPixel(hdc, xc+y, yc-x, RGB(135, 24, 163));
    SetPixel(hdc, xc-y, yc-x, RGB(135, 24, 163));
}

void DrawCircleBresenham(HDC& hdc, int xc, int yc, int R)
{
    int x = 0;
    int y = R;
    double des = 1 - R;
    int ch1 = 3;
    int ch2 = 5 - 2*R;
    Draw8Points(hdc, xc, yc, x, y);
    while(x < y)
    {
        if(des < 0)
        {
            des+=ch1;
            ch2+=2;
        }
        else
        {
            des+=ch2;
            ch2+=4;
            y--;
        }
        ch1+=2;
        x++;
        Draw8Points(hdc, xc, yc, x, y);
    }
}

void DrawCircleCartesien(HDC& hdc, int xc, int yc, int R)
{
    int x = 0;
    int y = R;
    Draw8Points(hdc, xc, yc, x, y);
    while(x<y)
    {
        x++;
        int d = R*R - x*x;
        y = sqrt(d);
        Draw8Points(hdc, xc, yc, x, y);
    }


}
//Draw Circle Polar
void CirclePolar(HDC& hdc, int xc, int yc, int R)
{
    double x = R, y = 0;
    double dtheta = 1.0/R;
    double cdtheta = cos(dtheta),
    sdtheta = sin(dtheta);
    Draw8Points(hdc, xc, yc, x, y);
    while(x>y)
    {
        double x1 = x*cdtheta - y*sdtheta;
        y = x*sdtheta + y*cdtheta;
        x=x1;
        Draw8Points(hdc,xc,yc,round(x),round(y));
    }

}
//Drawing Curves
void DrawParametricCurve(HDC& hdc, double xs, double ys, double xe, double ye)   //it will draw a line because its result from 1st degree
{
    double xt;
    double yt;
    double step = 0.0001;
    for(double t=0.0; t<=1.0; t+=step)
    {
        xt = xs+(xe-xs)*t;
        yt = ys+(ye-ys)*t;
        SetPixel(hdc,xt,yt,RGB(37,132,38));
    }
}
// Drawing parabola
void DrawParabla(HDC& hdc, double x1, double y1, double x2, double y2, double x3, double y3)
{
    double xt, yt;
    double step = 0.0001;
    for(double t = 0.0; t<=1.0; t+=step)
    {
        xt = x1 + ((-3*x1 - x2 + 4*x3)*t) + ((2*x1 + 2*x2 - 4*x3)*t*t);
        yt = y1 + ((-3*y1 - y2 + 4*y3)*t) + ((2*y1 + 2*y2 - 4*y3)*t*t);
        SetPixel(hdc, xt,yt,RGB(7,242,11));
    }
}
//*******************Draw Curves Using Hermite Algorithm***********************
void DrawHermite(HDC& hdc, double x1, double y1,double x2, double y2, double x3, double y3,double x4, double y4)
{
    double alpha0 = x1;
    double alpha1 = x3;
    double alpha2 = -3*x1 + 3*x2 - 2*x3 - x4;
    double alpha3 = 2*x1 - 2*x2 + x3 + x4;
    double beta0 = y1;
    double beta1 = y3;
    double beta2 = -3*y1 + 3*y2 - 2*y3 - y4;
    double beta3 = 2*y1 - 2*y2 + y3 +y4;
    double xt, yt;
    double step = 0.0001;
    for(double t=0.0; t<=1; t+=step)
    {
        xt = alpha0 + alpha1*t + alpha2*t*t + alpha3*t*t*t;
        yt = beta0 + beta1*t + beta2*t*t + beta3*t*t*t;
        SetPixel(hdc, xt, yt, RGB(0,150,2));
    }
}
void DrawBezier(HDC& hdc, double x1, double y1,double x2, double y2, double x3, double y3,double x4, double y4)
{
    double alpha0 = x1;
    double alpha1 = -3*x1 + 3*x2;
    double alpha2 = 3*x1 - 6*x2 + 3*x3;
    double alpha3 = -1*x1 + 3*x2 - 3*x3 + x4;
    double beta0 = y1;
    double beta1 = -3*y1 + 3*y2;
    double beta2 = 3*y1 - 6*y2 + 3*y3;
    double beta3 = -1*y1 + 3*y2 - 3*y3 +y4;
    double xt, yt;
    double step = 0.0001;
    for(double t=0.0; t<=1; t+=step)
    {
        xt = alpha0 + alpha1*t + alpha2*t*t + alpha3*t*t*t;
        yt = beta0 + beta1*t + beta2*t*t + beta3*t*t*t;
        SetPixel(hdc, xt, yt, RGB(0,0,2));
    }
}
//Line Clipping Cohen/Sutherland
union Outcode
{
    unsigned All : 4;
    struct
    {
        unsigned left: 1,
                 right: 1,
                 top: 1,
                 bottom: 1;
    };
};
Outcode GetOutcode(double x, double y, int xleft, int ytop, int xright, int ybottom)
{
    Outcode out;
    out.All = 0;
    if(x<xleft)
        out.left = 1;
    else if(x>xright)
        out.right = 1;
    if(y<ytop)
        out.top = 1;
    else if(y>ybottom)
        out.bottom = 1;
    return out;
}
void Vintersect(double xs, double ys, double xe, double ye, int xedge, double *xint, double *yint)
{
    *xint = xedge;
    *yint = ys + (xedge - xs) * (ye-ys)/(xe-xs);
}
void Hintersect(double xs, double ys, double xe, double ye, int yedge, double *xint, double *yint)
{
    *yint = yedge;
    *xint = xs + (yedge-ys) * (xe-xs)/(ye-ys);
}
void CohenSuther(HDC& hdc, double xs, double ys, double xe, double ye, int xleft, int xright, int ytop, int ybottom )
{
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    Outcode out1 = GetOutcode(x1, y1, xleft, ytop, xright, ybottom);
    Outcode out2 = GetOutcode(x2, y2, xleft, ytop, xright, ybottom);
    while (!(out1.All==0 && out2.All==0) && ((out1.All && out2.All)==0))
    {
        double xins, yins;
        if(out1.All)
        {
            if(out1.left)
                Vintersect(x1,y1,x2,y2,xleft,&xins,&yins);
            else if(out1.right)
                Vintersect(x1,y1,x2,y2,xright,&xins,&yins);
            else if(out1.top)
                Hintersect(x1,y1,x2,y2,ytop,&xins,&yins);
            else
                Hintersect(x1,y1,x2,y2,ybottom,&xins,&yins);
            x1 = xins;
            y1 = yins;
            out1 = GetOutcode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                Vintersect(x1, y1, x2, y2, xleft, &xins, &yins);
            else if(out2.right)
                Vintersect(x1, y1, x2, y2, xright, &xins, &yins);
            else if(out2.top)
                Hintersect(x1, y1, x2, y2, ytop, &xins, &yins);
            else
                Hintersect(x1, y1, x2, y2, ybottom, &xins, &yins);
            x2 = xins;
            y2 = yins;
            out2 = GetOutcode(x2,y2,xleft,ytop,xright,ybottom);
        }

    }
    if(!out1.All && out2.All)
    {
        //MoveToEx(hdc, round(x1), round(y1), NULL);
        //LineTo(hdc, round(x2),round(y2));
        DrawLineWithDDA(hdc,x1,y1,x2,y2);
    }

}
void load()
{

}




/*  This function is called by the Windows function DispatchMessage()  */

bool drawLineDDA = false;
bool drawLineMidPoint = false;
bool drawCircle = false;
bool drawCircleCart = false;
bool drawCirclePolar = false;
bool drawRegularLine = false;
bool drawParaCurve = false;
bool drawParabola = false;
bool drawHermite = false;
bool drawBezier = false;
bool ClipLine = false;



/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
         case WM_COMMAND:
        {
            switch(LOWORD(wParam))
                {
                case ID_Exit:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                case ID_LineDDA:
                    {   drawLineDDA = true;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                    }
                    break;
                case ID_LineMidPoint:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = true;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                    }

                    break;
                case ID_RegularLine:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawParaCurve = false;
                        drawRegularLine = true;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                    }
                    break;
                case ID_Circle:
                   {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = true;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                   }
                    break;
                case ID_CircleCart:
                   {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = true;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                   }
                    break;
                case ID_CirclePolar:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = true;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                    }
                    break;
                    case ID_ParaCurve:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = true;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                    }
                    break;
                    case ID_Parabola:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = true;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = false;
                    }
                    break;
                    case ID_Hermite:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = true;
                        drawBezier = false;
                        ClipLine = false;
                    }
                    break;
                    case ID_Bezier:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = true;
                        ClipLine = false;
                    }
                    break;
                    case ID_LineClip:
                    {
                        drawLineDDA = false;
                        drawLineMidPoint = false;
                        drawCircle = false;
                        drawCirclePolar = false;
                        drawCircleCart = false;
                        drawRegularLine = false;
                        drawParaCurve = false;
                        drawParabola = false;
                        drawHermite = false;
                        drawBezier = false;
                        ClipLine = true;
                    }
                    break;
                }
                break;
        }
        case WM_PAINT:
            {   PAINTSTRUCT ps;
                if(drawLineDDA&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    DrawLineWithDDA(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawLineMidPoint&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    DrawLineWithMidPoint(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    freopen("Lines.txt","w", stdin);
                    clicks.clear();
                }
                if(drawCircle&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    int r = sqrt((clicks[0].first - clicks[1].first) * (clicks[0].first - clicks[1].first)+
                             (clicks[0].second - clicks[1].second) * (clicks[0].second - clicks[1].second));
                    DrawCircleBresenham(ps.hdc, clicks[0].first, clicks[0].second, r);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawCircleCart&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    int r = sqrt((clicks[0].first - clicks[1].first) * (clicks[0].first - clicks[1].first)+
                             (clicks[0].second - clicks[1].second) * (clicks[0].second - clicks[1].second));
                    DrawCircleCartesien(ps.hdc, clicks[0].first, clicks[0].second, r);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawRegularLine&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    DrawRegularLine(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd,NULL, false);
                    clicks.clear();
                }
                if(drawCirclePolar&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    int r = sqrt((clicks[0].first - clicks[1].first) * (clicks[0].first - clicks[1].first)+
                             (clicks[0].second - clicks[1].second) * (clicks[0].second - clicks[1].second));
                    CirclePolar(ps.hdc, clicks[0].first, clicks[0].second, r);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawParaCurve&&clicks.size()==2)
                {
                    BeginPaint(hwnd, &ps);
                    DrawParametricCurve(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawParabola&&clicks.size()==3)
                {
                    BeginPaint(hwnd, &ps);
                    DrawParabla(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second, clicks[2].first, clicks[2].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawHermite&&clicks.size()==4)
                {
                    BeginPaint(hwnd, &ps);
                    DrawHermite(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second, clicks[2].first, clicks[2].second, clicks[3].first, clicks[3].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
                if(drawBezier&&clicks.size()==4)
                {
                    BeginPaint(hwnd, &ps);
                    DrawBezier(ps.hdc, clicks[0].first, clicks[0].second, clicks[1].first, clicks[1].second, clicks[2].first, clicks[2].second, clicks[3].first, clicks[3].second);
                    EndPaint(hwnd, &ps);
                    InvalidateRect(hwnd, NULL, false);
                    clicks.clear();
                }
            }
break;
    case WM_LBUTTONDOWN:
        {
            if (clicks.size() < 4)
            {
                clicks.push_back({
				LOWORD(lParam),
				HIWORD(lParam) });
            }
        }
        break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
