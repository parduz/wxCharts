/*
    Copyright (c) 2016-2018 Xavier Leclercq and the wxCharts contributors.

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

/*
    Part of this file were copied from the Chart.js project (http://chartjs.org/)
    and translated into C++.

    The files of the Chart.js project have the following copyright and license.

    Copyright (c) 2013-2016 Nick Downie
    Released under the MIT license
    https://github.com/nnnick/Chart.js/blob/master/LICENSE.md
*/

#include "wxchartgrid.h"
#include "wxchartnumericalaxis.h"
#include "wxchartcategoricalaxis.h"
#include <wx/pen.h>

//#define LogFunction(str)		fprintf(stdout, "   - +++ %s: %s\n",__FUNCTION__,(str))
#define LogFunction(str)

static const wxDouble MinDistance = 1.0e-3;
static const wxDouble MaxDistance = 1.0e3;

wxChartGrid::wxChartGrid(const wxPoint2DDouble &position,
                         const wxSize &size,
                         wxChartAxis::ptr xAxis,
                         wxChartAxis::ptr yAxis,
                         const wxChartGridOptions& options)
    : m_options			(options),
      m_position		(position),
      m_XAxis			(xAxis),
      m_YAxis			(yAxis),
      m_mapping			(size, m_XAxis, m_YAxis),
      m_needsFit		(true)
{
}

wxChartGrid::wxChartGrid(const wxPoint2DDouble &position,
                         const wxSize &size,
                         wxDouble minXValue,
                         wxDouble maxXValue,
                         wxDouble minYValue,
                         wxDouble maxYValue,
                         const wxChartGridOptions& options)
    : m_options			(options),
      m_position		(position),
      m_XAxis			(new wxChartNumericalAxis("x", minXValue, maxXValue, options.GetXAxisOptions())),
      m_YAxis			(new wxChartNumericalAxis("y", minYValue, maxYValue, options.GetYAxisOptions())),
      m_mapping			(size, m_XAxis, m_YAxis),
      m_needsFit		(true),
      m_origAxisLimits	(minXValue,maxXValue,minYValue,maxYValue),
      m_curAxisLimits	(minXValue,maxXValue,minYValue,maxYValue)
{

}

void wxChartGrid::Draw(wxGraphicsContext &gc) const
{
    const wxChartAxis* verticalAxis = 0;
    const wxChartAxis* horizontalAxis = 0;

    wxChartAxisLocation	XLocation = m_XAxis->GetOptions().GetLocation();
    wxChartAxisLocation	YLocation = m_YAxis->GetOptions().GetLocation();

	switch (XLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
			LogFunction("RIGHT");
		case wxCHARTAXISPOSITION_LEFT:
			{
				verticalAxis = m_XAxis.get();
			}
			break;

		case wxCHARTAXISPOSITION_TOP:
			LogFunction("TOP");
		case wxCHARTAXISPOSITION_BOTTOM:
			{
				horizontalAxis = m_XAxis.get();
			}
			break;
		default:
			LogFunction("WHAT?");
			break;
	}

	switch (YLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
			LogFunction("RIGHT");
		case wxCHARTAXISPOSITION_LEFT:
			{
				verticalAxis = m_YAxis.get();
			}
			break;

		case wxCHARTAXISPOSITION_TOP:
			LogFunction("TOP");
		case wxCHARTAXISPOSITION_BOTTOM:
			{
				horizontalAxis = m_YAxis.get();
			}
			break;
		default:
			LogFunction("WHAT?");
			break;
	}
/*
    if (XLocation == wxCHARTAXISPOSITION_LEFT)
    {
        verticalAxis = m_XAxis.get();
    }
    else if (YLocation == wxCHARTAXISPOSITION_LEFT)
    {
        verticalAxis = m_YAxis.get();
    }

    if (XLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        horizontalAxis = m_XAxis.get();
    }
    else if (YLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        horizontalAxis = m_YAxis.get();
    }
*/
    if (m_options.GetHorizontalGridLineOptions().ShowGridLines())
    {
        DrawHorizontalGridLines(*horizontalAxis, *verticalAxis, m_options.GetHorizontalGridLineOptions(), gc);
    }

    if (m_options.GetVerticalGridLineOptions().ShowGridLines())
    {
        DrawVerticalGridLines(*horizontalAxis, *verticalAxis, m_options.GetVerticalGridLineOptions(), gc);
    }

    m_XAxis->Draw(gc);
    m_YAxis->Draw(gc);
}

bool wxChartGrid::HitTest(const wxPoint &point) const
{
    return false;
}

wxPoint2DDouble wxChartGrid::GetTooltipPosition() const
{
    return wxPoint2DDouble(0, 0);
}

void wxChartGrid::Fit(wxGraphicsContext &gc)
{
    // Determine the axis positions in the available (draw-able) area.

    if (!m_needsFit)
    {
        return;
    }

	m_pxGraphArea.SetSize( m_mapping.GetSize() );
    int l,t,r,b; // left, top, right, bottom - in pixels
	l=t=0;
	r = m_mapping.GetSize().GetWidth();
	b = m_mapping.GetSize().GetHeight();


	// Recalculate labels size, cause the axis could not already know his internal dimensions
	// ('cause a gc must be passed to it)
    m_YAxis->UpdateLabelSizes(gc);
    m_XAxis->UpdateLabelSizes(gc);

	// each Axis knows his stuffs.
	int VertPadding = std::max(m_XAxis->pxPadding,m_YAxis->pxMaxLabelDimensions.y/2);
	int HorzPadding = std::max(m_YAxis->pxPadding,m_XAxis->pxMaxLabelDimensions.x/2);
//	int xfs = m_XAxis->GetOptions().GetFontOptions().GetSize();	// h of the Y axis font -- in pixels?
//	int yfs = m_YAxis->GetOptions().GetFontOptions().GetSize();	// h of the Y axis font -- in pixels?
//	int xMaxLabSize = m_XAxis->GetLabels().GetMaxWidth();
//	int yMaxLabSize = m_YAxis->GetLabels().GetMaxWidth();

	// Where's the Axis?
    wxChartAxisLocation	XLocation = m_XAxis->GetOptions().GetLocation();
    wxChartAxisLocation	YLocation = m_YAxis->GetOptions().GetLocation();

	switch (XLocation) {
		case wxCHARTAXISPOSITION_TOP:
			// Vertical top padding:
			t += VertPadding;
			// Vertical bottom padding
			b -= VertPadding/2;
			b -= VertPadding/3; // previous is too little... why?
			m_pxGraphArea.SetTop(t);
			m_pxGraphArea.SetBottom(b);
			break;

		case wxCHARTAXISPOSITION_BOTTOM:
			// Vertical top padding:
			t += VertPadding/2;
			// Vertical bottom padding
			b -= VertPadding;
			b -= VertPadding/2;	// Why?
			m_pxGraphArea.SetTop(t);
			m_pxGraphArea.SetBottom(b);
			break;
		default:
			LogFunction("WHAT?");
			break;
	}

	switch (YLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
			l += HorzPadding/2;
			r -= HorzPadding;
			m_pxGraphArea.SetLeft(l);
			m_pxGraphArea.SetRight(r);
			break;
		case wxCHARTAXISPOSITION_LEFT:
			l += HorzPadding;
			r -= HorzPadding/2;	// No labels here, keep space for half of the X axis
			m_pxGraphArea.SetLeft(l);
			m_pxGraphArea.SetRight(r);
			break;
		default:
			LogFunction("WHAT?");
			break;
	}
#ifdef USE_WXPOINTDOUBLE
//	m_XAxis->Fit(wxPoint2DDouble(m_pxGraphArea.GetLeftBottom()), wxPoint2DDouble(m_pxGraphArea.GetRightBottom()));
//	m_YAxis->Fit(wxPoint2DDouble(m_pxGraphArea.GetLeftBottom()), wxPoint2DDouble(m_pxGraphArea.GetLeftTop()));
#else
//	m_XAxis->Fit(m_pxGraphArea.GetLeftBottom(), m_pxGraphArea.GetRightBottom());
//	m_YAxis->Fit(m_pxGraphArea.GetLeftBottom(), m_pxGraphArea.GetLeftTop());
#endif // USE_WXPOINTDOUBLE
/*
	//-_- pass to each axis the min/max range and the "fixed" orthogonal position
	int xAxisY = m_XAxis->GetOptions().IsOpposite()?t:b;
	int yAxisX = m_YAxis->GetOptions().IsOpposite()?r:l;

	m_XAxis->SetRangeAndPosition(
		wxPoint(l,xAxisY),
		wxPoint(r,xAxisY)
	);
	m_XAxis->SetRangeAndPosition(
		wxPoint(b,yAxisX),
		wxPoint(t,yAxisX)
	);
*/
	m_XAxis->FitInThisRect(m_pxGraphArea);
	m_YAxis->FitInThisRect(m_pxGraphArea);
	// Recalculate the labels positions along the axis
    m_XAxis->UpdateLabelPositions();
    m_YAxis->UpdateLabelPositions();

    m_needsFit = false;
}

void wxChartGrid::Resize(const wxSize &NewSize)
{
    if (m_mapping.GetSize()!=NewSize) {
		m_mapping.SetSize(NewSize);
		m_needsFit = true;
    }
}

const wxChartGridMapping& wxChartGrid::GetMapping() const
{
    return m_mapping;
}

void wxChartGrid::SetOptions(const wxChartGridOptions& opt)
{
    m_options = opt;
    m_XAxis->SetOptions( opt.GetXAxisOptions() );
    m_YAxis->SetOptions( opt.GetYAxisOptions() );
    Update();
}

const wxChartGridOptions& wxChartGrid::GetOptions() const
{
    return m_options;
}

bool wxChartGrid::Scale(int c)
{
    if(c)
    {
        c = c>0 ? 4 : -2;
        double deltaX = (m_curAxisLimits.MaxX-m_curAxisLimits.MinX)/c;
        double deltaY = (m_curAxisLimits.MaxY-m_curAxisLimits.MinY)/c;

        auto absDX = std::abs(deltaX);
        auto absDY = std::abs(deltaY);

        if(absDX > MaxDistance || absDX < MinDistance
                || absDY > MaxDistance || absDY < MinDistance)
            return false;

        m_curAxisLimits.MinX+=deltaX;
        m_curAxisLimits.MaxX-=deltaX;
        m_curAxisLimits.MinY+=deltaY;
        m_curAxisLimits.MaxY-=deltaY;
    }
    else
        m_curAxisLimits = m_origAxisLimits;
    Update();
    return true;
}

void wxChartGrid::Shift(double dx,double dy)
{
    double deltaX = (m_curAxisLimits.MaxX-m_curAxisLimits.MinX)*dx;
    m_curAxisLimits.MinX+=deltaX;
    m_curAxisLimits.MaxX+=deltaX;

    double deltaY = (m_curAxisLimits.MaxY-m_curAxisLimits.MinY)*dy;
    m_curAxisLimits.MinY+=deltaY;
    m_curAxisLimits.MaxY+=deltaY;

    Update();
}

void wxChartGrid::UpdateAxisLimit(const std::string& axisId, wxDouble min, wxDouble max)
{
    if(axisId == "x")
    {
        m_origAxisLimits.MinX = min;
        m_origAxisLimits.MaxX = max;

        m_curAxisLimits.MinX = min;
        m_curAxisLimits.MaxX = max;

        m_XAxis = new wxChartNumericalAxis(axisId,m_curAxisLimits.MinX,
                                           m_curAxisLimits.MaxX,m_options.GetXAxisOptions());
    }
    else if(axisId == "y")
    {
        m_origAxisLimits.MinY = min;
        m_origAxisLimits.MaxY = max;

        m_curAxisLimits.MinY = min;
        m_curAxisLimits.MaxY = max;
        m_YAxis = new wxChartNumericalAxis(axisId,m_curAxisLimits.MinY,
                                           m_curAxisLimits.MaxY, m_options.GetYAxisOptions());
    }
    m_mapping = wxChartGridMapping(m_mapping.GetSize(), m_XAxis, m_YAxis);
    m_needsFit = true;
}

void wxChartGrid::UpdateForcedAxisLimit(const std::string& axisId, wxDouble min, wxDouble max)
{
    if(axisId == "x")
    {
        m_origAxisLimits.MinX = min;
        m_origAxisLimits.MaxX = max;

        m_curAxisLimits.MinX = min;
        m_curAxisLimits.MaxX = max;

        m_XAxis = new wxChartNumericalAxis(
			axisId,
			m_curAxisLimits.MinX,
            m_curAxisLimits.MaxX,
            m_options.GetXAxisOptions(),
			true
		);
    }
    else if(axisId == "y")
    {
        m_origAxisLimits.MinY = min;
        m_origAxisLimits.MaxY = max;

        m_curAxisLimits.MinY = min;
        m_curAxisLimits.MaxY = max;
		m_YAxis = new wxChartNumericalAxis(
			axisId,
			m_curAxisLimits.MinY,
			m_curAxisLimits.MaxY,
			m_options.GetYAxisOptions(),
			true
		);
    }
    m_mapping = wxChartGridMapping(m_mapping.GetSize(), m_XAxis, m_YAxis);
    m_needsFit = true;
}

void wxChartGrid::ChangeLabels(const std::string& axisId, const wxVector<wxString> &labels, wxChartAxisOptions options)
{
    if(axisId == "x")
        m_XAxis = wxChartCategoricalAxis::make_shared(axisId,labels,options);
    else if(axisId == "y")
        m_YAxis = wxChartCategoricalAxis::make_shared(axisId,labels,options);

    m_mapping = wxChartGridMapping(m_mapping.GetSize(), m_XAxis, m_YAxis);
    m_needsFit = true;
}

void wxChartGrid::Update()
{
    m_XAxis = new wxChartNumericalAxis("x", m_curAxisLimits.MinX,
        m_curAxisLimits.MaxX,m_options.GetXAxisOptions());
    m_YAxis = new wxChartNumericalAxis("y", m_curAxisLimits.MinY,
        m_curAxisLimits.MaxY, m_options.GetYAxisOptions());
    m_mapping = wxChartGridMapping(m_mapping.GetSize(), m_XAxis, m_YAxis);
    m_needsFit = true;
}
//-_-
/*
void wxChartGrid::CalculatePadding(const wxChartAxis &xAxis,
                                   const wxChartAxis &yAxis,
                                   wxDouble &left,
                                   wxDouble &right)
{
    wxChartAxisLocation	XLocation = xAxis.GetOptions().GetLocation();
    //wxChartAxisLocation	YLocation = yAxis.GetOptions().GetLocation();

    size_t szY = yAxis.GetLabels().size();
    size_t szX = xAxis.GetLabels().size();

    int firstLabelWidth;


	switch (XLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
			LogFunction("RIGHT");
		case wxCHARTAXISPOSITION_LEFT:
			{
				// Either the first y label or any of the x labels can be the widest
				// so they are all taken into account to compute the left padding
				left = xAxis.GetLabels().GetMaxWidth();// + 10;
				right = 0;
				firstLabelWidth = yAxis.GetLabels().front().GetSize().GetWidth() / 2;

				if ( szY>0 ) {
					if ( firstLabelWidth>left ) {
						left = firstLabelWidth;
					}
					right = (yAxis.GetLabels().back().GetSize().GetWidth() / 2);
				}
				right +=50;
				left+=20;
			}
			break;

		case wxCHARTAXISPOSITION_TOP:
			LogFunction("TOP");
		case wxCHARTAXISPOSITION_BOTTOM:
			{
				// Either the first x label or any of the y labels can be the widest
				// so they are all taken into account to compute the left padding
				left = yAxis.GetLabels().GetMaxWidth();// + 10;
				right = 0;
				firstLabelWidth = xAxis.GetLabels().front().GetSize().GetWidth() / 2;

				if ( szY>0 ) {
					if ( firstLabelWidth>left ) {
						left = firstLabelWidth;
					}
					right = (xAxis.GetLabels().back().GetSize().GetWidth() / 2);
				}
				left += 50;
				right+=20;
			}
			break;
		default:
			LogFunction("WHAT?");
			break;
	}
*/ //-_-
/*
    if (XLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        // Either the first x label or any of the y labels can be the widest
        // so they are all taken into account to compute the left padding
        left = yAxis.GetLabels().GetMaxWidth() + 10;
        if ((xAxis.GetLabels().size() > 0) && ((xAxis.GetLabels().front().GetSize().GetWidth() / 2) > left))
        {
            left = (xAxis.GetLabels().front().GetSize().GetWidth() / 2);
        }

        right = 0;
        if (xAxis.GetLabels().size() > 0)
        {
            right = (xAxis.GetLabels().back().GetSize().GetWidth() / 2);
        }
    }
    else if (XLocation == wxCHARTAXISPOSITION_LEFT)
    {
        // Either the first y label or any of the x labels can be the widest
        // so they are all taken into account to compute the left padding
        left = xAxis.GetLabels().GetMaxWidth() + 10;
        if ((yAxis.GetLabels().size() > 0) && ((yAxis.GetLabels().front().GetSize().GetWidth() / 2) > left))
        {
            left = (yAxis.GetLabels().front().GetSize().GetWidth() / 2);
        }

        right = 0;
        if (yAxis.GetLabels().size() > 0)
        {
            right = (yAxis.GetLabels().back().GetSize().GetWidth() / 2);
        }
    }
*/
/* -_-
}
*/ //-_-

void wxChartGrid::DrawHorizontalGridLines(const wxChartAxis &horizontalAxis,
        const wxChartAxis &verticalAxis,
        const wxChartGridLineOptions &options,
        wxGraphicsContext &gc)
{
    for (size_t i = 1; i < verticalAxis.GetNumberOfTickMarks(); ++i)
    {
        wxPoint2DDouble lineStartPosition = verticalAxis.GetTickMarkPosition(i);
        wxPoint2DDouble lineEndPosition = horizontalAxis.GetTickMarkPosition(horizontalAxis.GetNumberOfTickMarks() - 1);
        if (horizontalAxis.GetOptions().GetEndMarginType() == wxCHARTAXISMARGINTYPE_TICKMARKOFFSET)
        {
            lineEndPosition.m_x += horizontalAxis.GetDistanceBetweenTickMarks();
        }

        wxGraphicsPath path = gc.CreatePath();
        path.MoveToPoint(lineStartPosition);
        path.AddLineToPoint(lineEndPosition.m_x + horizontalAxis.GetOptions().GetOverhang(), lineStartPosition.m_y);

        wxPen pen1(options.GetMajorGridLineColor(), options.GetMajorGridLineWidth());
        gc.SetPen(pen1);
        gc.StrokePath(path);

        unsigned int n = options.GetNumberOfMinorGridLinesBetweenTickMarks();
        if (n != 0)
        {
            wxDouble spacing = verticalAxis.GetDistanceBetweenTickMarks() / (n + 1);
            wxDouble currentSpacing = spacing;
            for (size_t j = 0; j < n; ++j)
            {
                wxGraphicsPath path = gc.CreatePath();
                path.MoveToPoint(lineStartPosition.m_x, lineStartPosition.m_y + currentSpacing);
                path.AddLineToPoint(lineEndPosition.m_x + horizontalAxis.GetOptions().GetOverhang(), lineStartPosition.m_y + currentSpacing);

                wxPen pen1(wxColor(0, 0, 0, 0x0C), 1);
                gc.SetPen(pen1);
                gc.StrokePath(path);

                currentSpacing += spacing;
            }
        }
    }
}

void wxChartGrid::DrawVerticalGridLines(const wxChartAxis &horizontalAxis,
                                        const wxChartAxis &verticalAxis,
                                        const wxChartGridLineOptions &options,
                                        wxGraphicsContext &gc)
{
    size_t i = 1;
    if (horizontalAxis.GetOptions().GetStartMarginType() == wxCHARTAXISMARGINTYPE_TICKMARKOFFSET)
    {
        i = 0;
    }
    size_t end = horizontalAxis.GetNumberOfTickMarks();
    if (horizontalAxis.GetOptions().GetEndMarginType() == wxCHARTAXISMARGINTYPE_TICKMARKOFFSET)
    {
        ++end;
    }
    for (; i < end; ++i)
    {
        wxPoint2DDouble lineStartPosition = horizontalAxis.GetTickMarkPosition(i);
        wxPoint2DDouble lineEndPosition = verticalAxis.GetTickMarkPosition(verticalAxis.GetNumberOfTickMarks() - 1);

        wxGraphicsPath path = gc.CreatePath();
        path.MoveToPoint(lineStartPosition);
        path.AddLineToPoint(lineStartPosition.m_x, lineEndPosition.m_y - verticalAxis.GetOptions().GetOverhang());

        wxPen pen1(options.GetMajorGridLineColor(), options.GetMajorGridLineWidth());
        gc.SetPen(pen1);
        gc.StrokePath(path);

        unsigned int n = options.GetNumberOfMinorGridLinesBetweenTickMarks();
        if (n != 0)
        {
            wxDouble spacing = horizontalAxis.GetDistanceBetweenTickMarks() / (n + 1);
            wxDouble currentSpacing = spacing;
            for (size_t j = 0; j < n; ++j)
            {
                wxGraphicsPath path = gc.CreatePath();
                path.MoveToPoint(lineStartPosition.m_x - currentSpacing, lineStartPosition.m_y);
                path.AddLineToPoint(lineStartPosition.m_x - currentSpacing, lineEndPosition.m_y - verticalAxis.GetOptions().GetOverhang());

                wxPen pen1(wxColor(0, 0, 0, 0x0C), 1);
                gc.SetPen(pen1);
                gc.StrokePath(path);

                currentSpacing += spacing;
            }
        }
    }
}
