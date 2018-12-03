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

#include "wxchartnumericalaxis.h"
#include "wxchartutilities.h"
/*
wxChartNumericalAxis::wxChartNumericalAxis(const std::string &id,
                                           wxDouble minValue,
                                           wxDouble maxValue,
                                           const wxChartAxisOptions &options)
*/
wxChartNumericalAxis::wxChartNumericalAxis(const std::string &id,
                                           wxDouble minValue,
                                           wxDouble maxValue,
                                           const wxChartAxisOptions &options,
                                           bool Forced
                                           )
    : wxChartAxis(id, options),
    m_minValue(minValue), m_maxValue(maxValue)
{
    wxDouble effectiveMinXValue = minValue;
    if (options.GetStartValueMode() == wxCHARTAXISVALUEMODE_EXPLICIT)
    {
        effectiveMinXValue = options.GetStartValue();
    }
    wxDouble effectiveMaxXValue = maxValue;
    if (options.GetEndValueMode() == wxCHARTAXISVALUEMODE_EXPLICIT)
    {
        effectiveMaxXValue = options.GetEndValue();
    }



    wxDouble graphMinXValue;
    wxDouble graphMaxXValue;
    wxDouble xValueRange = 0;
    size_t steps = 0;
    wxDouble stepValue = 0;

    if ( Forced || options.GetEndValueMode() == wxCHARTAXISVALUEMODE_EXPLICIT ) {
		graphMinXValue = floor(effectiveMinXValue);
		graphMaxXValue = ceil(effectiveMaxXValue);
		xValueRange = int(graphMaxXValue - graphMinXValue);
		if (fmod(xValueRange,10) == 0) {
			steps = 10;
		}else if (fmod(xValueRange,8) == 0) {
			steps = 8;
		}else if (fmod(xValueRange,6) == 0) {
			steps = 6;
		}else if (fmod(xValueRange,5) == 0) {
			steps = 5;
		}else if (fmod(xValueRange,4) == 0) {
			steps = 4;
		}else if (fmod(xValueRange,3) == 0) {
			steps = 3;
		}else if (fmod(xValueRange,2) == 0) {
			steps = 2;
		}else{
			steps = 1;
		}
		stepValue = xValueRange/steps;
    }else{
        wxChartUtilities::CalculateGridRange(
			effectiveMinXValue,
			effectiveMaxXValue,
			graphMinXValue,
			graphMaxXValue,
			xValueRange,
			steps,
			stepValue
		);
    }

    SetMinValue(graphMinXValue);
    SetMaxValue(graphMaxXValue);

    wxVector<wxChartLabel> xLabels;
    wxChartUtilities::BuildNumericalLabels(
        GetMinValue(),
        steps,
        stepValue,
        wxChartLabelOptions(options.GetFontOptions(), false, wxChartBackgroundOptions(*wxWHITE, 0)),
        xLabels);
    SetLabels(xLabels);
}

wxChartNumericalAxis::ptr wxChartNumericalAxis::make_shared(const std::string &id,
                                                            wxDouble minValue,
                                                            wxDouble maxValue,
                                                            const wxChartAxisOptions &options)
{
    return ptr(new wxChartNumericalAxis(id, minValue, maxValue, options));
}

wxDouble wxChartNumericalAxis::GetMinValue() const
{
    return m_minValue;
}

void wxChartNumericalAxis::SetMinValue(wxDouble minValue)
{
    m_minValue = minValue;
}

wxDouble wxChartNumericalAxis::GetMaxValue() const
{
    return m_maxValue;
}

void wxChartNumericalAxis::SetMaxValue(wxDouble maxValue)
{
    m_maxValue = maxValue;
}
