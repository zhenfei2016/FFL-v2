

#include <utils/FFL_File.hpp>


namespace FFL {
	FFL::File*  gTrackFile;
	void postTraceinfo(const String& info) {
		if (gTrackFile == NULL) {
			gTrackFile = new FFL::File();
			gTrackFile->create("e:\\pipeline_track.txt");
		}

		if (gTrackFile) {
			gTrackFile->write((void*)info.c_str(), info.size(), 0);
			gTrackFile->write("\r", 1, 0);
		}
		//FFL_LOG_INFO_TAG("track", info.c_str());
	}
}