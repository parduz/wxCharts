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

#include "wxchartlabelgroup.h"

wxChartLabelGroup::wxChartLabelGroup()
    : m_px_maxWidth(0)
{
}

void wxChartLabelGroup::Draw(wxGraphicsContext &gc) const
{
    for (size_t i = 0; i < size(); ++i)
    {
        (*this)[i].Draw(gc);
    }
}

void wxChartLabelGroup::UpdateSizes(wxGraphicsContext &gc)
{
	int LabelWidth;
    m_px_maxWidth = 0;

    for (size_t i = 0; i < size(); ++i)
    {
        (*this)[i].UpdateSize(gc);

        LabelWidth = (*this)[i].GetSize().GetWidth();
        if (LabelWidth > m_px_maxWidth)
        {
            m_px_maxWidth = LabelWidth;
        }
    }
}

//-_- wxDouble wxChartLabelGroup::GetMaxWidth() const
int wxChartLabelGroup::GetMaxWidth() const
{
    return m_px_maxWidth;
}
