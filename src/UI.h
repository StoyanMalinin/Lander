#pragma once

#include <vector>

#include "olcPixelGameEngine.h"

namespace ui
{
	struct Button
	{
		const olc::PixelGameEngine* pge;

		bool clicked;
		olc::vi2d pos1, pos2;//1 - topleft, 2 - bottomright

		std::string text;

		Button();
		Button(const olc::PixelGameEngine* pge);
		Button(const olc::PixelGameEngine* pge, const olc::vi2d& pos1, const olc::vi2d& pos2, const std::string text);

		bool checkCliked();
		void updateState();
	};
	void renderButton(const Button& b, olc::PixelGameEngine* pge);

	struct RadioButton
	{
		const olc::PixelGameEngine* pge;

		int r;
		olc::vi2d pos;

		bool clicked;
		std::string text;

		RadioButton();
		RadioButton(const olc::PixelGameEngine* pge);
		RadioButton(const olc::PixelGameEngine* pge, int r, const olc::vi2d& pos, const std::string text);

		bool checkClicked();
		void reset();
	};
	void renderRadioButton(const RadioButton& rb, olc::PixelGameEngine* pge);

	struct RadioButtonCluster
	{
		bool clicked = false;
		std::vector <RadioButton> radioButtons;

		void addButton(const RadioButton& rb);
		void updateState();

		void reset();
	};
	void renderRadioButtonCluster(const RadioButtonCluster& rbc, olc::PixelGameEngine* pge);
}