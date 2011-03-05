/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/nwn/menu/ingamemain.cpp
 *  The ingame main menu.
 */

#include "aurora/talkman.h"

#include "engines/nwn/menu/ingamemain.h"
#include "engines/nwn/menu/yesnocancel.h"
#include "engines/nwn/menu/optionsgame.h"
#include "engines/nwn/menu/optionsvideo.h"
#include "engines/nwn/menu/optionssound.h"
#include "engines/nwn/menu/optionscontrols.h"

namespace Engines {

namespace NWN {

InGameMainMenu::InGameMainMenu() {
	load("options_main");

	// TODO: Load game
	getWidget("LoadButton", true)->setDisabled(true);

	// TODO: Save game
	getWidget("SaveButton", true)->setDisabled(true);

	// TODO: Save character
	getWidget("SaveCharButton", true)->setDisabled(true);

	_game     = new OptionsGameMenu(false);
	_video    = new OptionsVideoMenu(false);
	_sound    = new OptionsSoundMenu(false);
	_controls = new OptionsControlsMenu(false);

	_quitPrompt = new YesNoCancelDialog(TalkMan.getString(10308), false);
}

InGameMainMenu::~InGameMainMenu() {
	delete _quitPrompt;

	delete _controls;
	delete _sound;
	delete _video;
	delete _game;
}

void InGameMainMenu::initWidget(Widget &widget) {
	if (widget.getTag() == "BuildNumber") {
		dynamic_cast<WidgetLabel &>(widget).setText(PACKAGE_STRING);
		dynamic_cast<WidgetLabel &>(widget).setColor(0.6, 0.6, 0.6, 1.0);
		return;
	}

	if (widget.getTag() == "SaveCharButton#Caption") {
		dynamic_cast<WidgetLabel &>(widget).setText(TalkMan.getString(6560));
		return;
	}
}

void InGameMainMenu::callbackActive(Widget &widget) {
	if ((widget.getTag() == "ResumeGameButton") ||
	    (widget.getTag() == "XButton")) {
		_returnCode = 1;
		return;
	}

	if (widget.getTag() == "ExitButton") {
		if (sub(*_quitPrompt, 0, false) == 1) {
			_returnCode = 2;
			return;
		}

		show();
		return;
	}

	if (widget.getTag() == "GameButton") {
		sub(*_game);
		return;
	}

	if (widget.getTag() == "VideoOptionsButton") {
		sub(*_video);
		return;
	}

	if (widget.getTag() == "SoundOptionsButton") {
		sub(*_sound);
		return;
	}

	if (widget.getTag() == "ControlsButton") {
		sub(*_controls);
		return;
	}

}

} // End of namespace NWN

} // End of namespace Engines