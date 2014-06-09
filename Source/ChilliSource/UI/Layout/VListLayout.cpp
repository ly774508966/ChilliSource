//
//  VListLayout.cpp
//  Chilli Source
//  Created by Scott Downie on 23/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/UI/Layout/VListLayout.h>

#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetNumCells(u32 in_numCells)
        {
            CS_ASSERT(in_numCells > 0, "Cannot create a list with 0 cells");
            m_numCells = in_numCells;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_marginSizeTop.x = in_top;
            m_marginSizeRight.x = in_right;
            m_marginSizeBottom.x = in_bottom;
            m_marginSizeLeft.x = in_left;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_marginSizeTop.y = in_top;
            m_marginSizeRight.y = in_right;
            m_marginSizeBottom.y = in_bottom;
            m_marginSizeLeft.y = in_left;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetRelativeSpacing(f32 in_spacing)
        {
            m_spacingSize.x = in_spacing;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetAbsoluteSpacing(f32 in_spacing)
        {
            m_spacingSize.y = in_spacing;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::BuildLayout(const Widget* in_root, const std::vector<WidgetSPtr>& in_widgets)
        {
            CS_ASSERT(in_root != nullptr, "Cannot have null root");
            
            //The list doesn't care about the widgets as the size and position of each cell
            //is only relative to the number of cells and the root widget size
            Core::Vector2 rootSize = in_root->GetFinalSize();
            
            //The margins and spacing are relative to the root widget size
            m_finalSpacingSize = m_spacingSize.y + (rootSize.y * m_spacingSize.x);
            
            f32 finalMarginTop = m_marginSizeTop.y + (rootSize.y * m_marginSizeTop.x);
            f32 finalMarginBottom = m_marginSizeBottom.y + (rootSize.y * m_marginSizeBottom.x);
            m_finalMarginLeft = m_marginSizeLeft.y + (rootSize.x * m_marginSizeLeft.x);
            f32 finalMarginRight = m_marginSizeRight.y + (rootSize.x * m_marginSizeRight.x);
            
            m_cellSize.y = (rootSize.y - finalMarginTop - finalMarginBottom - (m_finalSpacingSize * (m_numCells-1)))/m_numCells;
            m_cellSize.x = rootSize.x - m_finalMarginLeft - finalMarginRight;
            
            m_firstCellPosY = rootSize.y - finalMarginTop;
        }
        //----------------------------------------------------------------------------------------
        /// The cell size if fixed and uniform so the index is not required
        //----------------------------------------------------------------------------------------
        Core::Vector2 VListLayout::GetSizeForIndex(u32 in_index) const
        {
            CS_ASSERT(in_index < m_numCells, "Cannot have more items in list than number of cells(" + Core::ToString(m_numCells) + ")");
            return m_cellSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 VListLayout::GetPositionForIndex(u32 in_index) const
        {
            //Aligned to the middle centre in cell space
            CS_ASSERT(in_index < m_numCells, "Cannot have more items in list than number of cells(" + Core::ToString(m_numCells) + ")");
            
            f32 spacingSize = in_index == 0 ? 0.0f : m_finalSpacingSize;
            
            f32 yPos = m_firstCellPosY - (m_cellSize.y * (in_index + 1)) - (spacingSize * in_index);
            yPos += (m_cellSize.y * 0.5f);
            
            return Core::Vector2(m_cellSize.x * 0.5f + m_finalMarginLeft, yPos);
        }
    }
}