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

/// @file

#ifndef _WX_CHARTS_WXCHARTAXISOPTIONS_H_
#define _WX_CHARTS_WXCHARTAXISOPTIONS_H_

#include "wxchartfontoptions.h"

/// The position of the axis in relation to the chart.

/// The position is used to know where to draw the labels
/// and markers.
enum wxChartAxisLocation
{
    /// The axis is on the left of the chart. Markers
    /// and labels should be drawn on the left of the axis.
//-_-    wxCHARTAXISPOSITION_LEFT = 0,
    wxCHARTAXISPOSITION_LEFT = 1,
    /// The axis is on the right of the chart. Markers
    /// and labels should be drawn on the right of the axis.
//-_-    wxCHARTAXISPOSITION_RIGHT = 1,
    wxCHARTAXISPOSITION_RIGHT = 3,
    /// The axis is at the top of the chart. Markers
    /// and labels should be drawn above the axis.
//-_-    wxCHARTAXISPOSITION_BOTTOM = 2,
    wxCHARTAXISPOSITION_BOTTOM = 0,
    /// The axis is at the bottom of the chart. Markers
    /// and labels should be drawn below the axis.
//-_-    wxCHARTAXISPOSITION_TOP = 3
    wxCHARTAXISPOSITION_TOP = 2
};

//-_-
#define wxCHARTAXIS_VERTICAL	0x0001
#define wxCHARTAXIS_OPPOSITE	0x0002
//-_-


/// Label types for the wxChartAxis class.

/// Labels on an axis can represent 2 things.
/// - A specific value or point, in which case
/// the labels should be centered on the grid
/// lines.
/// - A range, in which case the labels should
/// be positioned in between grid lines.
///
/// Note that the wxChartAxis class can be used
/// for many things so the above description may
/// not make sense for a particular type of data
/// shown on the axis.
///
/// This terminology was chosen because the type of
/// label may affect more than just where they are
/// positioned so something like 'centered' would
/// have its own issues.
enum wxChartAxisLabelType
{
    /// The labels represent specific points on the
    /// axis. Grid lines are drawn at these points.
    wxCHARTAXISLABELTYPE_POINT = 0,
    /// The labels represent portions of the axis.
    /// The start and end of each portion are marked
    /// by the grid lines.
    wxCHARTAXISLABELTYPE_RANGE = 1
};

/// Margin types for the gap between the first label and the start
/// of the axis (start margin) or between the last label
/// and the end of the axis (end margin).
enum wxChartAxisMarginType
{
    /// No margin.
    wxCHARTAXISMARGINTYPE_NONE = 0,
    /// In the case of the start margin the first tick mark
    /// is usually immediately located at the start of
    /// the axis. This margin type will offset the start of the first tick mark
    /// by the distance between two tick marks. It's like inserting a hidden
    /// tick mark before the first tick mark.
    /// In the case of the end margin the last tick mark
    /// is usually located right at the end of
    /// the axis. This margin type will offset the end of the last tick mark
    /// by the distance between two tick marks. It's like inserting a hidden
    /// tick mark after the last tick mark.
    wxCHARTAXISMARGINTYPE_TICKMARKOFFSET = 1
};

enum wxChartAxisValueMode
{
    wxCHARTAXISVALUEMODE_AUTO = 0,
    wxCHARTAXISVALUEMODE_EXPLICIT = 1
};

/// The options for the wxChartAxis element.

/// \ingroup elementclassess
class wxChartAxisOptions
{
public:
    /// Constructs a new wxChartAxisOptions
    /// instance. The axis will use the wxCHARTAXISLABELTYPE_POINT type
    /// as label type.
    /// @param location The location of the axis.
    wxChartAxisOptions(wxChartAxisLocation location);
    /// Constructs a new wxChartAxisOptions
    /// instance.
    /// @param location The location of the axis.
    /// @param labelType The type of labels.
    wxChartAxisOptions(wxChartAxisLocation location, wxChartAxisLabelType labelType);

    /// Gets the location of the axis.
    /// @return The location of the axis.
    wxChartAxisLocation GetLocation() const;
    void SetLocation(wxChartAxisLocation newPos);
    bool IsHorizontal()	const {return  ((m_location&wxCHARTAXIS_VERTICAL)==0); }; //-_-
    bool IsVertical()	const {return  ((m_location&wxCHARTAXIS_VERTICAL)!=0); }; //-_-
    bool IsOpposite()	const {return  ((m_location&wxCHARTAXIS_OPPOSITE)!=0); }; //-_-

    /// Gets the label type.
    /// @return The label type.
    wxChartAxisLabelType GetLabelType() const;

    /// Gets the start margin type.
    /// @return The start margin type.
    wxChartAxisMarginType GetStartMarginType() const;
    /// Sets the type of the start margin.
    /// @param type The margin type.
    void SetStartMarginType(wxChartAxisMarginType type);
    /// Gets the end margin type.
    /// @return The end margin type.
    wxChartAxisMarginType GetEndMarginType() const;
    /// Sets the type of the end margin.
    /// @param type The margin type.
    void SetEndMarginType(wxChartAxisMarginType type);

    wxDouble GetStartValue() const;
    wxDouble GetEndValue() const;

    wxChartAxisValueMode GetStartValueMode() const;
    wxChartAxisValueMode GetEndValueMode() const;

    void SetExplicitStartValue(wxDouble startValue);
    void SetExplicitEndValue(wxDouble endValue);

    /// Gets the width of the line.
    /// @return The width of the line.
    unsigned int GetLineWidth() const;
	void  SetLineWidth(unsigned int newWidth);
    /// Gets the color of the line.
    /// @return The color of the line.
    const wxColor& GetLineColor() const;
	void  SetLineColor(const wxColor NewColor);

    /// Gets the amount by which the axis is drawn
    /// past the last tick mark.
    /// @return The overhang.
    wxDouble GetOverhang() const;

    /// Gets the font options for the labels.
    /// @return The font options for the labels.
    const wxChartFontOptions& GetFontOptions() const;

    const bool IsReversed() const {return m_reversed;} ;
    void SetReversed(bool isReversed) {m_reversed = isReversed;};

private:
    wxChartAxisLocation		m_location;
    wxChartAxisLabelType	m_labelType;
    wxChartAxisMarginType	m_startMarginType;
    wxChartAxisMarginType	m_endMarginType;
    wxChartAxisValueMode	m_startValueMode;
    wxChartAxisValueMode	m_endValueMode;
    wxDouble				m_startValue;
    wxDouble				m_endValue;
    unsigned int			m_lineWidth;
    wxColor					m_lineColor;
    wxDouble				m_overhang;
    wxChartFontOptions		m_fontOptions;
    bool					m_reversed;
};

#endif
