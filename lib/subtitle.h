// vim: set noexpandtab:

#ifndef SUBTITLE_H
#define SUBTITLE_H

#include "text.h"
#include "font.h"
#include "dialog_line.h"
#include "boost/shared_ptr.hpp"

namespace grail {

	// Displays actors' lines in subtitles on the screen
	class Subtitle {

		public:
			Subtitle(boost::shared_ptr<DialogLine>, Font::Ptr);

			void eachFrame(uint32_t ticks);
			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;

			void start();
			bool isStarted() { return started; }
			bool isComplete() { return complete; }
			
			VirtualSize getSize() const {
				return text->getSize();
			}

			DialogLine::Ptr getDialogLine() {
				return dialogLine;
			}

		protected:
			boost::shared_ptr<DialogLine> dialogLine;
			Font::Ptr font;
			boost::shared_ptr<Text> text;

			// timer stuff (note should be put in timer wrapper
			uint32_t timeToLive;
			uint32_t timeStarted;
			bool started;
			bool complete;
	};

} // namespace grail

#endif // SUBTITLE_H

