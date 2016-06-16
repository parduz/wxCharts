/*
	Copyright (c) 2016 Xavier Leclercq

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
	IN THE SOFTWARE.
*/

#include "wxchartcircle.h"
#include <wx/pen.h>
#include <wx/brush.h>

wxChartCircle::wxChartCircle(wxDouble x, 
                             wxDouble y, 
                             const wxChartTooltipProvider::ptr tooltipProvider,
                             const wxChartCircleOptions &options)
    : wxChartElement(tooltipProvider), m_options(options),
    m_x(x), m_y(y)
{
}

bool wxChartCircle::HitTest(const wxPoint &point) const
{
    return false;
}

wxPoint2DDouble wxChartCircle::GetTooltipPosition() const
{
    return wxPoint2DDouble(0, 0);
}

void wxChartCircle::Draw(wxGraphicsContext &gc)
{
    wxGraphicsPath path = gc.CreatePath();

    
        path.AddCircle(m_x, m_y, 20);
        
    
 

    wxBrush brush(*wxRED);
    gc.SetBrush(brush);
    gc.FillPath(path);

    wxPen pen(*wxWHITE, 5);
    gc.SetPen(pen);
    gc.StrokePath(path);
}

void wxChartCircle::SetCenter(wxDouble x, wxDouble y)
{
    m_x = x;
    m_y = y;
}

void wxChartCircle::SetCenter(wxPoint2DDouble center)
{
    m_x = center.m_x;
    m_y = center.m_y;
}