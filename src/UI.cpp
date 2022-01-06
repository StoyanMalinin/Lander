#include "UI.h"
#pragma once

namespace ui
{
	Button::Button() : clicked(false) {}
	Button::Button(const olc::PixelGameEngine* pge) : Button() 
	{
		this->pge = pge;
	}
	Button::Button(const olc::PixelGameEngine* pge, const olc::vi2d& pos1, const olc::vi2d& pos2, const std::string text) : Button(pge)
	{
		this->pos1 = pos1;
		this->pos2 = pos2;
		this->text = text;
	}

	bool Button::checkCliked()
	{
		if (pge->GetMouse(0).bHeld == false)
			return false;

		olc::vi2d mousePos = pge->GetMousePos();
		if (mousePos.x<pos1.x || mousePos.x>pos2.x) return false;
		if (mousePos.y<pos1.y || mousePos.y>pos2.y) return false;
	
		return true;
	}

	void Button::updateState()
	{
		clicked = checkCliked();
	}

	void renderButton(const Button& b, olc::PixelGameEngine* pge)
	{
		int w = b.pos2.x - b.pos1.x, h = b.pos2.y - b.pos1.y;
		pge->FillRect(b.pos1.x, b.pos1.y, w, h);

		olc::vi2d textSz = pge->GetTextSize(b.text);
		float wTextScale = 0.8f*((float)w)/((float)textSz.x), hTextScale = 0.2f * ((float)h) / ((float)textSz.y);

		pge->DrawStringDecal(olc::vf2d(b.pos1.x, b.pos1.y) + olc::vf2d(w*0.1, h*0.4), b.text, olc::BLACK, olc::vf2d(wTextScale, hTextScale));
	}

	RadioButton::RadioButton() : clicked(false) {}
	RadioButton::RadioButton(const olc::PixelGameEngine* pge) : RadioButton()
	{
		this->pge = pge;
	}
	RadioButton::RadioButton(const olc::PixelGameEngine* pge, int r, const olc::vi2d& pos, const std::string text) : RadioButton(pge)
	{
		this->r = r;
		this->pos = pos;
		this->text = text;
	}

	bool RadioButton::checkClicked()
	{
		if (pge->GetMouse(0).bHeld == false)
			return false;

		olc::vi2d mousePos = pge->GetMousePos();
		if ((mousePos - pos).mag2() <= r * r) return true;

		return false;
	}

	void RadioButton::reset()
	{
		clicked = false;
	}

	void renderRadioButton(const RadioButton& rb, olc::PixelGameEngine* pge)
	{
		pge->DrawCircle(rb.pos, rb.r, olc::BLACK);

		if(rb.clicked==false)
			pge->FillCircle(rb.pos, rb.r - 1, olc::WHITE);
		else
			pge->FillCircle(rb.pos, rb.r - 1, olc::BLACK);
		
		pge->DrawString(rb.pos + olc::vi2d(+2*rb.r, -rb.r*0.5), rb.text, olc::BLACK, 2);
	}

	void RadioButtonCluster::addButton(const RadioButton& rb)
	{
		this->radioButtons.push_back(rb);
	}

	void RadioButtonCluster::updateState()
	{
		for (auto it1 = radioButtons.begin(); it1 != radioButtons.end(); it1++)
		{
			if (it1->checkClicked() == true)
			{
				clicked = true;

				it1->clicked = true;
				for (auto it2 = radioButtons.begin(); it2 != radioButtons.end(); it2++)
				{
					if (it1 == it2) continue;
					it2->clicked = false;
				}
			}
		}
	}

	void RadioButtonCluster::reset()
	{
		clicked = false;
		for (RadioButton& rb : radioButtons)
			rb.reset();
	}

	void renderRadioButtonCluster(const RadioButtonCluster& rbc, olc::PixelGameEngine* pge)
	{
		for (const RadioButton& rb : rbc.radioButtons)
			renderRadioButton(rb, pge);
	}
}