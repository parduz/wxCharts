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

/// @file

#ifndef _WX_CHARTS_WXCHARTAXIS_H_
#define _WX_CHARTS_WXCHARTAXIS_H_

#include "wxchartelement.h"
#include "wxchartaxisoptions.h"
#include "wxchartlabelgroup.h"
#include <wx/graphics.h>

//-_- i plan to see what will happen if i switch everything to
//-_- Int32
#define USE_WXPOINTDOUBLE
#ifdef USE_WXPOINTDOUBLE
 typedef	wxDouble		myVarType;
 typedef	wxPoint2DDouble	myPointType;
#else
 typedef	int				myVarType;
 typedef	wxPoint2DInt	myPointType;
#endif // USE_WXPOINTDOUBLE

/// This class represents an axis.

/// \ingroup elementclasses
class wxChartAxis : public wxChartElement
{
public:
    /// Smart pointer typedef.
    typedef wxSharedPtr<wxChartAxis> ptr;

    virtual void Draw(wxGraphicsContext &gc) const;

    virtual bool HitTest(const wxPoint &point) const;

    virtual myPointType GetTooltipPosition() const;

    /// Updates the size of each label using the
    /// font details specified in the axis options
    /// and the provided graphics context.
    /// @param gc The graphics context.
    void UpdateLabelSizes(wxGraphicsContext &gc);
#ifdef USE_WXPOINTDOUBLE
    void Fit(myPointType startPoint, myPointType endPoint);
#else
    void Fit(wxPoint startPoint, wxPoint endPoint);
#endif
    void UpdateLabelPositions();

    /// Gets the labels.
    /// @return The list of labels.
    const wxChartLabelGroup& GetLabels() const;
    void SetLabels(const wxVector<wxChartLabel> &labels);

    myPointType CalculateLabelPosition(size_t index);
    size_t GetNumberOfTickMarks() const;
    myVarType GetDistanceBetweenTickMarks() const;
    myPointType GetTickMarkPosition(size_t index) const;
    myPointType GetPosition(myVarType relativeValue) const;

    const std::string& GetId() const;
    const	wxChartAxisOptions& GetOptions() const;
	void	SetOptions(const wxChartAxisOptions& newOpt);


	void FitInThisRect			(const wxRect pxArea);	//-_-

	//-_- make getter/setter, or privatize, these stuffs
	//-_- pre-calculate things that doesn't changes until properties/range/size changes
	void		UpdateLabelsHeight			();
	void		UpdateLabelsFixedPositions		();
	myVarType	distance, marginCorrection;	// these were calculated each time, no need to do it
	int			pxLabelX,pxLabelY;			//-_- Coordinates or values in Pixels, pre-calculated coords for drawing labels
	int			pxPadding;					//-_- minimum padding needeed to make rooms for the labels + tickmarks
	wxSize		pxMaxLabelDimensions;


protected:
    /// Construcs a wxChartAxis element.
    /// @param options The settings to be used for the axis.
    wxChartAxis(const std::string &id, const wxChartAxisOptions &options);
    /// Constructs a wxChartAxis element.
    /// @param labels The labels to display along the axis.
    /// @param options The settings to be used for the axis.
    wxChartAxis(const std::string &id, const wxVector<wxString> &labels,
        const wxChartAxisOptions &options);

private:
    void DrawTickMarks(wxGraphicsContext &gc) const;

private:
    wxChartAxisOptions m_options;
    std::string m_id;

     myPointType m_startPoint;
     myPointType m_endPoint;

    wxChartLabelGroup m_labels;
};

#endif
