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

#include "wxchartpoint.h"

wxChartPoint::wxChartPoint(double x, 
						   double y, 
						   double radius,
						   unsigned int strokeWidth, 
						   const wxColor &strokeColor,
						   const wxColor &fillColor)
	: m_x(x), m_y(y), m_radius(radius), 
	m_strokeWidth(strokeWidth), m_strokeColor(strokeColor),
	m_fillColor(fillColor)
{
}

void wxChartPoint::Draw(wxGraphicsContext &gc)
{
	wxGraphicsPath path = gc.CreatePath();
	path.AddArc(m_x, m_y, m_radius, 0, 2* M_PI, false);
	path.CloseSubpath();

	wxBrush brush(m_fillColor);
	gc.SetBrush(brush);
	gc.FillPath(path);

	wxPen pen(m_strokeColor, m_strokeWidth);
	gc.SetPen(pen);
	gc.StrokePath(path);
}