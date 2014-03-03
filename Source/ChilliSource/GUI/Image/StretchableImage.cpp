//
//  StretchableImage.cpp
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Image/StretchableImage.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(StretchableImage)
		
		DEFINE_PROPERTY(SpriteSheet);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
		DEFINE_PROPERTY(BaseSpriteSheetIndexID);
		DEFINE_PROPERTY(CentreTouchConsumption);
		
		//--------------------------------------------------------
        /// Constructor
        /// 
        /// Empty
        //---------------------------------------------------------
        StretchableImage::StretchableImage()
		: HeightMaintain(false), WidthMaintain(false), CentreTouchConsumption(false)
        {
			memset(&msIndices, 0, sizeof(u32) * 9);
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //---------------------------------------------------------
        StretchableImage::StretchableImage(const Core::ParamDictionary& insParams) 
		: GUIView(insParams), HeightMaintain(false), WidthMaintain(false), CentreTouchConsumption(false)
        {
			CentreTouchConsumption = IsTouchConsumptionEnabled();
			
            std::string strValue;
			
			memset(&msIndices, 0, sizeof(u32) * 9);
            
            //---Sprite sheet
            Core::StorageLocation eSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SpriteSheetLocation", strValue))
            {
                eSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("SpriteSheet", strValue))
            {
				SetSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eSpriteSheetLocation, strValue));
			}
			
			//---Sprite sheet base name
			if(insParams.TryGetValue("BaseSpriteSheetIndexID", strValue))
            {
				SetBaseSpriteSheetIndexID(strValue);
			}
			
            //---Sprite sheet indices
            if(insParams.TryGetValue("TopLeftIndex", strValue))
            {
                msIndices.udwTopLeft = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("TopRightIndex", strValue))
            {
                msIndices.udwTopRight = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("BottomLeftIndex", strValue))
            {
                msIndices.udwBottomLeft = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("BottomRightIndex", strValue))
            {
                msIndices.udwBottomRight = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("TopIndex", strValue))
            {
                msIndices.udwTopCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("BottomIndex", strValue))
            {
                msIndices.udwBottomCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("LeftIndex", strValue))
            {
                msIndices.udwLeftCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("RightIndex", strValue))
            {
                msIndices.udwRightCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("CentreIndex", strValue))
            {
                msIndices.udwMiddleCentre = Core::ParseU32(strValue);
            }
            //---Index IDs
            if(insParams.TryGetValue("TopLeftIndexID", strValue))
            {
                msIndices.udwTopLeft = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("TopRightIndexID", strValue))
            {
                msIndices.udwTopRight = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("BottomLeftIndexID", strValue))
            {
                msIndices.udwBottomLeft = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("BottomRightIndexID", strValue))
            {
                msIndices.udwBottomRight = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("TopIndexID", strValue))
            {
                msIndices.udwTopCentre = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("BottomIndexID", strValue))
            {
                msIndices.udwBottomCentre = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("LeftIndexID", strValue))
            {
                msIndices.udwLeftCentre = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("RightIndexID", strValue))
            {
                msIndices.udwRightCentre = SpriteSheet->GetFrameIndexByID(strValue);
            }
            if(insParams.TryGetValue("CentreIndexID", strValue))
            {
                msIndices.udwMiddleCentre = SpriteSheet->GetFrameIndexByID(strValue);
            }
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::ParseBool(strValue);
			}
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::ParseBool(strValue);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
			//---Centre Touch Consumption
			if(insParams.TryGetValue("CentreTouchConsumption", strValue))
			{
				CentreTouchConsumption = Core::ParseBool(strValue);
			}
        }
        //---------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet containing the nine patches
        //---------------------------------------------------------
        void StretchableImage::SetSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
        {
            SpriteSheet = inpSpriteSheet;
        }
		//---------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite sheet containing the nine patches
		//---------------------------------------------------------
		const Rendering::SpriteSheetSPtr& StretchableImage::GetSpriteSheet() const
		{
			return SpriteSheet;
		}
		//---------------------------------------------------------
		/// Set Base Sprite Sheet Index ID
		///
		/// Set the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_TOP_LEFT"
		///			* "BLUE_PANEL_TOP_RIGHT"
		///			* "BLUE_PANEL_BOTTOM_LEFT"
		///			* "BLUE_PANEL_BOTTOM_RIGHT"
		///			* "BLUE_PANEL_TOP_CENTRE"
		///			* "BLUE_PANEL_BOTTOM_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_RIGHT"
		///			* "BLUE_PANEL_MIDDLE_LEFT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		void StretchableImage::SetBaseSpriteSheetIndexID(const std::string& instrID)
		{
			if(SpriteSheet)
			{
				BaseSpriteSheetIndexID = instrID;
				
				msIndices.udwTopLeft = SpriteSheet->GetFrameIndexByID(instrID + "TOP_LEFT");
				msIndices.udwTopRight = SpriteSheet->GetFrameIndexByID(instrID + "TOP_RIGHT");
				msIndices.udwBottomLeft = SpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_LEFT");
				msIndices.udwBottomRight = SpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_RIGHT");
				msIndices.udwTopCentre = SpriteSheet->GetFrameIndexByID(instrID + "TOP_CENTRE");
				msIndices.udwBottomCentre = SpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_CENTRE");
				msIndices.udwLeftCentre = SpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_LEFT");
				msIndices.udwRightCentre = SpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_RIGHT");
				msIndices.udwMiddleCentre = SpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_CENTRE");
			}
		}
		//---------------------------------------------------------
		/// Get Base Sprite Sheet Index ID
		///
		/// Get the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_TOP_LEFT"
		///			* "BLUE_PANEL_TOP_RIGHT"
		///			* "BLUE_PANEL_BOTTOM_LEFT"
		///			* "BLUE_PANEL_BOTTOM_RIGHT"
		///			* "BLUE_PANEL_TOP_CENTRE"
		///			* "BLUE_PANEL_BOTTOM_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_RIGHT"
		///			* "BLUE_PANEL_MIDDLE_LEFT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		const std::string& StretchableImage::GetBaseSpriteSheetIndexID() const
		{
			return BaseSpriteSheetIndexID;
		}
		//---------------------------------------------------------
        /// Set Patch Sprite Sheet Indices
        ///
        /// @param Struct containing the tpage index of each patch
        //---------------------------------------------------------
        void StretchableImage::SetSpriteSheetIndices(const StretchableImage::SpriteSheetIndex& insIndices)
        {
            msIndices = insIndices;
        }
        //---------------------------------------------------------
        /// Set Patch Sprite Sheet Indices
        ///
        /// Top Left
        /// Top Right
        /// Bottom Left
        /// Bottom Right
        /// Top Centre
        /// Bottom Centre
        /// Left Centre
        /// Right Centre
        /// Middle Centre
        ///
        /// @param Array containing the tpage index of each patch
        //---------------------------------------------------------
        void StretchableImage::SetSpriteSheetIndices(const u32* inpIndices)
        {
            msIndices.udwTopLeft = inpIndices[0];
            msIndices.udwTopRight = inpIndices[1];
            msIndices.udwBottomLeft = inpIndices[2];
            msIndices.udwBottomRight = inpIndices[3];
            msIndices.udwTopCentre = inpIndices[4];
            msIndices.udwBottomCentre = inpIndices[5];
            msIndices.udwLeftCentre = inpIndices[6];
            msIndices.udwRightCentre = inpIndices[7];
            msIndices.udwMiddleCentre = inpIndices[8];
        }
        //---------------------------------------------------------
        /// Draw
        /// 
        /// Draw the image constructed from the nine patches
        ///
        /// @param Canvas renderer pointer
        //---------------------------------------------------------
        void StretchableImage::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
			//Check if this is on screen
			Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::Screen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::Screen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
			
            if(Visible && SpriteSheet)
            {
                Core::Vector2 vPanelPos = GetAbsoluteScreenSpacePosition();
                Core::Vector2 vTopLeft = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
                Core::Vector2 vPatchPos;
                
                Core::Colour AbsColour = GetAbsoluteColour();
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::Matrix3x3 matTransform;
                Core::Matrix3x3 matPatchTransform;
                Core::Matrix3x3 matViewTransform;
                
                matViewTransform.SetTransform(vPanelPos, Core::Vector2(1, 1), GetAbsoluteRotation());
				
				// Retrieve each bit's size
				PatchSize sPatchSize;
				CalculatePatchSize(sPatchSize);
                
                // Render ourself
				
                // Draw the top left corner
                matPatchTransform.Translate(vTopLeft);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   sPatchSize.vSizeTopLeft, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwTopLeft), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                // Draw the top right corner
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeTopRight,  
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwTopRight), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                // Draw the bottom left corner
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeBottomLeft, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwBottomLeft), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                // Draw the bottom right corner
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeBottomRight,  
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwBottomRight), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomRight);
                
                // Draw the top
				vPatchPos.x = vTopLeft.x + sPatchSize.vSizeTopLeft.x;
				vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre).y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeTopCentre, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwTopCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
				
                // Draw the bottom
				vPatchPos.x = vTopLeft.x + sPatchSize.vSizeBottomLeft.x;
				vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomCentre).y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeBottomCentre, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwBottomCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                // Draw the left
				vPatchPos.x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleLeft).x;
				vPatchPos.y = vTopLeft.y - sPatchSize.vSizeTopLeft.y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeLeftCentre, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwLeftCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                // Draw the right
				vPatchPos.x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleRight).x;
				vPatchPos.y = vTopLeft.y - sPatchSize.vSizeTopRight.y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeRightCentre, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwRightCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                // Draw the centre
				vPatchPos.x = vTopLeft.x + sPatchSize.vSizeTopLeft.x;
				vPatchPos.y = vTopLeft.y - sPatchSize.vSizeTopLeft.y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   sPatchSize.vSizeMiddleCentre, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwMiddleCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                // Render subviews
                GUIView::Draw(inpCanvas);
            }
		}
		//--------------------------------------------------------
		/// Set Width Maintaining Aspect
		///
		/// Change the width of the image and resize the height
		/// to maintain the aspect ratio
		///
		/// @param Unified width
		//--------------------------------------------------------
		void StretchableImage::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
		{
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
			SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
			SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
		}
		//--------------------------------------------------------
		/// Set Height Maintaining Aspect
		///
		/// Change the height of the image and resize the width
		/// to maintain the aspect ratio
		///
		/// @param Unified height
		//--------------------------------------------------------
		void StretchableImage::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
		{
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
			SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsWidth = (fAspectRatio * vCurrentSize.y) / fScaleX;
			SetSize(0.0f, infRelHeight, fAbsWidth, infAbsHeight);
		}
		//--------------------------------------------------------
		/// Enable Height Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void StretchableImage::EnableHeightMaintainingAspect(bool inbEnabled)
		{
			HeightMaintain = inbEnabled;
		}
		//--------------------------------------------------------
		/// Enable Width Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void StretchableImage::EnableWidthMaintainingAspect(bool inbEnabled)
		{
			WidthMaintain = inbEnabled;            
		}
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return auto scaling of the Width to maintain the aspect ratio
		//--------------------------------------------------------
		bool StretchableImage::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool StretchableImage::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
		
		void StretchableImage::CalculatePatchSize(PatchSize& outPatchSize)
		{
			Core::Vector2 vPanelSize = GetAbsoluteSize();

			//Get the patch sizes
			outPatchSize.vSizeTopLeft = SpriteSheet->GetSizeForFrame(msIndices.udwTopLeft);
			outPatchSize.vSizeTopRight = SpriteSheet->GetSizeForFrame(msIndices.udwTopRight);
			outPatchSize.vSizeBottomLeft = SpriteSheet->GetSizeForFrame(msIndices.udwBottomLeft);
			outPatchSize.vSizeBottomRight = SpriteSheet->GetSizeForFrame(msIndices.udwBottomRight);
			
			//Check to see if they are going to fit in the bounds of the view
			f32 fTotal = outPatchSize.vSizeTopLeft.y + outPatchSize.vSizeBottomLeft.y;
			if(fTotal > vPanelSize.y)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.y/fTotal;
				outPatchSize.vSizeTopLeft.y *= fScale;
				outPatchSize.vSizeBottomLeft.y *= fScale;
			}
			fTotal = outPatchSize.vSizeTopRight.y + outPatchSize.vSizeBottomRight.y ;
			if(fTotal > vPanelSize.y)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.y/fTotal;
				outPatchSize.vSizeTopRight.y *= fScale;
				outPatchSize.vSizeBottomRight.y *= fScale;
			}
			fTotal = outPatchSize.vSizeTopLeft.x + outPatchSize.vSizeTopRight.x;
			if(fTotal > vPanelSize.x)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.x/fTotal;
				outPatchSize.vSizeTopLeft.x *= fScale;
				outPatchSize.vSizeTopRight.x *= fScale;
			}
			fTotal = outPatchSize.vSizeBottomLeft.x + outPatchSize.vSizeBottomRight.x;
			if(fTotal > vPanelSize.x)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.x/fTotal;
				outPatchSize.vSizeBottomLeft.x *= fScale;
				outPatchSize.vSizeBottomRight.x *= fScale;
			}
			
			// Calculate the top
			outPatchSize.vSizeTopCentre.x = vPanelSize.x - (outPatchSize.vSizeTopLeft.x + outPatchSize.vSizeTopRight.x);
			outPatchSize.vSizeTopCentre.y = outPatchSize.vSizeTopLeft.y;
			
            // Calculate the bottom
			outPatchSize.vSizeBottomCentre.x = vPanelSize.x - (outPatchSize.vSizeBottomLeft.x + outPatchSize.vSizeBottomRight.x);
			outPatchSize.vSizeBottomCentre.y = outPatchSize.vSizeBottomLeft.y;
			
			// Calculate the left
			outPatchSize.vSizeLeftCentre.y = vPanelSize.y - (outPatchSize.vSizeTopLeft.y + outPatchSize.vSizeBottomLeft.y);
			outPatchSize.vSizeLeftCentre.x = outPatchSize.vSizeTopLeft.x;
			
			// Calculate the right
			outPatchSize.vSizeRightCentre.y = vPanelSize.y - (outPatchSize.vSizeTopRight.y + outPatchSize.vSizeBottomRight.y);
			outPatchSize.vSizeRightCentre.x = outPatchSize.vSizeTopRight.x;
			
			// Calculate the centre
			outPatchSize.vSizeMiddleCentre.x = vPanelSize.x - (outPatchSize.vSizeLeftCentre.x + outPatchSize.vSizeRightCentre.x);
			outPatchSize.vSizeMiddleCentre.y = vPanelSize.y - (outPatchSize.vSizeTopCentre.y + outPatchSize.vSizeBottomCentre.y);
		}
		
		//--------------------------------------------------------
		/// Enable Centre Touch Consumption
		///
		/// Enables the touch to go through the middle part of the image
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void StretchableImage::EnableCentreTouchConsumption(bool inbEnabled)
		{
			CentreTouchConsumption = inbEnabled;
		}
		
		//--------------------------------------------------------
		/// Is Centre Touch Consumption Enabled
		///
		/// @return whether the touch though the middle is enabled or not
		//--------------------------------------------------------
		bool StretchableImage::IsCentreTouchConsumptionEnabled() const
		{
			return CentreTouchConsumption;
		}
		
		bool StretchableImage::OnTouchBegan(const Input::TouchInfo &insTouchInfo)
		{
			bool bConsumed = GUIView::OnTouchBegan(insTouchInfo);
			
			if (!CentreTouchConsumption && bConsumed && IsTouchConsumptionEnabled(TouchType::k_began))
			{
				// If the patch contains the touch
				if(Contains(insTouchInfo.vLocation))
				{
					// Retrieve the patch sizes
					PatchSize sPatchSize;
					CalculatePatchSize(sPatchSize);
					
					// Get the AABB of the centre
					Core::Rectangle sMiddleAABB(GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft) + sPatchSize.vSizeBottomLeft, sPatchSize.vSizeMiddleCentre);
					
					// If the touch is not located in the centre, we consume it
					bool bContainsMiddle = sMiddleAABB.Contains(insTouchInfo.vLocation);
					
					bConsumed = !bContainsMiddle;
				}
			}
			
			return bConsumed;
		}
		
		bool StretchableImage::OnTouchMoved(const Input::TouchInfo & insTouchInfo)
		{
			// Special treatment if the middle image has to let the touch through
			if (!CentreTouchConsumption && IsTouchConsumptionEnabled(TouchType::k_moved))
			{
				if(UserInteraction)
				{
					if(mbIsBeingDragged)
					{
						if(!AlignedWithParent)
						{
							SetPosition(Core::UnifiedVector2(Core::Vector2::ZERO, insTouchInfo.vLocation));
						}
						else
						{
							SetOffsetFromParentAlignment(Core::UnifiedVector2(Core::Vector2::ZERO, insTouchInfo.vLocation));
						}
					}
					
					mSubviewsCopy = mSubviews;
					
					for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
					{
						if((*it)->OnTouchMoved(insTouchInfo))
						{
							mSubviewsCopy.clear();
							return true;
						}
					}
					
					//Check for input events
					//If we contain this touch we can consume it
					if(mInputEvents.OnTouchMoved(this, insTouchInfo) && IsTouchConsumptionEnabled(TouchType::k_moved))
					{
						// Retrieve the patch sizes
						PatchSize sPatchSize;
						CalculatePatchSize(sPatchSize);
						
						// Get the AABB of the centre
						Core::Rectangle sMiddleAABB(GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft) + sPatchSize.vSizeBottomLeft, sPatchSize.vSizeMiddleCentre);
						
						// If the touch is not located in the centre, we consume it
						bool bContainsMiddle = sMiddleAABB.Contains(insTouchInfo.vLocation);
						
						if (bContainsMiddle)
						{
							mSubviewsCopy.clear();
						}
						
						return !bContainsMiddle;
					}
				}
				
				//We have no user interaction enabled
				mSubviewsCopy.clear();
				return false;
			}
			
			return GUIView::OnTouchMoved(insTouchInfo);
		}
    }
}
