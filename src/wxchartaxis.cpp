/*
    Copyright (c) 2016-2018 Xavier Leclercq

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

#include "wxchartaxis.h"
#include <wx/pen.h>

#define LogFunction(str)		fprintf(stdout, "   - +++ %s: %s\n",__FUNCTION__,(str))

#ifndef LogFunction
 #define LogFunction(str)
#endif // LogFunction

#define pSTART		m_startPoint
#define pEND		m_endPoint
#define pSTART_X	pSTART.m_x
#define pSTART_Y	pSTART.m_y
#define pEND_X		pEND.m_x
#define pEND_Y		pEND.m_y



//-_- Should'nt be a property?
#define TICKMARKS_LENGTH	5


//---------------------------------------------------------------------------------------
void wxChartAxis::Draw(wxGraphicsContext &gc) const
{
    wxPen pen(m_options.GetLineColor(), m_options.GetLineWidth());
    gc.SetPen(pen);

    // Draw the axis
    wxGraphicsPath path = gc.CreatePath();
    wxChartAxisLocation	chaxLocation = m_options.GetLocation();

	switch (chaxLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
		case wxCHARTAXISPOSITION_LEFT:
			path.MoveToPoint(m_startPoint);
			path.AddLineToPoint(pEND_X, pEND_Y - m_options.GetOverhang());
			break;
		case wxCHARTAXISPOSITION_TOP:
//			path.MoveToPoint(m_endPoint);
//			path.AddLineToPoint(pSTART_X + m_options.GetOverhang(), pEND_Y);
//			break;
		case wxCHARTAXISPOSITION_BOTTOM:
			path.MoveToPoint(m_startPoint);
			path.AddLineToPoint(pEND_X + m_options.GetOverhang(), pEND_Y);
			break;
		default:
			LogFunction("WHAT?");
			break;
	}
/*
    if (chaxLocation == wxCHARTAXISPOSITION_LEFT)
    {
        path.MoveToPoint(m_startPoint);
        path.AddLineToPoint(pEND_X, pEND_Y - m_options.GetOverhang());
    }
    else if (chaxLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        path.MoveToPoint(m_startPoint);
        path.AddLineToPoint(pEND_X + m_options.GetOverhang(), pEND_Y);
    }
*/
    gc.StrokePath(path);

    DrawTickMarks(gc);	// Draw the little lines corresponding to the labels
    m_labels.Draw(gc);
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
wxChartAxis::wxChartAxis(const std::string &id,
                         const wxChartAxisOptions &options)
    : m_options(options),
      m_id(id),
      m_startPoint(0, 0),
      m_endPoint(0, 0)
{
	UpdateLabelsHeight();	 //-_- OPTIMIZE: do it only if differences between old and new options
	UpdateLabelsFixedPositions();	 //-_- OPTIMIZE: do it only if differences between old and new options
}
//---------------------------------------------------------------------------------------
wxChartAxis::wxChartAxis(const std::string &id,
                         const wxVector<wxString> &labels,
                         const wxChartAxisOptions &options)
    : m_options(options),
      m_id(id),
      m_startPoint(0, 0),
      m_endPoint(0, 0)
{
    for (size_t i = 0; i < labels.size(); ++i)
    {
        m_labels.push_back(
            wxChartLabel(
                labels[i],
                wxChartLabelOptions(
					m_options.GetFontOptions(),
					false,
					wxChartBackgroundOptions(*wxWHITE, 0)
				)
			)
		);
    }
	UpdateLabelsHeight();	 //-_- OPTIMIZE: do it only if differences between old and new options
	UpdateLabelsFixedPositions();	 //-_- OPTIMIZE: do it only if differences between old and new options
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------



//#######################################################################################
//#######################################################################################
//#######################################################################################
bool wxChartAxis::HitTest(const wxPoint &point) const
{
    return false;
}
//#######################################################################################
myPointType wxChartAxis::GetTooltipPosition() const
{
    return myPointType(0, 0);
}



#ifdef USE_WXPOINTDOUBLE
 void wxChartAxis::Fit(myPointType startPoint, myPointType endPoint)
#else
 void wxChartAxis::Fit(wxPoint startPoint, wxPoint endPoint)
#endif // USE_WXPOINTDOUBLE
{

    if (m_options.IsReversed()) {
		m_startPoint = endPoint;
		m_endPoint = startPoint;
    }else{
		m_startPoint = startPoint;
		m_endPoint = endPoint;
    }
	UpdateLabelsFixedPositions();	 //-_- OPTIMIZE: do it only if differences between old and new options
}

void wxChartAxis::UpdateLabelSizes(wxGraphicsContext &gc)
{
    m_labels.UpdateSizes(gc);
    pxMaxLabelDimensions.x = m_labels.GetMaxWidth();
	UpdateLabelsHeight();	 //-_- OPTIMIZE: do it only if differences between old and new options
	UpdateLabelsFixedPositions();	 //-_- OPTIMIZE: do it only if differences between old and new options
}

void wxChartAxis::UpdateLabelPositions()
{
    for (size_t i = 0; i < m_labels.size(); ++i)
    {
        m_labels[i].SetPosition(CalculateLabelPosition(i));
    }
}


//#######################################################################################
//### GET/SET PROPERTIES ################################################################
//#######################################################################################
const wxChartLabelGroup& wxChartAxis::GetLabels() const
{
    return m_labels;
}
//#######################################################################################
void wxChartAxis::SetLabels(const wxVector<wxChartLabel> &labels)
{
    m_labels.assign(labels.begin(), labels.end());
	UpdateLabelsHeight();	 //-_- OPTIMIZE: do it only if differences between old and new options
	UpdateLabelsFixedPositions();	 //-_- OPTIMIZE: do it only if differences between old and new options
}
//#######################################################################################
const std::string& wxChartAxis::GetId() const
{
    return m_id;
}
//#######################################################################################
const wxChartAxisOptions& wxChartAxis::GetOptions() const
{
    return m_options;
}
//#######################################################################################
void wxChartAxis::SetOptions(const wxChartAxisOptions& newOpt)
{
    m_options = newOpt;

	UpdateLabelsHeight();	 		//-_- OPTIMIZE: do it only if differences between old and new options
	UpdateLabelsFixedPositions();	//-_- OPTIMIZE: do it only if differences between old and new options
}
//#######################################################################################




//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lll Label Functions lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
myPointType wxChartAxis::CalculateLabelPosition(size_t index)
{
	myPointType position;

    wxChartAxisLocation	chaxLocation = m_options.GetLocation();
	switch (chaxLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
			position.m_x = pxLabelX;
			position.m_y = pSTART_Y - (distance * index) - pxLabelY;
			//-_- position.m_y = pSTART_Y - (distance * index) - (m_labels[index].GetSize().GetHeight() / 2) - marginCorrection;
			return position;
			break;

		case wxCHARTAXISPOSITION_LEFT:
			position.m_x = pxLabelX - m_labels[index].GetSize().GetWidth();
			position.m_y = pSTART_Y - (distance * index) - pxLabelY;
			//-_- position.m_y = pSTART_Y - (distance * index) - (m_labels[index].GetSize().GetHeight() / 2) - marginCorrection;
			return position;
			break;

		case wxCHARTAXISPOSITION_TOP:
			position.m_y = pxLabelY; //-_- pSTART_Y - 8;
			position.m_x = pxLabelX + (distance * index) - (m_labels[index].GetSize().GetWidth() / 2);
			return position;
			break;

		case wxCHARTAXISPOSITION_BOTTOM:
			position.m_y = pxLabelY; //-_- pSTART_Y + 8;
			position.m_x = pxLabelX + (distance * index) - (m_labels[index].GetSize().GetWidth() / 2);
			return position;
			break;

		default:
			LogFunction("WHAT?");
			break;
	}
/*
    if (chaxLocation == wxCHARTAXISPOSITION_LEFT)
    {
        wxDouble distance = GetDistanceBetweenTickMarks();
        wxPoint2DDouble position(
            pSTART_X - 10 - m_labels[index].GetSize().GetWidth(),
            pSTART_Y - (distance * index) - (m_labels[index].GetSize().GetHeight() / 2) - marginCorrection
            );

        if (m_options.GetLabelType() == wxCHARTAXISLABELTYPE_RANGE)
        {
            position.m_y -= (distance / 2);
        }

        return position;
    }
    else if (chaxLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        wxDouble distance = GetDistanceBetweenTickMarks();
        wxPoint2DDouble position(
            pSTART_X + (distance * index) - (m_labels[index].GetSize().GetWidth() / 2) + marginCorrection,
            pSTART_Y + 8
            );


        //innerWidth = this.width - (this.xScalePaddingLeft + this.xScalePaddingRight),
        //valueWidth = innerWidth/Math.max((this.valuesCount - ((this.offsetGridLines) ? 0 : 1)), 1),
        //valueOffset = (valueWidth * index) + this.xScalePaddingLeft;


        if (m_options.GetLabelType() == wxCHARTAXISLABELTYPE_RANGE)
        {
            position.m_x += (distance / 2);
        }

        //return Math.round(valueOffset);

        return position;
    }
*/
    wxTrap();
    return myPointType(0, 0);
}
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
myPointType wxChartAxis::GetPosition(myVarType relativeValue) const
{
    wxChartAxisLocation	chaxLocation = m_options.GetLocation();
	switch (chaxLocation) {
		case wxCHARTAXISPOSITION_RIGHT:
		case wxCHARTAXISPOSITION_LEFT:
			{
				return myPointType(
					pSTART_X,
					pSTART_Y - (relativeValue * (pSTART_Y - pEND_Y))
					);
			}
			break;

		case wxCHARTAXISPOSITION_TOP:
		case wxCHARTAXISPOSITION_BOTTOM:
			{
				return myPointType(
					pSTART_X + (relativeValue * (pEND_X - pSTART_X)),
					pSTART_Y
					);
			}
			break;
		default:
			LogFunction("WHAT?");
			break;
	}
/*    if (chaxLocation == wxCHARTAXISPOSITION_LEFT)
    {
        return wxPoint2DDouble(
            pSTART_X,
            pSTART_Y - (relativeValue * (pSTART_Y - pEND_Y))
            );
    }
    else if (chaxLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        return wxPoint2DDouble(
            pSTART_X + (relativeValue * (pEND_X - pSTART_X)),
            pSTART_Y
            );
    }
*/
    wxTrap();
    return myPointType(0, 0);
}
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
//lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll



//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||| Tickmarks Functions |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
size_t wxChartAxis::GetNumberOfTickMarks() const
{
	//-_- TODO: Make it function of range, number of divisions,
	//-_- Division minimum distance and label size
    if (m_options.GetLabelType() == wxCHARTAXISLABELTYPE_POINT)
    {
        return m_labels.size();
    }
    else
    {
        return (m_labels.size() + 1);
    }
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
myVarType wxChartAxis::GetDistanceBetweenTickMarks() const
{
	//-_- TODO: Make it function of range, number of divisions,
	//-_- Division minimum distance and label size
	int nt = GetNumberOfTickMarks();	//-_-
	if (nt<2) {
		return 0;	//-_- Also saves from a div/0;
	}

    size_t marginCorrection = 0;
    if (m_options.GetStartMarginType() == wxCHARTAXISMARGINTYPE_TICKMARKOFFSET)
    {
        ++marginCorrection;
    }
    if (m_options.GetEndMarginType() == wxCHARTAXISMARGINTYPE_TICKMARKOFFSET)
    {
        ++marginCorrection;
    }

	if (m_options.IsHorizontal()) {
		return ((pEND_X - pSTART_X) / (nt + marginCorrection - 1)); //-_- why -1?
	}else{
		return ((pSTART_Y - pEND_Y) / (nt + marginCorrection - 1)); //-_- why -1?
	}
    wxTrap();
    return 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
myPointType wxChartAxis::GetTickMarkPosition(size_t index) const
{
	myVarType valueOffset = (distance * index) + marginCorrection;

	if (m_options.IsVertical()) {
		valueOffset = pSTART_Y - valueOffset;
		return myPointType(pSTART_X, valueOffset);
	}else{
		valueOffset = pSTART_X + valueOffset;
		return myPointType(valueOffset, pSTART_Y);
	}
    wxTrap();
    return myPointType(0, 0);
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void wxChartAxis::DrawTickMarks(wxGraphicsContext &gc) const
{
    wxChartAxisLocation	chaxLocation = m_options.GetLocation();

    myPointType		TickStart,TickEnd;	//-_-
	wxGraphicsPath path = gc.CreatePath();	//-_-

	size_t n = GetNumberOfTickMarks();
	for (size_t i = 0; i < n; ++i)
	{
		//-_-wxDouble linePositionY = GetTickMarkPosition(i).m_y;
		//-_-wxGraphicsPath path = gc.CreatePath();
		//-_-path.MoveToPoint(pSTART_X - TICKMARKS_LENGTH, linePositionY);
		//-_-path.AddLineToPoint(pSTART_X, linePositionY);
		//-_-path.AddLineToPoint()
		//-_-gc.StrokePath(path);
		TickStart = GetTickMarkPosition(i);	//-_-
		TickEnd = TickStart;				//-_-

		switch (chaxLocation) {
			case wxCHARTAXISPOSITION_RIGHT:
				TickEnd.m_x += TICKMARKS_LENGTH;	//-_-
				break;
			case wxCHARTAXISPOSITION_LEFT:
				TickEnd.m_x -= TICKMARKS_LENGTH;	//-_-
				break;

			case wxCHARTAXISPOSITION_TOP:
				TickEnd.m_y -= TICKMARKS_LENGTH;	//-_-
				break;

			case wxCHARTAXISPOSITION_BOTTOM:
				TickEnd.m_y += TICKMARKS_LENGTH;	//-_-
				break;

			default:
				LogFunction("WHAT?");
				break;
		}
		path.MoveToPoint(TickStart);		//-_-
		path.AddLineToPoint(TickEnd);		//-_-
	}
	gc.StrokePath(path);	//-_-

/*
    if (chaxLocation == wxCHARTAXISPOSITION_LEFT)
    {
        size_t n = GetNumberOfTickMarks();
        for (size_t i = 0; i < n; ++i)
        {
            wxDouble linePositionY = GetTickMarkPosition(i).m_y;

            wxGraphicsPath path = gc.CreatePath();
            path.MoveToPoint(pSTART_X - 5, linePositionY);
            path.AddLineToPoint(pSTART_X, linePositionY);
            gc.StrokePath(path);
        }
    }
    else if (chaxLocation == wxCHARTAXISPOSITION_BOTTOM)
    {
        size_t n = GetNumberOfTickMarks();
        for (size_t i = 0; i < n; ++i)
        {
            wxDouble linePosition = GetTickMarkPosition(i).m_x;

            wxGraphicsPath path = gc.CreatePath();
            path.MoveToPoint(linePosition, pSTART_Y);
            path.AddLineToPoint(linePosition, pSTART_Y + 5);
            gc.StrokePath(path);
        }
    }
*/
}

//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
void wxChartAxis::UpdateLabelsHeight()
{
    pxMaxLabelDimensions.SetHeight( m_options.GetFontOptions().GetSize() );
}
//-_- ------------------------------------------------------------------------------------------------------------
void wxChartAxis::UpdateLabelsFixedPositions()
{
	LogFunction("Update Fixed Position");

	distance = GetDistanceBetweenTickMarks();
    // wxDouble marginCorrection = 0;
    marginCorrection = 0;
    if (m_options.GetStartMarginType() == wxCHARTAXISMARGINTYPE_TICKMARKOFFSET) {
        marginCorrection += distance;
    }

	int pxDifference = 0;
	switch (m_options.GetLocation()) {
		case wxCHARTAXISPOSITION_LEFT:
			pxDifference = 1;
			pxLabelX = pSTART_X - TICKMARKS_LENGTH - marginCorrection - pxDifference; // 1px correction to make it like the right
			break;
		case wxCHARTAXISPOSITION_RIGHT:
			pxLabelX = pSTART_X + TICKMARKS_LENGTH - marginCorrection;
			break;
		case wxCHARTAXISPOSITION_TOP:
			pxDifference = 1;
			pxLabelY = pSTART_Y - TICKMARKS_LENGTH - pxMaxLabelDimensions.y - pxDifference; // 1px correction to make it like the bottom
			break;
		case wxCHARTAXISPOSITION_BOTTOM:
			pxLabelY = pSTART_Y + TICKMARKS_LENGTH;
			break;
		default:
			LogFunction("WHAT?");
			return;
			break;
	}

	if (m_options.IsHorizontal() ) {
		pxLabelX = pSTART_X + marginCorrection;
		if (m_options.GetLabelType() == wxCHARTAXISLABELTYPE_RANGE) {
			pxLabelX += (distance / 2);
		}
		pxPadding = pxMaxLabelDimensions.y + TICKMARKS_LENGTH + pxDifference +1;	//-_- 1px of margin
	}else{
		pxLabelY = pxMaxLabelDimensions.y/2;
		if (m_options.GetLabelType() == wxCHARTAXISLABELTYPE_RANGE) {
			pxLabelY -= (distance / 2);
		}
		pxPadding = pxMaxLabelDimensions.x + TICKMARKS_LENGTH + pxDifference +1;	//-_- 1px of margin
	}
}
//-_- ------------------------------------------------------------------------------------------------------------
void wxChartAxis::FitInThisRect	(const wxRect pxArea)
{

	bool Rev = m_options.IsReversed();

	switch (m_options.GetLocation()) {
		case wxCHARTAXISPOSITION_LEFT:
			if (Rev) {
				pSTART	= pxArea.GetTopLeft();
				pEND	= pxArea.GetBottomLeft();
			}else{
				pSTART	= pxArea.GetBottomLeft();
				pEND	= pxArea.GetTopLeft();
			}
			break;
		case wxCHARTAXISPOSITION_RIGHT:
			if (Rev) {
				pSTART	= pxArea.GetTopRight();
				pEND	= pxArea.GetBottomRight();
			}else{
				pSTART	= pxArea.GetBottomRight();
				pEND	= pxArea.GetTopRight();
			}
			break;
		case wxCHARTAXISPOSITION_TOP:
			if (Rev) {
				pSTART	= pxArea.GetTopRight();
				pEND	= pxArea.GetTopLeft();
			}else{
				pSTART	= pxArea.GetTopLeft();
				pEND	= pxArea.GetTopRight();
			}
			break;
		case wxCHARTAXISPOSITION_BOTTOM:
			if (Rev) {
				pSTART	= pxArea.GetBottomRight();
				pEND	= pxArea.GetBottomLeft();
			}else{
				pSTART	= pxArea.GetBottomLeft();
				pEND	= pxArea.GetBottomRight();
			}
			break;
		default:
			LogFunction("WHAT?");
			return;
			break;
	}
	UpdateLabelsFixedPositions();	 //-_- OPTIMIZE: do it only if differences between old and new options
}
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
//-_- ------------------------------------------------------------------------------------------------------------
