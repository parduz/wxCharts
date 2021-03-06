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

	Copyright (c) 2013-2017 Nick Downie
	Released under the MIT license
	https://github.com/nnnick/Chart.js/blob/master/LICENSE.md
*/

/// @file

#ifndef _WX_CHARTS_WXMATH2DPLOT_H_
#define _WX_CHARTS_WXMATH2DPLOT_H_

#include "wxchart.h"
#include "wxmath2dplotoptions.h"
#include "wxchartgrid.h"
#include "wxchartpoint.h"

enum wxChartType
{
	wxCHARTTYPE_LINE,
	wxCHARTTYPE_STEPPED,
	wxCHARTTYPE_STEM
};

/// Stores the information about a dataset to be shown

/// \ingroup dataclasses
class wxMath2DPlotDataset
{
public:
	/// Smart pointer typedef.
	typedef wxSharedPtr<wxMath2DPlotDataset> ptr;

	/// Constructs a Math2DPlot instance.
	/// @param label The name of the dataset. This
	/// is the name that will be used in the legend.
	/// @param dotColor The color of the points.
	/// @param dotStrokeColor The color of the pen
	/// used to draw the outline of the points.
	/// @param fillColor The part of the graph between
	/// the X-axis and the line will be filled using
	/// this color.
	/// @param data The list of values.
	/// @param showDots Enable display of dots on lines.
	wxMath2DPlotDataset(
		const wxString &label,
		const wxColor &dotColor,
		const wxColor &dotStrokeColor,
		const wxColor &fillColor,
		wxVector<wxPoint2DDouble> &data,
		const wxChartType &chartType = wxCHARTTYPE_LINE,
		const bool &showLine = true,
		const bool &showDots = true
	);

	/// Gets the name of the dataset.
	/// @return The name of the dataset.
	const wxString& GetLabel() const;
	/// Whether to show the points on the chart.
	/// @retval true Show the points.
	/// @retval false Don't show the points.
	bool ShowDots() const;
	const wxColor& GetDotColor() const;
	const wxColor& GetDotStrokeColor() const;
	/// Whether to show the line on the chart.
	/// @retval true Show the line.
	/// @retval false Don't show the line.
	bool ShowLine() const;
	const wxColor& GetLineColor() const;
	/// Whether to fill the part of the chart
	/// between the line and X-axis with the
	/// color returned by GetFillColor().
	/// @retval true Fill.
	/// @retval false Don't fill.
	bool Fill() const;
	/// Returns the color with which to
	/// fill the part of the chart between
	/// the line and the X-axis.
	/// @return The fill color.
	const wxColor& GetFillColor() const;
	const wxVector<wxPoint2DDouble>& GetData() const;
	const wxChartType& GetType() const;

private:
	wxString m_label;
	bool m_showDots;
	wxColor m_dotColor;
	wxColor m_dotStrokeColor;
	bool m_showLine;
	wxColor m_lineColor;
	bool m_fill;
	wxColor m_fillColor;
	wxVector<wxPoint2DDouble> m_data;
	wxChartType m_type;
};

/// Data for the wxMath2DPlotCtrl control.

/// \ingroup dataclasses
class wxMath2DPlotData
{
public:
	/// Constructs a wxMath2DPlotData instance.
	wxMath2DPlotData();
	/// @param labels The labels of the X axis.
	wxMath2DPlotData(const wxVector<wxString> &labels);

	/// Adds a dataset.
	/// @param dataset The dataset to add.
	void AddDataset(wxMath2DPlotDataset::ptr dataset);

	/// Gets the labels of the X axis.
	/// @return A vector containing the labels of the
	/// X axis.
	const wxVector<wxString>& GetLabels() const;
	const wxVector<wxMath2DPlotDataset::ptr>& GetDatasets() const;

private:
	wxVector<wxString> m_labels;
	wxVector<wxMath2DPlotDataset::ptr> m_datasets;
};

/// A wxMath2D plot.

/// \ingroup chartclasses
class wxMath2DPlot : public wxChart
{
public:
	wxMath2DPlot(
		const wxMath2DPlotData &data,
		const wxSize &size
	);
	wxMath2DPlot(
		const wxMath2DPlotData &data,
		const wxMath2DPlotOptions &options,
		const wxSize &size
	);

	virtual const wxChartCommonOptions& GetCommonOptions() const;

	void Save(
		const wxString &filename,
		const wxBitmapType &type,
		const wxSize &size
	);

	bool Scale					(int coeff);
	void Shift					(double dx,double dy);
	bool UpdateData				(std::size_t index,const wxVector<wxPoint2DDouble> &points);
	bool UpdateDataPoints		(std::size_t index,const wxVector<wxPoint2DDouble> &points);
	bool AddData				(std::size_t index,const wxVector<wxPoint2DDouble> &points);
	bool RemoveDataset			(std::size_t index);
	void AddDataset				(const wxMath2DPlotDataset::ptr &newset,bool is_new = true);

	bool UpdateDataPointsFromArrayOfBytes(std::size_t index, const unsigned char *byteArray, const std::size_t ArraySize);


	const wxChartGridOptions&	GetGridOptions	() const;
	void  						SetGridOptions	(const wxChartGridOptions& opt);
	const wxMath2DPlotOptions&	GetChartOptions	() const;
	void  						SetChartOptions	(const wxMath2DPlotOptions& opt);
	bool  						SetChartType	(std::size_t index,const wxChartType &type);

	void SetAutoAxesRange();
	void SetFixedAxesRange(const wxPoint2DDouble& min,const wxPoint2DDouble& max);

	void SetForcedAxesRange(const wxPoint2DDouble& min,const wxPoint2DDouble& max);

private:
	void Initialize(const wxMath2DPlotData &data);
	void Update();
	static wxDouble GetMinXValue(const wxVector<wxMath2DPlotDataset::ptr>& datasets,const AxisFunc &F);
	static wxDouble GetMaxXValue(const wxVector<wxMath2DPlotDataset::ptr>& datasets,const AxisFunc &F);
	static wxDouble GetMinYValue(const wxVector<wxMath2DPlotDataset::ptr>& datasets,const AxisFunc &F);
	static wxDouble GetMaxYValue(const wxVector<wxMath2DPlotDataset::ptr>& datasets,const AxisFunc &F);

	virtual void DoSetSize(const wxSize &size);
	virtual void DoFit();
	virtual void DoDraw(wxGraphicsContext &gc, bool suppressTooltips);
	virtual wxSharedPtr<wxVector<const wxChartElement*> > GetActiveElements(const wxPoint &point);

private:
	class Point : public wxChartPoint
	{
	public:
		typedef wxSharedPtr<Point> ptr;

		Point(wxPoint2DDouble value,
			const wxChartTooltipProvider::ptr tooltipProvider,
			wxDouble x, wxDouble y, wxDouble radius,
			unsigned int strokeWidth, const wxColor &strokeColor,
			const wxColor &fillColor, wxDouble hitDetectionRange);

		virtual wxPoint2DDouble GetTooltipPosition() const;
		virtual bool HitTest(const wxPoint &point, const AxisFunc &F) const;

		wxPoint2DDouble GetValue() const;
		void SetValue(const wxPoint2DDouble& val);

	private:
		wxPoint2DDouble m_value;
		wxDouble m_hitDetectionRange;
	};

	class Dataset
	{
	public:
		typedef wxSharedPtr<Dataset> ptr;

		Dataset(
			bool showDots,
			bool showLine,
			const wxColor &lineColor,
			const wxColor &dotStrokeColor,
			bool fill,
			const wxColor &fillColor,

			const wxChartType &chartType=wxCHARTTYPE_LINE
		);

		bool ShowDots() const;
		bool ShowLine() const;
		const wxColor& GetLineColor() const;
		const wxColor& GetDotColor() const;
		const wxColor& GetDotStrokeColor() const;

		bool Fill() const;
		const wxColor& GetFillColor() const;

		const wxChartType& GetType() const;
		void SetType(const wxChartType &chartType);

		const wxVector<Point::ptr>& GetPoints() const;
		void AppendPoint(Point::ptr point);
		void UpdatePoints(const wxVector<wxPoint2DDouble> &points);

		void UpdatePointsFromArrayOfBytes(const unsigned char *byteArray, const std::size_t ArraySize);

	private:
		bool m_showDots;
		bool m_showLine;
		wxColor m_lineColor;
		wxColor m_dotStrokeColor;
		bool m_fill;
		wxColor m_fillColor;
		wxColor m_dotColor;
		wxChartType m_type;
		wxVector<Point::ptr> m_points;
	};

private:
	wxMath2DPlotOptions m_options;
	wxChartGrid m_grid;
	wxVector<Dataset::ptr> m_datasets;
	bool m_autoRange;
};

#endif
